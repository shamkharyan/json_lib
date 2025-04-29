#include "json_serialization.hpp"
#include "json_value.hpp"
#include "json_exceptions.hpp"
#include <fstream>

void to_string_recursive(const json::value& val, std::string& json);

std::string json::to_string(const json::value& val)
{
	std::string json;
	to_string_recursive(val, json);
	return json;
}

void to_file(const json::value& val, const std::string& path)
{
	std::ofstream file(path);

	if (!file.is_open())
		throw json::json_parse_error("Can't open file to serialize");
	
	file << json::to_string(val);
	file.close();
}

void to_string_recursive(const json::value& val, std::string& json)
{
	if (val.is_null())
		json += "null";
	else if (val.is_bool())
		json += val.as_bool()? "true" : "false";
	else if (val.is_string())
	{
		json.push_back('"');
		json += val.as_string();
		json.push_back('"');
	}
	else if (val.is_number())
	{
		std::string n = std::to_string(val.as_number());
		n.erase(n.find_last_not_of('0') + 1, std::string::npos);
		n.erase(n.find_last_not_of('.') + 1, std::string::npos);
		json += n;
	}
	else if (val.is_array())
	{
		json.push_back('[');
		for (const json::value& v : val.as_array())
		{
			to_string_recursive(v, json);
			json.push_back(',');
		}
		if (json.back() == ',')
			json.back() = ']';
		else
			json.push_back(']');
	}
	else if (val.is_object())
	{
		json.push_back('{');
		for (const auto& v : val.as_object())
		{
			json += "\"" + v.first + "\":";
			to_string_recursive(v.second, json);
			json.push_back(',');
		}
		if (json.back() == ',')
			json.back() = '}';
		else
			json.push_back('}');
	}
}