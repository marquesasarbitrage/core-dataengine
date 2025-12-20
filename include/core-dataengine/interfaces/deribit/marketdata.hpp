#pragma once 
#include <iostream>
#include "../../../include/core-dataengine/interfaces/base.hpp"
#include "../../../include/core-dataengine/tools.hpp"
#include "core-datetime/tools.hpp"

namespace DataInterfaces {

    namespace DeribitMarketData {

        class DeribitMarketDataInterface : public DataInterfaces::DataInterface {

            public: 
                DeribitMarketDataInterface(); 
                virtual ~DeribitMarketDataInterface() = default;

            protected: 
                std::string getBaseURL() const;
                virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) override = 0;
                virtual void _run() override = 0;
        
        };

        class Instruments : public DeribitMarketDataInterface {

            public: 
                Instruments(); 
                Instruments(nlohmann::json instrumentsData, nlohmann::json comboData); 
                ~Instruments() = default; 
            
            protected: 
                virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) override;
                virtual void _run() override;
            

        };

        class SummmaryOrderBook : public DeribitMarketDataInterface {

            public: 
                SummmaryOrderBook(); 
                SummmaryOrderBook(std::map<int, nlohmann::json> summaryOrderbookMap);
                ~SummmaryOrderBook() = default; 

            protected: 
                virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) override;
                virtual void _run() override;
        };

        class OrderBook : public DeribitMarketDataInterface {

            public: 
                OrderBook(std::string instrumentName); 
                OrderBook(std::string instrumentName, int depth); 
                OrderBook(nlohmann::json orderBookData);
                ~OrderBook() = default;
                
                std::string getInstrumentName() const; 
            
            protected: 
                virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) override;
                virtual void _run() override;
            
            private: 
                int depth_;
                std::string instrumentName_;
        };
    
    };
}
