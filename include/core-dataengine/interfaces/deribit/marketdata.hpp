#pragma once 
#include <iostream>
#include "base.hpp"
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
            
            private: 
                std::string urlInstruments_; 
                std::string urlCombos_; 

        };
    
    };
}
