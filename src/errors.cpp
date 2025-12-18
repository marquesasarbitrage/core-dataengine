#include <iostream>
#include "../include/core-dataengine/errors.hpp"

const char* DataEngineLibraryError::what() const noexcept
{
    if (cachedMessage_.empty()) {
            cachedMessage_ = getErrorMessage();  
        }
    return cachedMessage_.c_str();
}

namespace DataEngineErrorRegistry
{

    namespace Tools {

        std::string EmptyStringError::getErrorMessage() const {return "The string given for conversion is empty";}
        std::string InvalidStringError::getErrorMessage() const {return "The string given for conversion is invalid.";}
    }

    namespace DotEnv 
    {
        std::string OpenFileError::getErrorMessage() const {return "The .env file could not be open. Please make sure it exists.";}
        std::string UnknownKeyError::getErrorMessage() const {return "A key could was not found in the .env file.";}
        std::string UnknownBuildName::getErrorMessage() const {return "Cannot find the build name in building path.";}
    }

    namespace DataInterfaces {

        JSONKeyError::JSONKeyError(std::string key): key_(key){};
        std::string JSONKeyError::getErrorMessage() const {return "The key " + key_ + " could not be found in the JSON file.";}

        JSONArrayError::JSONArrayError(std::string key): key_(key){};
        std::string JSONArrayError::getErrorMessage() const {return "The key " + key_ + " from the JSON file should return an array";}

        std::string JSONInvalidDataError::getErrorMessage() const {return "There is invalid data found in the JSON file.";}
    }
};

