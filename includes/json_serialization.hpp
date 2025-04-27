#ifndef JSON_SERIALIZATION_HPP_
#define JSON_SERIALIZATION_HPP_

#include "json_value.hpp"
#include <string>

namespace json
{
	std::string to_string(const value& val);
}

#endif // JSON_SERIALIZATION_HPP_