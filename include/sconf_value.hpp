/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of sConf.
 * 
 * N8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * N8 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with N8. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file sconf_value.hpp
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file defining the sConfValue class
 *        for handling various types of configuration values.
 */
#ifndef SCONF_VALUE_HPP
#define SCONF_VALUE_HPP

#include <ctime>
#include <iomanip>
#include <string>
#include <vector>

/**
 * @class sConfValue
 * @brief Represents a single value in a structured configuration file.
 *
 * The sConfValue class supports multiple data types, including strings, integers, doubles, 
 * booleans, dates, and arrays. It provides methods for type-safe access and manipulation 
 * of the stored value, making it versatile for handling complex configurations.
 */
class sConfValue {
public:
    /**
     * @enum Type
     * @brief Enumerates the supported data types for a configuration value.
     */
    enum class Type {
        String,  ///< A string value.
        Integer, ///< An integer value.
        Double,  ///< A double-precision floating-point value.
        Boolean, ///< A boolean value.
        Date,    ///< A date and time value.
        Array    ///< An array of sConfValue objects.
    };

    /**
     * @brief Default constructor. Initializes the value as an empty string.
     */
    sConfValue() = default;

    /**
     * @brief Constructs a sConfValue with a string value.
     * @param val The string value to set.
     */
    explicit sConfValue(const std::string& val) : stringValue(val), type(Type::String) {}

    /**
     * @brief Constructs a sConfValue with an integer value.
     * @param val The integer value to set.
     */
    explicit sConfValue(int val) : stringValue(std::to_string(val)), type(Type::Integer) {}

    /**
     * @brief Constructs a sConfValue with a double value.
     * @param val The double value to set.
     */
    explicit sConfValue(double val) : stringValue(std::to_string(val)), type(Type::Double) {}

    /**
     * @brief Constructs a sConfValue with a boolean value.
     * @param val The boolean value to set. Stored as "true" or "false".
     */
    explicit sConfValue(bool val) : stringValue(val ? "true" : "false"), type(Type::Boolean) {}

    /**
     * @brief Constructs a sConfValue with an array of sConfValue objects.
     * @param val The vector of sConfValue objects to set.
     */
    explicit sConfValue(const std::vector<sConfValue>& val) : values(val), type(Type::Array) {}

    /**
     * @brief Constructs a sConfValue with a date value.
     * @param val The date and time value to set.
     */
    explicit sConfValue(const std::tm& val) : type(Type::Date) {
        char buffer[32];
        std::strftime(
            buffer,
            sizeof(buffer),
            "%Y-%m-%d %H:%M:%S",
            &val
        );

        stringValue = buffer;
    }

    /**
     * @brief Retrieves the data type of the value.
     * @return The type of the value as a sConfValue::Type enum.
     */
    Type getType() const;

    /**
     * @brief Checks if the value is an array.
     * @return `true` if the value is of type Array, `false` otherwise.
     */
    bool isArray() const;

    /**
     * @brief Retrieves the value as an integer.
     * @return The integer value.
     * @throws std::runtime_error If the value is not of type Integer.
     */
    int getInteger() const;

    /**
     * @brief Retrieves the value as a double.
     * @return The double value.
     * @throws std::runtime_error If the value is not of type Double.
     */
    double getDouble() const;

    /**
     * @brief Retrieves the value as a boolean.
     * @return The boolean value.
     * @throws std::runtime_error If the value is not of type Boolean.
     */
    bool getBoolean() const;

    /**
     * @brief Retrieves the value as a string.
     * @return The string value.
     * @throws std::runtime_error If the value is not of type String.
     */
    std::string getString() const;

    /**
     * @brief Retrieves the value as a date.
     * @return The date value as a `std::tm` object.
     * @throws std::runtime_error If the value is not of type Date.
     */
    std::tm getDate() const;

    /**
     * @brief Retrieves the value as an array of sConfValue objects.
     * @return A vector of sConfValue objects.
     * @throws std::runtime_error If the value is not of type Array.
     */
    std::vector<sConfValue> getArray() const;

    /**
     * @brief Sets the value as an integer.
     * @param value The integer to set.
     */
    void setInteger(int value);

    /**
     * @brief Sets the value as a double.
     * @param value The double to set.
     */
    void setDouble(double value);

    /**
     * @brief Sets the value as a boolean.
     * @param value The boolean to set.
     */
    void setBoolean(bool value);

    /**
     * @brief Sets the value as a string.
     * @param value The string to set.
     */
    void setString(const std::string& value);

    /**
     * @brief Sets the value as a date.
     * @param value The date to set.
     */
    void setDate(const std::tm& value);

    /**
     * @brief Sets the value as an array of sConfValue objects.
     * @param value The vector of sConfValue objects to set.
     */
    void setArray(const std::vector<sConfValue>& value);

private:
    /**
     * @brief Stores the value as a string, regardless of the type.
     */
    std::string stringValue;

    /**
     * @brief Stores the value as an array if the type is Array.
     */
    std::vector<sConfValue> values;

    /**
     * @brief The current type of the value.
     */
    Type type{Type::String};

    /**
     * @brief Checks if a string is numeric.
     * @param str The string to check.
     * @return `true` if the string represents a number, `false` otherwise.
     */
    static bool isNumber(const std::string& str);

    /**
     * @brief Checks if a string is an integral number.
     * @param str The string to check.
     * @return `true` if the string represents an integer, `false` otherwise.
     */
    static bool isIntegral(const std::string& str);

    /**
     * @brief Parses a string into a date value.
     * @param str The string representing a date.
     * @return The parsed date as a `std::tm` object.
     * @throws std::runtime_error If the string is not a valid date.
     */
    static std::tm parseDate(const std::string& str);
};

#endif
