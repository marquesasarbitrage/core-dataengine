#include <iostream>
#include "../include/cpp-dataengine/core.hpp"

namespace DataEngineTool 
{
    std::string trim(const std::string& input)
    {
            size_t start = 0;
        while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start]))) {
            ++start;
        }
        if (input.empty()) return input.substr(start);
        size_t end = input.size() - 1;
        while (end != std::string::npos && std::isspace(static_cast<unsigned char>(input[end]))) {
            if (end == 0) return ""; // all spaces
            --end;
        }
        return input.substr(start).substr(0, end + 1);
    }

}