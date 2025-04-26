#ifndef JSON_VALUE_HPP_
#define JSON_VALUE_HPP_

#include <cstddef>
#include <string>
#include <variant>
#include <optional>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace json
{
	class value
	{
	private:
		using jarray = std::vector<value>;
		using jobject = std::unordered_map<std::string, value>;
	public:
		value() noexcept : m_value(nullptr) {}
		value(std::nullptr_t) noexcept : m_value(nullptr) {}

		value(const jobject& val) : m_value(val) {}
		value(jobject&& val) noexcept : m_value(std::move(val)) {}

		value(const jarray& val) : m_value(val) {}
		value(jarray&& val) noexcept : m_value(std::move(val)) {}

		value(const std::string& val) : m_value(val) {}
		value(std::string&& val) noexcept : m_value(std::move(val)) {}
		value(const char *val) : m_value(val) {}

		value(double val) : m_value(val) {}
		value(int val) : m_value(static_cast<double>(val)) {}
		value(bool val) : m_value(val) {}

		//Rule of Five
		~value() = default;
		value(const value& other) = default;
		value(value&& other) = default;
		value& operator=(const value& other) = default;
		value& operator=(value&& other) = default;

		value& operator[](const std::string& key);
		value& operator[](std::size_t i);
		const value& operator[](std::size_t i) const;

		value& at(const std::string& key);
		const value& at(const std::string& key) const;
		value& at(std::size_t i);
		const value& at(std::size_t i) const;

		bool is_number() const noexcept;
		bool is_object() const noexcept;
		bool is_array() const noexcept;
		bool is_string() const noexcept;
		bool is_bool() const noexcept;
		bool is_null() const noexcept;

		jarray& as_array();
		const jarray& as_array() const;
		jobject& as_object();
		const jobject& as_object() const;
		std::string& as_string();
		const std::string& as_string() const;
		double as_number() const;
		bool as_bool() const;

	private:
		std::variant<
			jobject,
			jarray,
			std::string,
			double,
			bool,
			std::nullptr_t
		> m_value;
	};

	class json_value_error : std::runtime_error
	{
	public:
		explicit json_value_error(const std::string& what_arg) : std::runtime_error(what_arg) {}
		explicit json_value_error(const char *what_arg) : std::runtime_error(what_arg) {}
	};

	value object(std::initializer_list<std::pair<const std::string, value>> init);
	value array(std::initializer_list<value> init);

	std::string to_string(const value& val);
}

#endif // JSON_VALUE_HPP_