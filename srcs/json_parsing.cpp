#include "json_parsing.hpp"
#include "json_value.hpp"
#include "json_exceptions.hpp"
#include <stack>
#include <sstream>
#include <unordered_map>
#include <fstream>

namespace json
{
	static bool is_sep(char c);
	static bool is_null_str(const std::string& json);
	static bool is_bool_str(const std::string& json);
	static bool is_string_str(const std::string& json);
	static bool is_num_str(const std::string& json);
	static void validate_tokens(const std::vector<std::string>& tokens);
	static std::vector<std::string> tokenize(const std::string& json);
	static void from_valid_tokens(json::value& val, const std::vector<std::string>& tokens, std::size_t& i);

	json::value from_string(const std::string& json)
	{
		std::vector<std::string> tokens = tokenize(json);

		validate_tokens(tokens); //if invalid tokens, throws an exception

		json::value val;
		std::size_t i = 0;

		from_valid_tokens(val, tokens, i);

		return val;
	}

	json::value from_file(const std::string& path)
	{
		std::ifstream file(path, std::ios::binary);
		std::string json;

		if (!file.is_open())
			throw json_parse_error("Failed to read from: " + path);

		file.seekg(0, std::ios::end);
		std::size_t len = file.tellg();
		file.seekg(0, std::ios::beg);

		json.reserve(len);

		constexpr std::size_t buff_size = 1024;
		char buff[buff_size];

		while (file.read(buff, buff_size))
			json.append(buff, file.gcount());
		json.append(buff, file.gcount());

		if (file.bad())
			throw json_parse_error("Error reading file: " + path);

		if (json.empty())
			throw json_parse_error("Empty file: " + path);
		
		return from_string(json);
	}

	bool is_sep(char c)
	{
    	return
        	c == '{' || c == '}' || 
        	c == '[' || c == ']' ||
        	c == ':' || c == ',';
	}

	bool is_null_str(const std::string& json)
	{
		return json == "null";
	}

	bool is_bool_str(const std::string& json)
	{
		return json == "true" || json == "false";
	}

	bool is_string_str(const std::string& json)
	{
		return json.size() >= 2 && json.front() == '"' && json.back() == '"';
	}

	bool is_num_str(const std::string& json)
	{
		std::stringstream ss(json);
		std::string remain;
		double val;
		
		if (ss >> val)
		{
			ss >> remain;
			return remain.empty();
		}
		return false;
	}

	std::vector<std::string> tokenize(const std::string& json)
	{
		std::vector<std::string> tokens;
		std::size_t i = 0;

		while (i < json.size())
		{
			if (isspace(json[i]))
				++i;
			else if (is_sep(json[i]))
				tokens.emplace_back(1, json[i++]);
			else if (json[i] == '"')
			{
				tokens.emplace_back(1, json[i++]); //push_backs '"'
				while (i < json.size() && (json[i] != '"' || json[i - 1] == '\\'))
					tokens.back().push_back(json[i++]);
				if (i < json.size()) //if json is valid, this is always true
					tokens.back().push_back(json[i++]); //push_backs '"' 
			}
			else
			{
				tokens.emplace_back(1, json[i++]);
				while (i < json.size() && !isspace(json[i]) && !is_sep(json[i]) && json[i] != '"')
					tokens.back().push_back(json[i++]);
			}
		}
		return tokens;
	}

