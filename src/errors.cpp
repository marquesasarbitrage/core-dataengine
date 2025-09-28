#include <iostream>
#include "../include/cpp-dataengine/errors.hpp"

DataEngineErrors::UnknownBuildingNameError::UnknownBuildingNameError(): message_(getMessageAsString()){}
const char* DataEngineErrors::UnknownBuildingNameError::what() const noexcept {return message_.c_str();}
std::string DataEngineErrors::UnknownBuildingNameError::getMessageAsString() const{return "Cannot find the build name in building path.";}

DataEngineErrors::EnvironmentFileOpenError::EnvironmentFileOpenError(const std::string& pathFile): pathFile_(pathFile), message_(getMessageAsString()){}
const char* DataEngineErrors::EnvironmentFileOpenError::what() const noexcept {return message_.c_str();}
std::string DataEngineErrors::EnvironmentFileOpenError::getMessageAsString() const{return "Could not open " + pathFile_ + ".";}

DataEngineErrors::UnknwonEnvironmentKeyError::UnknwonEnvironmentKeyError(const std::string& key): key_(key), message_(getMessageAsString()){}
const char* DataEngineErrors::UnknwonEnvironmentKeyError::what() const noexcept {return message_.c_str();}
std::string DataEngineErrors::UnknwonEnvironmentKeyError::getMessageAsString() const{return "The variable for key: " + key_ + " could not be found in the .env file.";}