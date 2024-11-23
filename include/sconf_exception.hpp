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
 * @file sconf_exception.hpp
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file defining the SconfException
 *        class for handling errors in the sConf library.
 */
#ifndef SCONF_EXCEPTION_HPP
#define SCONF_EXCEPTION_HPP

#include <stdexcept>
#include <string>

/**
 * @class SconfException
 * @brief Custom exception class for the sConf library.
 *
 * The SconfException class extends the standard `std::runtime_error` class 
 * to provide meaningful error messages specific to the sConf configuration 
 * parsing and handling library. It can be used to signal errors encountered 
 * during operations such as file parsing, invalid value handling, or other 
 * unexpected conditions in the library.
 */
class SconfException : public std::runtime_error {
public:
    /**
     * @brief Constructs a SconfException with a custom error message.
     * @param message The error message describing the exception.
     *
     * Example usage:
     * @code
     * throw SconfException("Invalid configuration format.");
     * @endcode
     */
    explicit SconfException(std::string message) :
        std::runtime_error(message) {}
};

#endif
