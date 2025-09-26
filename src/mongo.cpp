#include <iostream>
#include "../include/cpp-marketdata/core.hpp"

MongoService::MongoService(const std::shared_ptr<mongocxx::client>& clientPtr, const std::string& databaseName, const std::string& collectionName):
clientPtr_(clientPtr), databaseName_(databaseName), collectionName_(collectionName){}; 

std::shared_ptr<mongocxx::client> MongoService::getClientPtr() const {return clientPtr_;}
mongocxx::database MongoService::getDatabase() const {return (*getClientPtr())[databaseName_];}
mongocxx::collection MongoService::getCollection() const {return (*getClientPtr())[databaseName_][collectionName_];}
void MongoService::setDatabaseName(const std::string& databaseName) {databaseName_ = databaseName;}
void MongoService::setCollectionName(const std::string& collectionName) {collectionName_ = collectionName;}
std::string MongoService::getDatabaseName() const {return databaseName_;}
std::string MongoService::getCollectionName() const {return collectionName_;} 

nlohmann::json MongoService::getFallBackJson(const std::exception& exception)
{
    nlohmann::json response;
    response["result"] = nlohmann::json::object();  // empty object if parsing fails
    response["status"] = {
        {"success", "false"},
        {"error", exception.what()}
    };
}

nlohmann::json MongoService::getSuccesfullJson(const nlohmann::json& result)
{
    nlohmann::json response;
    response["result"] = result;  // empty object if parsing fails
    response["status"] = {
        {"success", "true"},
        {"error", "none"}
    };
}

nlohmann::json MongoService::getParsedBson(const bsoncxx::document::view& doc) {
    try {
        nlohmann::json parsed = nlohmann::json::parse(bsoncxx::to_json(doc));
        return getSuccesfullJson(parsed);
        
    } catch (const std::exception& e) {
        return getFallBackJson(e);
    }
}

nlohmann::json MongoService::getAll() 
{
    try {
        nlohmann::json results = nlohmann::json::array();
        auto cursor = getCollection().find({});
        for (auto&& doc : cursor) {
            results.push_back(getParsedBson(doc));
        }
        return getSuccesfullJson(results);
    } catch (const std::exception& e) {
        return getFallBackJson(e);
    }
}

nlohmann::json MongoService::getFromFilter(bsoncxx::document::view_or_value filter) {
    try {
        nlohmann::json results = nlohmann::json::array();
        auto cursor = getCollection().find(filter);
        for (auto&& doc : cursor) {
            results.push_back(getParsedBson(doc));
        }
        return getSuccesfullJson(results);
    } catch (const std::exception& e) {
        return getFallBackJson(e);
    }
}

nlohmann::json MongoService::getFromId(std::string _id)
{
    try{
        auto filter = bsoncxx::builder::stream::document{} 
                            << "_id" << _id 
                            << bsoncxx::builder::stream::finalize;
        return getFromFilter(filter.view()); 
    }catch (const std::exception& e) {
        return getFallBackJson(e);
    }
    
    
}