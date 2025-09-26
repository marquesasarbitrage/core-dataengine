#pragma once 
#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

class HTTPRequest
{
    public:
        HTTPRequest(const std::string& url);
        virtual ~HTTPRequest(){};

        std::string getUrl() const;
        void setUrl(const std::string& url);
        virtual void run();
        nlohmann::json getData() const;
        std::exception_ptr getException() const;
        
    
    protected: 
        void setException(const std::exception_ptr& exceptionPtr); 
        void setData(const nlohmann::json& data);

    private:
        std::string url_; 
        nlohmann::json data_;
        std::exception_ptr exceptionPtr_;
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

class MongoService
{
    public: 
        MongoService(const std::shared_ptr<mongocxx::client>& clientPtr, const std::string& databaseName, const std::string& collectionName);
        virtual ~MongoService(){};

        std::shared_ptr<mongocxx::client> getClientPtr() const;
        mongocxx::database getDatabase() const; 
        mongocxx::collection getCollection() const; 
        void setDatabaseName(const std::string& databaseName); 
        void setCollectionName(const std::string& collectionName); 
        std::string getDatabaseName() const; 
        std::string getCollectionName() const; 

        nlohmann::json getAll(); 
        nlohmann::json getFromId(std::string _id);
        nlohmann::json getFromFilter(bsoncxx::document::view_or_value filter); 
    
    private: 
        const std::shared_ptr<mongocxx::client> clientPtr_; 
        std::string databaseName_; 
        std::string collectionName_;

        static nlohmann::json getFallBackJson(const std::exception& exception); 
        static nlohmann::json getSuccesfullJson(const nlohmann::json& result); 
        nlohmann::json getParsedBson(const bsoncxx::document::view& doc);
        
};




