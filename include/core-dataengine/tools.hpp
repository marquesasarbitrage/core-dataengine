#pragma once 
#include <iostream>
#include <map>
#include <charconv>
#include <filesystem>
#include <sstream>
#include <fstream>
#include "errors.hpp"

namespace DataEngineTools
{
    std::string trim(const std::string& input); 

    double stringToDouble(const std::string& input);

    int stringToInt(const std::string& input);

    class DotEnv
    {
        public:
            DotEnv(const std::string& buildName);
            ~DotEnv() = default; 

            std::string getVariable(const std::string& key) const; 
            void printKeyMap() const; 
        
        private: 
            std::string buildName_;
            std::map<std::string, std::string> keyMap_;
            void loadMap(); 
            
    };

    

};
