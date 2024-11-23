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

#include <regex>
#include <sconf_exception.hpp>
#include <sconf_value.hpp>
#include <string>

sConfValue::Type sConfValue::getType() const {
    return this->type;
}

bool sConfValue::isArray() const {
    return this->type == Type::Array;
}

int sConfValue::getInteger() const {
    if(this->type != Type::Integer)
        throw SconfException("Value is not an integer");

    return std::stoi(this->stringValue);
}

double sConfValue::getDouble() const {
    if(this->type != Type::Double)
        throw SconfException("Value is not a double");

    return std::stod(this->stringValue);
}

bool sConfValue::getBoolean() const {
    if(this->type != Type::Boolean)
        throw SconfException("Value is not a boolean");

    return this->stringValue == "true";
}

std::string sConfValue::getString() const {
    if(this->type != Type::String)
        throw SconfException("Value is not a string");

    return this->stringValue;
}

std::tm sConfValue::getDate() const {
    if(this->type != Type::Date)
        throw SconfException("Value is not a date");

    return parseDate(this->stringValue);
}

std::vector<sConfValue> sConfValue::getArray() const {
    if(this->type != Type::Array)
        throw SconfException("Value is not an array");

    return this->values;
}

void sConfValue::setInteger(int value) {
    this->type = Type::Integer;
    this->stringValue = std::to_string(value);

    this->values.clear();
}

void sConfValue::setDouble(double value) {
    this->type = Type::Double;
    this->stringValue = std::to_string(value);

    this->values.clear();
}

void sConfValue::setBoolean(bool value) {
    this->type = Type::Boolean;
    this->stringValue = value ? "true" : "false";

    this->values.clear();
}

void sConfValue::setString(const std::string& value) {
    this->type = Type::String;
    this->stringValue = value;

    this->values.clear();
}

void sConfValue::setDate(const std::tm& value) {
    this->type = Type::Date;

    char buffer[32];
    std::strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d %H:%M:%S",
        &value
    );

    this->stringValue = buffer;
    this->values.clear();
}

void sConfValue::setArray(const std::vector<sConfValue>& value) {
    this->type = Type::Array;
    this->values = value;

    this->stringValue.clear();
}

bool sConfValue::isNumber(const std::string& str) {
    return std::regex_match(str, std::regex("^-?\\d+(\\.\\d+)?$"));
}

bool sConfValue::isIntegral(const std::string& str) {
    return std::regex_match(str, std::regex("^-?\\d+$"));
}

std::tm sConfValue::parseDate(const std::string& str) {
    std::tm tm{};
    std::istringstream ss(str);
    
    if(str.length() > 10)
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    else {
        ss >> std::get_time(&tm, "%Y-%m-%d");

        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
    }

    if(ss.fail())
        throw SconfException("Invalid date format. Expected yyyy-MM-dd [hh:mm:ss]");

    return tm;
}
