#include <iostream>
#include "../include/core-dataengine/errors.hpp"

const char* DataEngineLibraryError::what() const noexcept
{
    if (cachedMessage_.empty()) {
            cachedMessage_ = getErrorMessage();  
        }
    return cachedMessage_.c_str();
}

namespace DateEngineErrorRegistry
{
    namespace DotEnv 
    {
       std::string OpenFileError::getErrorMessage() const {return "The .env file could not be open. Please make sure it exists.";}
       std::string UnknownKeyError::getErrorMessage() const {return "A key could was not found in the .env file.";}
       std::string UnknownBuildName::getErrorMessage() const {return "Cannot find the build name in building path.";}
    }
};

