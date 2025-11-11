#pragma once 
#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <map>
#include <optional>
#include <filesystem>
#include <sstream>
#include <fstream>
#include "errors.hpp"
#include <nlohmann/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

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

        void addBulkInsert(const nlohmann::json& data); 
        void addBulkDeleteFromId(const std::string& _id); 
        void addBulkDeleteAll(); 
        void confirmingBulkDeleteAll(bool value);

        void cleanBulk(); 
        void runBulk();
        void setBulkOrdered(bool value); 
        std::optional<mongocxx::result::bulk_write> getBulkResult() const;
        std::exception_ptr getBulkException() const;

        nlohmann::json getAll(); 
        nlohmann::json getFromId(std::string _id);
        nlohmann::json getFromFilter(bsoncxx::document::view_or_value filter); 
    
    private: 
        const std::shared_ptr<mongocxx::client> clientPtr_; 
        std::string databaseName_; 
        std::string collectionName_;

        mongocxx::bulk_write bulk_;
        bool bulkOrdered_;
        bool confirmedBulkDelete_;
        std::optional<mongocxx::result::bulk_write> bulkResult_;
        std::exception_ptr bulkExceptionPtr_;



        static nlohmann::json getFallBackJson(const std::exception& exception); 
        static nlohmann::json getSuccesfullJson(const nlohmann::json& result); 
        nlohmann::json getParsedBson(const bsoncxx::document::view& doc);

        
};
