#include "value.hpp"
#include <stack>
#include <sstream>
#include <unordered_map>

namespace json
{
	static bool is_sep(char c)
	{
    	return
        	c == '{' || c == '}' || 
        	c == '[' || c == ']' ||
        	c == ':' || c == ',';
	}

	static std::vector<std::string> tokenize(const std::string& json)
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

	static bool is_null_str(const std::string& json)
	{
		return json == "null";
	}

	static bool is_bool_str(const std::string& json)
	{
		return json == "true" || json == "false";
	}

	static bool is_string_str(const std::string& json)
	{
		return json.front() == '\"' && json.back() == '\"';
	}

	static bool is_num_str(const std::string& json)
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


	// -1 -> delim
	// 0  -> key
	// 1  -> val

	// {    -> key, } +
	// }    -> }, ], ,, EOF
	// key  -> : 
	// :    -> val, [, {
	// val  -> ,, ], }
	// [    -> [, {, val, ]
	// ]    -> ], }, ,, EOF
	// ,    -> key (in obj), val (in arr)
	// strt -> {, [, val
	void from_string_help(value& val, const std::vector<std::string>& tokens, std::size_t i, int dkv, bool in_arr)
	{
		if (is_null_str(tokens[i]))
		{
			if (dkv == 1)
				val = nullptr;
			else
				throw json::json_value_error("Key can\'t be null");
		}
		else if (is_bool_str(tokens[i]))
		{
			if (dkv == 1)
				val = (tokens[i] == "true");
			else
				throw json::json_value_error("Key can\'t be bool");
		}
		else if (is_num_str(tokens[i]))
		{
			if (dkv == 1)
				val = std::stod(tokens[i]);
			else
				throw json::json_value_error("Key can\'t be number");
		}
		else if (is_string_str(tokens[i]))
		{
			if (dkv == 1)
				val = tokens[i];
			else if (dkv == 0)
				from_string_help(val[tokens[i]], tokens, i + 1, -1, in_arr);
			else
				throw json::json_value_error("Key can\'t be number");
		}
		else if (tokens[i] == "{")
		{
			if (dkv == 1)
				from_string_help(val, tokens, i + 1, 0, in_arr);
			else
				throw json::json_value_error("Key can\'t be number");
		}
		else if (tokens[i] == "[")
		{
			if (dkv == 1)
				from_string_help(val, tokens, i + 1, 1, true);
			else
				throw json::json_value_error("Key can\'t be number");
		}
		else if (tokens[i] == ",")
		{
			if (dkv == -1 && in_arr)
				from_string_help(val, tokens, i + 1, 1, in_arr);
			else if (dkv == -1 && !in_arr)
				from_string_help(val, tokens, i + 1, 0, in_arr);
			else
				throw json::json_value_error("Key can\'t be number");
		}

		
	}

	static bool validate(const std::vector<std::string>& tokens)
	{
		std::stack<char> brackets;
		bool is_key = false;
		bool in_arr = false;
		char c = 's';
		std::unordered_map<char, std::string> expected = {
			{'{', "k}"},
			{'}', "}],e"},
			{'k', ":"},
			{':', "v[{"},
			{'v', ",]}"},
			{'[', "[{v]"},
			{']', "]},e"},
			{',', "kv"},
			{'s', "{[v"}
		};

		if (tokens.empty())
			return false;
	}

	value from_string(const std::string& json)
	{
		std::vector<std::string> tokens = tokenize(json);
		std::stack<char> brackets;
		
		std::unordered_map<char, std::string> expected = {
			{'{', "k}"},
			{'}', "}],e"},
			{'k', ":"},
			{':', "v[{"},
			{'v', ",]}"},
			{'[', "[{v]"},
			{']', "]},e"},
			{',', "kv"},
			{'s', "{[v"}
		};







	}
}