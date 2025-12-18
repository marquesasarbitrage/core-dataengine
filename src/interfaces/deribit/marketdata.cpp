#include "../../../include/core-dataengine/interfaces/deribit/marketdata.hpp"

namespace DataInterfaces {

    namespace DeribitMarketData {

        DeribitMarketDataInterface::DeribitMarketDataInterface(){};

        std::string DeribitMarketDataInterface::getBaseURL() const { return "https://deribit.com/api/v2/public/"; }

        Instruments::Instruments(): 
        urlInstruments_(getBaseURL()+"get_instruments?currency=any")
        , urlCombos_(getBaseURL()+"get_combos?currency=any")
        {};

        Instruments::Instruments(nlohmann::json instrumentsData, nlohmann::json comboData): urlInstruments_(""), urlCombos_("") {

            std::map<int, nlohmann::json> dataMap = {{0, instrumentsData}, {1, comboData}};
            processData(dataMap);
        }

        void Instruments::_run() {

            HTTPRequest requestInstruments(urlInstruments_); 
            HTTPRequest requestCombos(urlCombos_); 
            if (requestInstruments.isSuccess() && requestCombos.isSuccess()) {

                std::map<int, nlohmann::json> dataMap = {{0, *requestInstruments.getData()}, {1, *requestCombos.getData()}};
                processData(dataMap);

            } else {

                if (!requestInstruments.isSuccess()) setException(requestInstruments.getException()); 
                else setException(requestCombos.getException()); 
                setData(nullptr); 
                setTimestamp(requestInstruments.getTimestamp());

            }
            
        }

        std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) {

            nlohmann::json instruments = dataMap[0]; 
            nlohmann::json combos = dataMap[1]; 

            if (!instruments.contains("result")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("result"); 
            if (!instruments["result"].is_array()) throw DataEngineErrorRegistry::DataInterfaces::JSONArrayError("result");
            if (!combos.contains("result")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("result"); 
            if (!combos["result"].is_array()) throw DataEngineErrorRegistry::DataInterfaces::JSONArrayError("result");

            std::map<std::string, nlohmann::json> combosMap; 
            std::map<std::string, nlohmann::json> instrumentsMap; 
            nlohmann::json combosResult = combos["result"]; 
            nlohmann::json instrumentsResult = instruments["result"]; 

            for (const auto& item : combosResult) {

                if (!item.contains("id")) continue;
                combosMap[item["id"]] = item;
            }

            for (const auto& item : instrumentsResult) {

                if (!item.contains("instrument_name")) continue;
                std::string name = item["instrument_name"];
                auto it = combosMap.find(name); 
                if (it != combosMap.end()) {

                    nlohmann::json combo_ = it->second; 
                    nlohmann::json item_ = item; 
                    item_["combo"] = combo_;
                    instrumentsMap[name] = item_;

                } else {

                    instrumentsMap[name] = item;
                }
                
            }

            return std::make_shared<nlohmann::json>(instrumentsMap); 
        }

        
    }
}