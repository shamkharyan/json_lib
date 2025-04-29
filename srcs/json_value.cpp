#include "json_value.hpp"
#include "json_exceptions.hpp"
#include "json_serialization.hpp"
#include <algorithm>
#include <string>
#include <variant>
#include <fstream>
#include <sstream>

//operator[] for array
typename json::value& json::value::operator[](std::size_t i)
{
	if (std::holds_alternative<jarray>(m_value))
		return std::get<jarray>(m_value)[i];
	throw json::json_value_error("Can't index non array type with integer");
}

const typename json::value& json::value::operator[](std::size_t i) const
{
	if (std::holds_alternative<jarray>(m_value))
		return std::get<jarray>(m_value)[i];
	throw json::json_value_error("Can't index non array type with integer");
}

//operator[] for object
typename json::value& json::value::operator[](const std::string& key)
{
	if (std::holds_alternative<jobject>(m_value))
		return std::get<jobject>(m_value)[key];
	throw json::json_value_error("Can't index non object type with string");
}

//at for array
typename json::value& json::value::at(std::size_t i)
{
	if (std::holds_alternative<jarray>(m_value))
		return std::get<jarray>(m_value).at(i);
	throw json::json_value_error("Can't index non array type with integer");
}

const typename json::value& json::value::at(std::size_t i) const
{
	if (std::holds_alternative<jarray>(m_value))
		return std::get<jarray>(m_value).at(i);
	throw json::json_value_error("Can't index non array type with integer");
}

//at for object
typename json::value& json::value::at(const std::string& key)
{
	if (std::holds_alternative<jobject>(m_value))
		return std::get<jobject>(m_value).at(key);
	throw json::json_value_error("Can't index non object type with string");
}

const typename json::value& json::value::at(const std::string& key) const
{
	if (std::holds_alternative<jobject>(m_value))
		return std::get<jobject>(m_value).at(key);
	throw json::json_value_error("Can't index non object type with string");
}

bool json::value::is_number() const noexcept { return std::holds_alternative<double>(m_value); }
bool json::value::is_object() const noexcept { return std::holds_alternative<json::value::jobject>(m_value); }
bool json::value::is_array() const noexcept { return std::holds_alternative<json::value::jarray>(m_value); }
bool json::value::is_string() const noexcept { return std::holds_alternative<std::string>(m_value); }
bool json::value::is_bool() const noexcept { return std::holds_alternative<bool>(m_value); }
bool json::value::is_null() const noexcept { return std::holds_alternative<std::nullptr_t>(m_value); }

json::value::jarray& json::value::as_array()
{
	if (std::holds_alternative<jarray>(m_value))
		return std::get<jarray>(m_value);
	throw json::json_value_error("Value type is not array");
}

const json::value::jarray& json::value::as_array() const
{
	if (std::holds_alternative<jarray>(m_value))
		return std::get<jarray>(m_value);
	throw json::json_value_error("Value type is not array");
}

json::value::jobject& json::value::as_object()
{
	if (std::holds_alternative<jobject>(m_value))
		return std::get<jobject>(m_value);
	throw json::json_value_error("Value type is not array");
}

const json::value::jobject& json::value::as_object() const
{
	if (std::holds_alternative<jobject>(m_value))
		return std::get<jobject>(m_value);
	throw json::json_value_error("Value type is not array");
}

std::string& json::value::as_string()
{
	if (std::holds_alternative<std::string>(m_value))
		return std::get<std::string>(m_value);
	throw json::json_value_error("Value type is not string");
}

const std::string& json::value::as_string() const
{
	if (std::holds_alternative<std::string>(m_value))
		return std::get<std::string>(m_value);
	throw json::json_value_error("Value type is not string");
}

double& json::value::as_number()
{
	if (std::holds_alternative<double>(m_value))
		return std::get<double>(m_value);
	throw json::json_value_error("Value type is not number");
}

const double& json::value::as_number() const
{
	if (std::holds_alternative<double>(m_value))
		return std::get<double>(m_value);
	throw json::json_value_error("Value type is not number");
}

bool& json::value::as_bool()
{
	if (std::holds_alternative<bool>(m_value))
		return std::get<bool>(m_value);
	throw json::json_value_error("Value type is not boolean");
}

const bool& json::value::as_bool() const
{
	if (std::holds_alternative<bool>(m_value))
		return std::get<bool>(m_value);
	throw json::json_value_error("Value type is not boolean");
}

std::string json::value::to_string() const
{
	return json::to_string(*this);
}

json::value json::object(std::initializer_list<std::pair<const std::string, json::value>> init)
{
	using jobject = std::unordered_map<std::string, json::value>;
	return json::value(jobject(init)); 
}

json::value json::array(std::initializer_list<json::value> init) 
{
	using jarray = std::vector<json::value>;
	return json::value(jarray(init));
}