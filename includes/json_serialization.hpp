#ifndef JSON_SERIALIZATION_HPP_
#define JSON_SERIALIZATION_HPP_

#include "json_value.hpp"
#include <string>

namespace json
{
	std::string to_string(const value& val);
	void to_file(const value& val, const std::string& path);
}

#endif // JSON_SERIALIZATION_HPP_