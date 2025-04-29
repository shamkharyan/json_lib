#ifndef JSON_PARSING_HPP_
#define JSON_PARSING_HPP_

#include "json_value.hpp"
#include <string>

namespace json
{
	json::value from_string(const std::string& json);
	json::value from_file(const std::string& path);
}

#endif // JSON_PARSING_HPP_