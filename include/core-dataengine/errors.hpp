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

namespace DataEngineErrorRegistry
{

    namespace Tools {

        class EmptyStringError final: public DataEngineLibraryError {protected: std::string getErrorMessage() const override; };
        class InvalidStringError final: public DataEngineLibraryError {protected: std::string getErrorMessage() const override; };
    }

    namespace DotEnv {

        class OpenFileError final: public DataEngineLibraryError {protected: std::string getErrorMessage() const override; };
        class UnknownKeyError final: public DataEngineLibraryError {protected: std::string getErrorMessage() const override; };
        class UnknownBuildName final: public DataEngineLibraryError {protected: std::string getErrorMessage() const override; };
    }

    namespace DataInterfaces {

        class JSONKeyError final : public DataEngineLibraryError {

            public: 
                JSONKeyError(std::string key); 
                ~JSONKeyError() = default; 

            protected: 
                std::string getErrorMessage() const override;
            
            private: 
                std::string key_;
        };

        class JSONArrayError final : public DataEngineLibraryError {

            public: 
                JSONArrayError(std::string key); 
                ~JSONArrayError() = default; 

            protected: 
                std::string getErrorMessage() const override;
            
            private: 
                std::string key_;
        };

        class JSONInvalidDataError final :  public DataEngineLibraryError {protected: std::string getErrorMessage() const override; };

        

    }
};


