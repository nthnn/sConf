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

#include <algorithm>
#include <fstream>
#include <sconf_exception.hpp>
#include <sconf_parser.hpp>
#include <sstream>
#include <stdexcept>

std::string sConfParser::trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](char c) { 
        return std::isspace(c); 
    });
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](char c) { 
        return std::isspace(c); 
    }).base();

    return (start < end) ? std::string(start, end) : "";
}

std::string sConfParser::trimQuotes(const std::string& str) {
    if(str.size() >= 2 && str.front() == '"' && str.back() == '"')
        return str.substr(1, str.size() - 2);
    return str;
}

bool sConfParser::isArray(const std::string& value) {
    return !value.empty() && value.front() == '[' && value.back() == ']';
}

std::vector<sConfValue> sConfParser::parseArray(const std::string& value) {
    std::vector<sConfValue> result;
    std::string inner = value.substr(1, value.size() - 2);
    std::istringstream stream(inner);
    std::string item;

    while(std::getline(stream, item, ',')) {
        std::string trimmed = trimQuotes(trim(item));

        if(isArray(trimmed))
            result.emplace_back(parseArray(trimmed));
        else result.emplace_back(sConfValue(trimmed));
    }

    return result;
}

void sConfParser::parseLine(
    const std::string& line,
    std::string& currentSection,
    std::vector<std::string>& commentBuffer
) {
    std::string trimmed = trim(line);
    if(trimmed.empty())
        return;

    if(trimmed[0] == ';') {
        commentBuffer.push_back(trim(trimmed.substr(1)));
        return;
    }

    if(trimmed[0] == '[' && trimmed.back() == ']') {
        currentSection = trimQuotes(trim(trimmed.substr(1, trimmed.size() - 2)));
        if(this->comments.find(currentSection) == this->comments.end())
            this->comments[currentSection] = {};

        this->comments[currentSection].insert(
            this->comments[currentSection].end(),
            commentBuffer.begin(),
            commentBuffer.end()
        );
    }
    else {
        size_t eqPos = trimmed.find('=');
        if(eqPos == std::string::npos)
            throw SconfException("Invalid key-value pair: " + line);

        std::string key = trimQuotes(trim(trimmed.substr(0, eqPos)));
        std::string value = trim(trimmed.substr(eqPos + 1));

        size_t commentPos = value.find(';');
        if(commentPos != std::string::npos)
            value = trimQuotes(trim(value.substr(0, commentPos)));

        sConfValue sValue;
        if(this->isArray(value))
            sValue = sConfValue(parseArray(value));
        else sValue = sConfValue(trimQuotes(value));

        this->data[currentSection][key] = sValue;
    }

    commentBuffer.clear();
}

void sConfParser::saveValue(std::ofstream& file, const sConfValue& value) {
    switch(value.getType()) {
        case sConfValue::Type::Array:
            file << "[";
            for(size_t i = 0; i < value.getArray().size(); ++i) {
                saveValue(file, value.getArray()[i]);
                if(i < value.getArray().size() - 1)
                    file << ", ";
            }
            file << "]";
            break;

        case sConfValue::Type::String:
            file << value.getString();
            break;

        case sConfValue::Type::Integer:
            file << value.getInteger();
            break;

        case sConfValue::Type::Double:
            file << value.getDouble();
            break;

        case sConfValue::Type::Boolean:
            file << (value.getBoolean() ? "true" : "false");
            break;

        default:
            throw SconfException("Unsupported value type");
    }
}

void sConfParser::load(const std::string& filename) {
    std::ifstream file(filename);
    if(!file)
        throw SconfException("Failed to open file: " + filename);

    std::string line;
    std::string currentSection;
    std::vector<std::string> commentBuffer;

    while(std::getline(file, line))
        this->parseLine(line, currentSection, commentBuffer);
}

void sConfParser::save(const std::string& filename) const {
    std::ofstream file(filename);
    if(!file)
        throw SconfException("Failed to open file for writing: " + filename);

    for(const auto& [section, keys] : this->data) {
        if(this->comments.find(section) != this->comments.end())
            for(const auto& comment : this->comments.at(section))
                file << "; " << comment << '\n';

        file << "[" << section << "]\n";
        for(const auto& [key, value] : keys) {
            file << key << " = ";
            saveValue(file, value);
            file << "\n";
        }
    }
}

