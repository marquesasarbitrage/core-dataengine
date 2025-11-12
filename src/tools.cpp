#include <iostream>
#include "../include/core-dataengine/tools.hpp"

namespace DataEngineTools
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

    DotEnv::DotEnv(const std::string& buildName): buildName_(buildName){loadMap();}; 

    void DotEnv::printKeyMap() const
    {
        std::cout << "Contents of the map:\n";
        for (const auto& [key, value] : keyMap_) {  // C++17 structured binding
            std::cout << key << " = " << value << "\n";
        }
    }

    void DotEnv::loadMap()
    {
        std::vector<std::string> result;
        std::filesystem::path cwd = std::filesystem::current_path();
        bool buildNameFound = false;
        for (const auto& part : cwd) {
            std::string segment = part.string();
            if (segment != buildName_)
            {
                result.push_back(segment);
                
            } else
            {
                buildNameFound = true;
                break; 
            }
            
        }
        if (!buildNameFound) throw DateEngineErrorRegistry::DotEnv::UnknownBuildName();

        std::string path; 
        for (std::string& s : result)
        {
            path += s; 
            path += "/";
        }
        path += ".env";
        std::ifstream file(path);
        std::string line;
        if (!file.is_open()) throw DateEngineErrorRegistry::DotEnv::OpenFileError();

        while (std::getline(file, line)) {
            // Skip empty lines or comments
            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::istringstream iss(line);
            std::string key, value;

            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                keyMap_[DataEngineTools::trim(key)] = DataEngineTools::trim(value);
            }
        }

    }

    std::string DotEnv::getVariable(const std::string& key) const 
    {
        auto it = keyMap_.find(key);
        if (it != keyMap_.end()) {
            return it->second;
        } else {
            throw DateEngineErrorRegistry::DotEnv::UnknownKeyError();
        }
    }





}