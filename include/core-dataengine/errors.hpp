#pragma once 
#include <stdexcept>
#include <string>

class DataEngineLibraryError: public std::exception 
{
    public:
        const char* what() const noexcept override;
        explicit DataEngineLibraryError(){};
        virtual ~DataEngineLibraryError() = default;
    protected: 
        virtual std::string getErrorMessage() const = 0; 
    private:
        mutable std::string cachedMessage_;  // must be mutable to modify in const what()
};

namespace DateEngineErrorRegistry
{
    namespace DotEnv 
    {
        class OpenFileError final: public DataEngineLibraryError {protected: std::string getErrorMessage() const override; };
        class UnknownKeyError final: public DataEngineLibraryError {protected: std::string getErrorMessage() const override; };
        class UnknownBuildName final: public DataEngineLibraryError {protected: std::string getErrorMessage() const override; };
    }
};


