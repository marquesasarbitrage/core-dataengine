#pragma once 
#include <iostream>
#include <map>
#include "../../../include/core-dataengine/http.hpp"
#include "../../../include/core-dataengine/errors.hpp"

namespace DataInterfaces {

    class DataInterface {

        public:
            DataInterface();
            virtual ~DataInterface() = default;

            nlohmann::json getData() const;
            DateTime getTimestamp() const;
            std::exception_ptr getException() const;
            bool isSuccess() const; 
            bool isProcessed() const; 
            void run();

        protected: 
            virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) = 0;
            virtual void _run() = 0; 
            void processData(std::map<int, nlohmann::json> dataMap);
            void setException(std::exception_ptr value); 
            void setData(std::shared_ptr<nlohmann::json> value); 
            void setTimestamp(DateTime value);
        
        private: 
            std::shared_ptr<nlohmann::json> dataPtr_; 
            std::exception_ptr exceptionPtr_; 
            DateTime timestamp_; 

    };

};