std::vector<std::string> sConfParser::getSections() const {
    std::vector<std::string> sections;
    sections.reserve(this->data.size());

    for(const auto& [section, _] : this->data)
        sections.push_back(section);
    return sections;
}

std::unordered_map<std::string, sConfValue> sConfParser::getSection(
    const std::string& section
) const {
    auto it = this->data.find(trimQuotes(trim(section)));
    if(it != this->data.end())
        return it->second;

    throw SconfException("Section not found: " + section);
}

void sConfParser::addSection(const std::string& section) {
    std::string sectionName = trimQuotes(trim(section));
    if(this->data.find(sectionName) == this->data.end())
        this->data[sectionName] = {};
}

void sConfParser::setKey(
    const std::string& section,
    const std::string& key,
    const sConfValue& value
) {
    std::string sectionName = trimQuotes(trim(section)),
        keyName = trimQuotes(trim(key));

    if(this->data.find(sectionName) == this->data.end())
        throw SconfException("Section not found: " + section);
    this->data[sectionName][keyName] = value;
}

void sConfParser::removeSection(const std::string& section) {
    std::string sectionName = trimQuotes(section);
    if(this->data.erase(sectionName) == 0)
        throw SconfException("Section not found: " + section);

    this->comments.erase(sectionName);
}

bool sConfParser::hasSection(const std::string& section) const {
    return this->data.find(trimQuotes(trim(section))) != this->data.end();
}

std::unordered_map<std::string, sConfValue> sConfParser::getSectionKeyPair(
    const std::string& section
) const {
    auto it = this->data.find(trimQuotes(trim(section)));
    if(it != this->data.end())
        return it->second;

    throw SconfException("Section not found: " + section);
}

bool sConfParser::hasSectionPairByKey(
    const std::string& section,
    const std::string& key
) const {
    auto sectionIt = this->data.find(trimQuotes(trim(section)));
    if(sectionIt == this->data.end())
        return false;

    return sectionIt->second.find(trimQuotes(trim(key))) != sectionIt->second.end();
}

void sConfParser::removeSectionPairByKey(
    const std::string& section,
    const std::string& key
) {
    std::string keyName = trimQuotes(trim(key));

    auto sectionIt = this->data.find(trimQuotes(trim(section)));
    if(sectionIt == this->data.end() || sectionIt->second.erase(keyName) == 0)
        throw SconfException("Key not found in section: " + keyName);
}

bool sConfParser::isSectionPairArray(
    const std::string& section,
    const std::string& key
) const {
    std::string sectionName = trimQuotes(trim(section)),
        keyName = trimQuotes(trim(key));

    auto sectionIt = this->data.find(sectionName);
    if(sectionIt == this->data.end())
        throw SconfException("Section not found: " + sectionName);

    auto keyIt = sectionIt->second.find(keyName);
    if(keyIt == sectionIt->second.end())
        throw SconfException("Key not found: " + keyName);

    return keyIt->second.isArray();
}

bool sConfParser::isSectionPairSingleString(
    const std::string& section,
    const std::string& key
) const {
    std::string sectionName = trimQuotes(trim(section)),
        keyName = trimQuotes(trim(key));

    auto sectionIt = this->data.find(sectionName);
    if(sectionIt == this->data.end())
        throw SconfException("Section not found: " + sectionName);

    auto keyIt = sectionIt->second.find(keyName);
    if(keyIt == sectionIt->second.end())
        throw SconfException("Key not found: " + keyName);

    return !keyIt->second.isArray();
}

std::vector<std::string> sConfParser::getSectionComment(
    const std::string& section
) const {
    std::string sectionName = trimQuotes(trim(section));

    auto it = this->comments.find(sectionName);
    if(it != this->comments.end())
        return it->second;

    throw SconfException("Section not found: " + sectionName);
}

bool sConfParser::hasSectionComment(const std::string& section) const {
    auto it = this->comments.find(trimQuotes(trim(section)));
    return it != this->comments.end() && !it->second.empty();
}

void sConfParser::removeSectionComment(const std::string& section) {
    auto it = this->comments.find(trimQuotes(trim(section)));
    if(it != this->comments.end())
        it->second.clear();
}
