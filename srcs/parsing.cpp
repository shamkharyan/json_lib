#include "json_lib.hpp"
#include <stack>
#include <sstream>
#include <unordered_map>

namespace json
{
	static bool is_sep(char c);
	static std::vector<std::string> tokenize(const std::string& json);
	static bool is_null_str(const std::string& json);
	static bool is_bool_str(const std::string& json);
	static bool is_string_str(const std::string& json);
	static bool is_num_str(const std::string& json);
	static void from_valid_tokens(json::value& val, const std::vector<std::string>& tokens, std::size_t& i);

	bool is_sep(char c)
	{
    	return
        	c == '{' || c == '}' || 
        	c == '[' || c == ']' ||
        	c == ':' || c == ',';
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
			else if (json[i] == '\"')
			{
				tokens.emplace_back(1, json[i++]);
				while (i < json.size() && (json[i] != '\"' || json[i - 1] == '\\'))
					tokens.back().push_back(json[i++]);
				if (i < json.size())
					tokens.back().push_back(json[i++]);
			}
			else
			{
				tokens.emplace_back(1, json[i++]);
				while (i < json.size() && !isspace(json[i]) && !is_sep(json[i]))
					tokens.back().push_back(json[i++]);
			}
		}
		return tokens;
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
		return json.front() == '\"' && json.back() == '\"';
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

	// {    -> key, } +
	// }    -> }, ], ,, EOF
	// key  -> : 
	// :    -> val, [, {
	// val  -> ,, ], }
	// [    -> [, {, val, ]
	// ]    -> ], }, ,, EOF
	// ,    -> key (in obj), val (in arr)
	// strt -> {, [, val

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
				if (tokens[i] == ",") ++i; //skip ,
			}
			++i; //skip ]
			val = temp_arr;
		}
	}

	json::value from_string(const std::string& json)
	{
		std::vector<std::string> tokens = tokenize(json);

		json::value val;
		std::size_t i = 0;

		from_valid_tokens(val, tokens, i);

		return val;
	}
}