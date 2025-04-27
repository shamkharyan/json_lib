#ifndef JSON_EXCEPTIONS_HPP_
#define JSON_EXCEPTIONS_HPP_

#include <string>
#include <stdexcept>

namespace json
{
	class json_value_error : public std::runtime_error
	{
	public:
		explicit json_value_error(const std::string& what_arg) : std::runtime_error(what_arg) {}
		explicit json_value_error(const char *what_arg) : std::runtime_error(what_arg) {}
	};

	class json_parse_error : public std::runtime_error
	{
	public:
		explicit json_parse_error(const std::string& what_arg) : std::runtime_error(what_arg) {}
		explicit json_parse_error(const char *what_arg) : std::runtime_error(what_arg) {}
	};
}

#endif // JSON_EXCEPTIONS_HPP_