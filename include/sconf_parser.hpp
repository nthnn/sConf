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
 * @file sconf_parser.hpp
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for the sConfParser class,
 *        responsible for parsing and managing
 *        structured configuration data.
 */
#ifndef SCONF_PARSER_HPP
#define SCONF_PARSER_HPP

#include <sconf_value.hpp>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class sConfParser
 * @brief A parser and manager for structured configuration (sConf) files.
 *
 * The sConfParser class provides functionality to load, parse, modify, 
 * and save structured configuration files. It supports sections, key-value 
 * pairs, arrays, and comments associated with sections.
 */
class sConfParser {
private:
    /**
     * @brief Storage for configuration data.
     *
     * Organized as a map where:
     * - The key is the section name.
     * - The value is another map of key-value pairs for that section.
     */
    std::unordered_map<std::string, std::unordered_map<std::string, sConfValue>> data;

    /**
     * @brief Storage for section comments.
     *
     * Maps each section name to its associated vector of comments.
     */
    std::unordered_map<std::string, std::vector<std::string>> comments;

    /**
     * @brief Removes leading and trailing whitespace from a string.
     * @param str The string to be trimmed.
     * @return A new string with leading and trailing whitespace removed.
     */
    static std::string trim(const std::string& str);    

    /**
     * @brief Removes enclosing quotes from a string, if present.
     * @param str The string to be processed.
     * @return A new string without enclosing quotes.
     */
    static std::string trimQuotes(const std::string& str);

    /**
     * @brief Formats the parameter to yyyy-mm-dd HH:MM:SS
     * @param time The std::tm variable to be converted.
     * @return String-formatted date to yyyy-mm-dd HH:MM:SS.
     */
    static std::string formatDateTime(const std::tm& time);

    /**
     * @brief Checks if a value represents an array.
     * @param value The string value to check.
     * @return `true` if the value represents an array, `false` otherwise.
     */
    static bool isArray(const std::string& value);

    /**
     * @brief Writes a single configuration value to an output file stream.
     * @param file The output file stream.
     * @param value The configuration value to save.
     */
    static void saveValue(std::ofstream& file, const sConfValue& value);

    /**
     * @brief Parses a string representing an array into individual sConfValues.
     * @param value The string representing the array.
     * @return A vector of sConfValue objects parsed from the array.
     */
    static std::vector<sConfValue> parseArray(const std::string& value);

    /**
     * @brief Parses a single line of the configuration file.
     * @param line The line to be parsed.
     * @param currentSection The current section being processed.
     * @param commentBuffer A buffer for storing comments until associated with a section.
     */
    void parseLine(const std::string& line, std::string& currentSection, std::vector<std::string>& commentBuffer);

public:
    /**
     * @brief Default sConfParser class constructor.
     */
    sConfParser() :
        data({}),
        comments({}) {}

    /**
     * @brief Loads a configuration file.
     * @param filename The path to the file to load.
     * @throws std::runtime_error If the file cannot be opened or parsed.
     */
    void load(const std::string& filename);

    /**
     * @brief Saves the current configuration to a file.
     * @param filename The path to the file to save.
     * @throws std::runtime_error If the file cannot be written.
     */
    void save(const std::string& filename) const;

    /**
     * @brief Retrieves the names of all sections in the configuration.
     * @return A vector of section names.
     */
    std::vector<std::string> getSections() const;

    /**
     * @brief Retrieves all key-value pairs for a specific section.
     * @param section The name of the section.
     * @return A map of key-value pairs in the section.
     * @throws std::runtime_error If the section does not exist.
     */
    std::unordered_map<std::string, sConfValue> getSection(const std::string& section) const;

    /**
     * @brief Adds a new section to the configuration.
     * @param section The name of the section to add.
     * @throws std::runtime_error If the section already exists.
     */
    void addSection(const std::string& section);

    /**
     * @brief Checks if a section exists.
     * @param section The name of the section to check.
     * @return `true` if the section exists, `false` otherwise.
     */
    bool hasSection(const std::string& section) const;

    /**
     * @brief Removes a section from the configuration.
     * @param section The name of the section to remove.
     * @throws std::runtime_error If the section does not exist.
     */
    void removeSection(const std::string& section);

    /**
     * @brief Sets a key-value pair in a section.
     * @param section The name of the section.
     * @param key The key to set.
     * @param value The value to associate with the key.
     * @throws std::runtime_error If the section does not exist.
     */
    void setKey(const std::string& section, const std::string& key, const sConfValue& value);

    /**
     * @brief Removes a key-value pair from a section.
     * @param section The name of the section.
     * @param key The key to remove.
     * @throws std::runtime_error If the section or key does not exist.
     */
    void removeSectionPairByKey(const std::string& section, const std::string& key);

    /**
     * @brief Checks if a key exists in a section.
     * @param section The name of the section.
     * @param key The key to check.
     * @return `true` if the key exists in the section, `false` otherwise.
     */
    bool hasSectionPairByKey(const std::string& section, const std::string& key) const;

    /**
     * @brief Retrieves all key-value pairs for a specific section.
     * @param section The name of the section.
     * @return A map of key-value pairs in the section.
     */
    std::unordered_map<std::string, sConfValue> getSectionKeyPair(const std::string& section) const;

    /**
     * @brief Checks if a key's value in a section is an array.
     * @param section The name of the section.
     * @param key The key to check.
     * @return `true` if the value is an array, `false` otherwise.
     */
    bool isSectionPairArray(const std::string& section, const std::string& key) const;

    /**
     * @brief Checks if a key's value in a section is a single string.
     * @param section The name of the section.
     * @param key The key to check.
     * @return `true` if the value is a single string, `false` otherwise.
     */
    bool isSectionPairSingleString(const std::string& section, const std::string& key) const;

    /**
     * @brief Retrieves the comments associated with a section.
     * @param section The name of the section.
     * @return A vector of comments for the section.
     */
    std::vector<std::string> getSectionComment(const std::string& section) const;

    /**
     * @brief Checks if a section has associated comments.
     * @param section The name of the section.
     * @return `true` if comments exist for the section, `false` otherwise.
     */
    bool hasSectionComment(const std::string& section) const;

    /**
     * @brief Removes all comments associated with a section.
     * @param section The name of the section.
     * @throws std::runtime_error If the section does not exist.
     */
    void removeSectionComment(const std::string& section);
};

#endif
