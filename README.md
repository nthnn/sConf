# sConf Library

<img alt="Build Test CI" src="https://github.com/nthnn/sConf/actions/workflows/build_test.yml/badge.svg" />

sConf is a lightweight and flexible C++ library for parsing, managing, and manipulating structured configuration files. With support for multiple value types, comments, and error handling, sConf is designed to be intuitive and efficient for developers who need robust configuration management in their C++ projects.

- **Versatile Value Types**: Supports strings, integers, doubles, booleans, dates, and arrays.
- **Section-Based Structure**: Organize configuration data into sections for easy access and management.
- **Comments Handling**: Preserve and retrieve comments associated with configuration sections.
- **Error Handling**: Custom exception handling with detailed error messages.
- **File Operations**: Load configuration files and save changes back to disk with ease.

## Example Usage

```sconf
; Example Section
[example_section]
pi = 3.14

; Settings Section
[settings]
name = Nathanne Isip                ; My name
git  = https://github.com/nthnn     ; My GitHub link
```

---

[example/full_example.cpp](example/full_example.cpp):
```cpp
#include <iostream>
#include <sconf.hpp>

int main() {
    sConfParser parser;
    try {
        parser.load("example.sconf");
        
        std::cout << "Sections in the sConf file:\n";
        for(const auto& section : parser.getSections())
            std::cout << "  Section: " << section << "\n";

        std::cout << "\nSection Contents:\n";
        for(const auto& section : {"example_section", "settings"})
            if(parser.hasSection(section)) {
                std::cout << "  Section: " << section << "\n";
                auto mySection = parser.getSection(section);

                for(const auto& [key, value] : mySection) {
                    std::cout << "    " << key << " -> ";
                    if(value.isArray()) {
                        std::cout << "[";
                        const auto& arrayValues = value.getArray();

                        for(size_t i = 0; i < arrayValues.size(); ++i) {
                            std::cout << arrayValues[i].getString();
                            if(i < arrayValues.size() - 1)
                                std::cout << ", ";
                        }

                        std::cout << "]";
                    }
                    else std::cout << value.getString();

                    std::cout << "\n";
                }
            }

        std::cout << "\nComments:\n";
        for(const auto& section : {"example_section", "settings"})
            if(parser.hasSectionComment(section)) {
                std::cout << "  Comments for '" << section << "':\n";
                auto comments = parser.getSectionComment(section);

                for(const auto& comment : comments)
                    std::cout << "    " << comment << '\n';
            }

        if(parser.hasSection("example_section"))
            parser.removeSection("example_section");

        parser.save("output.sconf");
    }
    catch(const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }

    return 0;
}
```

## Contribution and Feedback

Contributions and feedback are all welcome to enhance this library. If you encounter any issues, have suggestions for improvements, or would like to contribute code, please do so.

## License

sConf is distributed under the GNU General Public License v3.0. For further details, refer to the LICENSE file.

```
This program is free software: you can redistribute it and/or modify  
it under the terms of the GNU General Public License as published by  
the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program. If not, see <http://www.gnu.org/licenses/>.
```
