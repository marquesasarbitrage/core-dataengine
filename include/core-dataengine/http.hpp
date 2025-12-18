#pragma once 
#include <iostream>
#include "errors.hpp"
#include "core-datetime/datetime.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h> 

namespace py = pybind11;

class HTTPRequest
{
    public:
        HTTPRequest(const std::string& url);
        virtual ~HTTPRequest(){};

        std::string getUrl() const;
        std::shared_ptr<nlohmann::json> getData() const;
        std::exception_ptr getException() const;
        DateTime getTimestamp() const;
        bool isSuccess() const; 
        virtual void run();
        
    protected: 
        void setException(const std::exception_ptr& exceptionPtr); 
        void setData(const nlohmann::json& data);
        void setTimestamp();

    private:
        std::string url_; 
        std::shared_ptr<nlohmann::json> dataPtr_;
        std::exception_ptr exceptionPtr_;
        DateTime timestamp_;
        static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);

};

class YahooFinanceRequest final: public HTTPRequest
{
    public: 
        YahooFinanceRequest(const std::string& url);
        ~YahooFinanceRequest(){};

        virtual void run() override; 
        
    private: 
        std::string getPythonVirtualEnvPath();
}; 

