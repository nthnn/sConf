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
