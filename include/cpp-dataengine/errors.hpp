#pragma once 
#include <stdexcept>
#include <string>

namespace DataEngineErrors
{

    class UnknownBuildingNameError : public std::exception {
        public:
            explicit UnknownBuildingNameError();
            const char* what() const noexcept override;
            std::string getMessageAsString() const;
        private: 
            std::string message_;
    };

    class EnvironmentFileOpenError : public std::exception {
        public:
            explicit EnvironmentFileOpenError(const std::string& pathFile);
            const char* what() const noexcept override;
            std::string getMessageAsString() const;
        private: 
            std::string pathFile_;
            std::string message_;
    };

    class UnknwonEnvironmentKeyError : public std::exception {
        public:
            explicit UnknwonEnvironmentKeyError(const std::string& key);
            const char* what() const noexcept override;
            std::string getMessageAsString() const;
        private: 
            std::string key_;
            std::string message_;
    };
}