	void validate_tokens(const std::vector<std::string>& tokens)
	{
		std::stack<char> brackets;
		bool expect_key = false;
		bool expect_val = true;
		bool in_arr = false;
		bool in_obj = false;
		bool expect_comma = false;
		bool expect_dots = false;
		char last;

		for (const std::string& token : tokens)
		{
			if (token == "{")
			{
				if (!expect_val)
					throw json::json_parse_error("Token '{' in invalid place");
				brackets.push('{');
				expect_key = true;
				expect_val = false;
				in_obj = true;
				in_arr = false;
				expect_comma = false;
				expect_dots = false;
				last = '{';
			}
			else if (token == "[")
			{
				if (!expect_val)
					throw json::json_parse_error("Token '[' in invalid place");
				brackets.push('[');
				expect_key = false;
				expect_val = true;
				in_obj = false;
				in_arr = true;
				expect_comma = false;
				expect_dots = false;
				last = '[';
			}
			else if (token == "}")
			{
				if (brackets.empty() || brackets.top() != '{')
					throw json::json_parse_error("Token '}' in invalid place");
				if (last == ',' || last == ':' || last == 'k')
					throw json::json_parse_error("Trailing comma, ':' or key");
				brackets.pop();
				in_obj = (!brackets.empty() && brackets.top() == '{');
				in_arr = (!brackets.empty() && brackets.top() == '[');
				expect_comma = true;
				expect_dots = false;
				expect_key = in_obj;
				expect_val = in_arr;
				last = '}';
			}
			else if (token == "]")
			{
				if (brackets.empty() || brackets.top() != '[')
					throw json::json_parse_error("Token ']' in invalid place");
				if (last == ',' || last == ':' || last == 'k')
					throw json::json_parse_error("Trailing comma,':' or key");
				brackets.pop();
				in_obj = (!brackets.empty() && brackets.top() == '{');
				in_arr = (!brackets.empty() && brackets.top() == '[');
				expect_comma = true;
				expect_dots = false;
				expect_key = in_obj;
				expect_val = in_arr;
				last = ']';
			}
			else if (token == ":")
			{
				if (!expect_dots || !in_obj || in_arr)
					throw json::json_parse_error("Token ':' in invalid place");
				expect_val = true;
				expect_key = false;
				expect_comma = false;
				expect_dots = false;
				last = ':';
			}
			else if (token == ",")
			{
				if (!expect_comma)
					throw json::json_parse_error("Token ',' in invalid place");
				expect_comma = false;
				last = ',';
				if (in_arr)
				{
					expect_val = true;
					expect_key = false;
				}
				else if (in_obj)
				{
					expect_val = false;
					expect_key = true;
				}
				else
					throw json::json_parse_error("Token ',' in invalid place");
			}
			else
			{
				if (in_obj && expect_key)
				{
					if (!is_string_str(token))
						throw json::json_parse_error("Token " + token + " in invalid place");
					expect_dots = true;
					expect_comma = false;
					expect_val = false;
					expect_key = false;
					last = 'k';
				}
				else if (expect_val)
				{
					if (!is_bool_str(token) && !is_null_str(token) && !is_string_str(token) && !is_num_str(token))
						throw json::json_parse_error("Invalid token: " + token);
					expect_dots = false;
					expect_val = false;
					expect_comma = true;
					last = 'v';
				}
				else
					throw json::json_parse_error("Unexpected token: " + token);
			}
		}
		if (expect_val)
    		throw json::json_parse_error("Expected value but reached end of input");
		if (expect_dots)
			throw json::json_parse_error("Expected ':' but reached end of input");
		if (!brackets.empty())
			throw json::json_parse_error("Brackets mismatch");
	}

	void from_valid_tokens(json::value& val, const std::vector<std::string>& tokens, std::size_t& i)
	{
		if (is_null_str(tokens[i]))
		{
			val = nullptr;
			++i;
		}
		else if (is_bool_str(tokens[i]))
		{
			val = (tokens[i] == "true");
			++i;
		}
		else if (is_num_str(tokens[i]))
		{
			val = std::stod(tokens[i]);
			++i;
		}
		else if (is_string_str(tokens[i]))
		{
			val = tokens[i].substr(1, tokens[i].size() - 2); //remove quotes
			++i;
		}
		else if (tokens[i] == "{")
		{
			json::value temp = json::object({});
			++i; //skip {
			while (tokens[i] != "}")
			{
				std::string key = tokens[i].substr(1, tokens[i].size() - 2); //remove quotes
				i += 2; //skip key and :
				from_valid_tokens(temp[key], tokens, i);
				if (tokens[i] == ",") ++i; //skip , if available
			}
			++i; //skip }
			val = temp;
		}
		else if (tokens[i] == "[")
		{
			json::value temp_arr = json::array({});
			++i; //skip [
			while (tokens[i] != "]")
			{
				json::value temp_val;
				from_valid_tokens(temp_val, tokens, i);
				temp_arr.as_array().push_back(temp_val);
				if (tokens[i] == ",") ++i; //skip , if available
			}
			++i; //skip ]
			val = temp_arr;
		}
	}
}