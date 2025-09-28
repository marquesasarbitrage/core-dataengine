#include <iostream>
#include "../include/cpp-dataengine/core.hpp"

DataEngineEnvFileLoad::DataEngineEnvFileLoad(const std::string& buildName): buildName_(buildName){loadMap();}; 

void DataEngineEnvFileLoad::printKeyMap() const
{
    std::cout << "Contents of the map:\n";
    for (const auto& [key, value] : keyMap_) {  // C++17 structured binding
        std::cout << key << " = " << value << "\n";
    }
}

void DataEngineEnvFileLoad::loadMap()
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
    if (!buildNameFound) throw DataEngineErrors::UnknownBuildingNameError();

    std::string path; 
    for (std::string& s : result)
    {
        path += s; 
        path += "/";
    }
    path += ".env";
    std::ifstream file(path);
    std::string line;
    if (!file.is_open()) throw DataEngineErrors::EnvironmentFileOpenError(path);

    while (std::getline(file, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            keyMap_[DataEngineTool::trim(key)] = DataEngineTool::trim(value);
        }
    }

}

std::string DataEngineEnvFileLoad::getVariable(const std::string& key) const 
{
    std::cout << key << std::endl;
    auto it = keyMap_.find(key);
    if (it != keyMap_.end()) {
        return it->second;
    } else {
        throw DataEngineErrors::UnknwonEnvironmentKeyError(key);
    }
}


