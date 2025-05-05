#ifndef JSON_VALUE_HPP_
#define JSON_VALUE_HPP_

#include "json_exceptions.hpp"
#include <cstddef>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

namespace json
{
	class value
	{
	public:
		using jarray = std::vector<value>;
		using jobject = std::unordered_map<std::string, value>;

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
		value& operator=(value&& other) noexcept = default;

		value& operator[](const std::string& key); // only for arrays
		value& operator[](std::size_t i); // only for arrays
		const value& operator[](std::size_t i) const; // only for objects

		value& at(const std::string& key); // only for objects
		const value& at(const std::string& key) const; // only for objects
		value& at(std::size_t i); // only for arrays
		const value& at(std::size_t i) const; // only for arrays

		void remove(const std::string& key); // only for objects
		void remove(std::size_t i); // only for arrays

		bool is_number() const noexcept;
		bool is_object() const noexcept;
		bool is_array() const noexcept;
		bool is_string() const noexcept;
		bool is_bool() const noexcept;
		bool is_null() const noexcept;

		template <typename T>
		bool is() const noexcept;

		jarray& as_array();
		const jarray& as_array() const;

		jobject& as_object();
		const jobject& as_object() const;

		std::string& as_string();
		const std::string& as_string() const;

		double& as_number();
		const double& as_number() const;

		bool& as_bool();
		const bool& as_bool() const;

		template <typename T>
		T& as();

		template <typename T>
		const T& as() const;

		std::string to_string() const;

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

	value object(std::initializer_list<std::pair<const std::string, value>> init);
	value array(std::initializer_list<value> init);
}

template <typename T>
bool json::value::is() const noexcept { return std::holds_alternative<T>(m_value); }

template <typename T>
T& json::value::as()
{
	if (is<T>())
		return std::get<T>(m_value);
	throw json::json_value_error("Wrong value type");
}

template <typename T>
const T& json::value::as() const
{
	if (is<T>())
		return std::get<T>(m_value);
	throw json::json_value_error("Wrong value type");
}

#endif // JSON_VALUE_HPP_