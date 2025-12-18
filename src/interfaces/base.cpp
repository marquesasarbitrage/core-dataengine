#include "../../include/core-dataengine/interfaces/base.hpp"

namespace DataInterfaces {

    DataInterface::DataInterface(): dataPtr_(nullptr), exceptionPtr_(nullptr), timestamp_(DateTime()){}; 

    nlohmann::json DataInterface::getData() const { return isSuccess() ? *dataPtr_ : nlohmann::json::object(); }

    DateTime DataInterface::getTimestamp() const { return timestamp_; }

    std::exception_ptr DataInterface::getException() const { return exceptionPtr_; }

    void DataInterface::processData(std::map<int, nlohmann::json> dataMap) {

        try{
            dataPtr_ = _processData(dataMap);
            exceptionPtr_ = nullptr;
            timestamp_ = DateTime();
        } catch (const std::exception& e){
            
            dataPtr_ = nullptr;
            exceptionPtr_ = std::current_exception();
            timestamp_ = DateTime();
        }
    }

    void DataInterface::run() {

        if (isProcessed()) ; 
        else {
            try {

                _run(); 

            } catch (const std::exception& e) {

                dataPtr_ = nullptr;
                exceptionPtr_ = std::current_exception();
                timestamp_ = DateTime();

            }
        }
    }

    void DataInterface::setException(std::exception_ptr value) { exceptionPtr_ = value; }

    void DataInterface::setData(std::shared_ptr<nlohmann::json> value) { dataPtr_ = value; }

    void DataInterface::setTimestamp(DateTime value) { timestamp_ = value; }

    bool DataInterface::isSuccess() const { return getException()==nullptr ? true : false ; }

    bool DataInterface::isProcessed() const { return (getException()==nullptr && dataPtr_==nullptr) ? false : true ; }

}