#include "../../../include/core-dataengine/interfaces/deribit/marketdata.hpp"

namespace DataInterfaces {

    namespace DeribitMarketData {

        DeribitMarketDataInterface::DeribitMarketDataInterface(){};

        std::string DeribitMarketDataInterface::getBaseURL() const { return "https://deribit.com/api/v2/public/"; }

        Instruments::Instruments(){};

        Instruments::Instruments(nlohmann::json instrumentsData, nlohmann::json comboData) {

            std::map<int, nlohmann::json> dataMap = {{0, instrumentsData}, {1, comboData}};
            processData(dataMap);
        }

        void Instruments::_run() {

            HTTPRequest requestInstruments(getBaseURL()+"get_instruments?currency=any"); 
            HTTPRequest requestCombos(getBaseURL()+"get_combos?currency=any"); 
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

        std::shared_ptr<nlohmann::json> Instruments::_processData(std::map<int, nlohmann::json> dataMap) {

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

        SummmaryOrderBook::SummmaryOrderBook(){};

        SummmaryOrderBook::SummmaryOrderBook(std::map<int, nlohmann::json> summaryOrderbookMap){

            _processData(summaryOrderbookMap);
        };

        void SummmaryOrderBook::_run() {

            std::vector<std::string> urls_ = {
                getBaseURL()+"get_book_summary_by_currency?currency=BTC", 
                getBaseURL()+"get_book_summary_by_currency?currency=ETH", 
                getBaseURL()+"get_book_summary_by_currency?currency=USDC", 
                getBaseURL()+"get_book_summary_by_currency?currency=USDT", 
                getBaseURL()+"get_book_summary_by_currency?currency=EURR"
            }; 

            std::exception_ptr ePtr = nullptr;
            DateTime last = DateTime();
            std::map<int, nlohmann::json> dataMap;

            for (int i = 0; i<urls_.size(); i++) {

                HTTPRequest request(urls_[i]);
                last = request.getTimestamp();
                if (request.isSuccess()) {

                    dataMap[i] = *request.getData(); 

                } else {

                    ePtr = request.getException();
                    continue; 
                }
            }


            if (dataMap.size()>0) {
                
                processData(dataMap);

            } else {

                setException(ePtr);
                setData(nullptr); 
                setTimestamp(last);

            }
        }
    
        std::shared_ptr<nlohmann::json> SummmaryOrderBook::_processData(std::map<int, nlohmann::json> dataMap) {

            std::map<std::string, nlohmann::json> outputMap; 
            for (const auto& [key, value] : dataMap) {

                if (!value.contains("result")) continue; 
                if (!value["result"].is_array()) continue;
                nlohmann::json result = value["result"]; 

                for (const auto& item : result) {
                    if (!item.contains("instrument_name")) continue;
                    outputMap[item["instrument_name"]] = item;
                }

            }
            return std::make_shared<nlohmann::json>(outputMap); 
        }

        OrderBook::OrderBook(std::string instrumentName): instrumentName_(instrumentName), depth_(10){}

        OrderBook::OrderBook(std::string instrumentName, int depth): instrumentName_(instrumentName), depth_(depth){}

        OrderBook::OrderBook(nlohmann::json orderBookData): instrumentName_(""){

            std::map<int, nlohmann::json> dataMap = {{0, orderBookData}};
            processData(dataMap);
        }

        std::string OrderBook::getInstrumentName() const { return instrumentName_; }

        void OrderBook::_run() {

            HTTPRequest request(getBaseURL()+"get_order_book?instrument_name="+instrumentName_+ "&depth="+ std::to_string(depth_)); 
            if (request.isSuccess()) {

                std::map<int, nlohmann::json> dataMap = {{0, *request.getData()}};
                processData(dataMap);

            } else {

                setException(request.getException()); 
                setData(nullptr); 
                setTimestamp(request.getTimestamp());

            }

        }

        std::shared_ptr<nlohmann::json> OrderBook::_processData(std::map<int, nlohmann::json> dataMap) {

            nlohmann::json data0 = dataMap[0]; 
            if (!data0.contains("result")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("result"); 
            nlohmann::json result = data0["result"]; 
            if (!result.contains("instrument_name")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("instrument_name"); 
            instrumentName_ = result["instrument_name"];
            return std::make_shared<nlohmann::json>(result);
        }
        
    }
}