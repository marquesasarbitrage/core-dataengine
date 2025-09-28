#include <iostream>
#include "../include/cpp-dataengine/core.hpp"

MongoService::MongoService(const std::shared_ptr<mongocxx::client>& clientPtr, const std::string& databaseName, const std::string& collectionName):
clientPtr_(clientPtr), databaseName_(databaseName), collectionName_(collectionName), 
bulk_(getCollection().create_bulk_write()), bulkOrdered_(true), confirmedBulkDelete_(false),bulkResult_(std::nullopt), bulkExceptionPtr_(nullptr){}; 

std::shared_ptr<mongocxx::client> MongoService::getClientPtr() const {return clientPtr_;}
mongocxx::database MongoService::getDatabase() const {return (*getClientPtr())[databaseName_];}
mongocxx::collection MongoService::getCollection() const {return (*getClientPtr())[databaseName_][collectionName_];}
void MongoService::setDatabaseName(const std::string& databaseName) {databaseName_ = databaseName;}
void MongoService::setCollectionName(const std::string& collectionName) {collectionName_ = collectionName;}
std::string MongoService::getDatabaseName() const {return databaseName_;}
std::string MongoService::getCollectionName() const {return collectionName_;} 
void MongoService::setBulkOrdered(bool value){bulkOrdered_ = value;}
std::optional<mongocxx::result::bulk_write> MongoService::getBulkResult() const{return bulkResult_;}
std::exception_ptr MongoService::getBulkException() const {return bulkExceptionPtr_;}

void MongoService::cleanBulk()
{
    bulk_ = getCollection().create_bulk_write();
    bulkResult_ = std::nullopt; 
    bulkExceptionPtr_ = nullptr;
}

void MongoService::runBulk()
{
    mongocxx::options::bulk_write opts;
    opts.ordered(bulkOrdered_);
    try{
        bulkResult_ = bulk_.execute();
    }catch (const std::exception& e) {
        bulkExceptionPtr_ = std::current_exception();
    }
}

nlohmann::json MongoService::getFallBackJson(const std::exception& exception)
{
    nlohmann::json response;
    response["result"] = nlohmann::json::object();  // empty object if parsing fails
    response["status"] = {
        {"success", "false"},
        {"error", exception.what()}
    };
    return response;
}

nlohmann::json MongoService::getSuccesfullJson(const nlohmann::json& result)
{
    nlohmann::json response;
    response["result"] = result;  // empty object if parsing fails
    response["status"] = {
        {"success", "true"},
        {"error", "none"}
    };
    return response;
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

void MongoService::addBulkInsert(const nlohmann::json& data)
{
    mongocxx::model::insert_one insert_op{bsoncxx::from_json(data.dump()).view()};
    bulk_.append(insert_op);
}

void MongoService::addBulkDeleteFromId(const std::string& _id)
{
    auto filter = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", _id));
    bulk_.append(mongocxx::model::delete_one(filter.view()));
}

void MongoService::addBulkDeleteAll()
{
    if (confirmedBulkDelete_){
        auto filter = bsoncxx::builder::basic::make_document();
        bulk_.append(mongocxx::model::delete_one(filter.view()));
    }
}

void MongoService::confirmingBulkDeleteAll(bool value){confirmedBulkDelete_ = value;}