#include "../../include/core-dataengine/interfaces/yfinance.hpp"

namespace DataInterfaces {

    namespace YahooFinance {

            YfinanceInterface::YfinanceInterface(){};

            std::string YfinanceInterface::getBaseURLv1() const { return "https://query1.finance.yahoo.com/v1/finance/"; }

            std::string YfinanceInterface::getBaseURLv7() const { return "https://query1.finance.yahoo.com/v7/finance/"; }

            FutureDataInfo::FutureDataInfo(std::string query): query_(query) {}; 

            std::shared_ptr<nlohmann::json> FutureDataInfo::_processData(std::map<int, nlohmann::json> dataMap) {

                nlohmann::json data0 = dataMap[0]; 
                if (!data0.contains("finance")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("finance"); 
                nlohmann::json finance = data0["finance"];
                if (!finance.contains("result")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("result"); 
                nlohmann::json result = finance["result"]; 
                if (!result.contains("documents")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("documents"); 
                if (!result["documents"].is_array()) throw DataEngineErrorRegistry::DataInterfaces::JSONArrayError("documents");

                return std::make_shared<nlohmann::json>(data0);

            }

            void FutureDataInfo::_run(){

                HTTPRequest request(getBaseURLv1() + "lookup?query=" + query_ + "&quoteType=future/future?start=0&count=10000"); 
                if (request.isSuccess()) {

                    std::map<int, nlohmann::json> dataMap = {{0, *request.getData()}};
                    processData(dataMap);
                } else {

                    setException(request.getException()); 
                    setData(nullptr); 
                    setTimestamp(request.getTimestamp());

                }
            }

            SOFRFutureInfo::SOFRFutureInfo(): FutureDataInfo("-Month SOFR Futures"){}; 

            std::vector<std::string> SOFRFutureInfo::getTickers() const {

                std::vector<std::string> output; 
                for (const auto& item : getData()["documents"]) {

                    if (!item.contains("symbol")) continue;
                    std::string symbol = item["symbol"];
                    if (symbol.find("CME") != std::string::npos && symbol.rfind("SR", 0) == 0) {   

                        output.push_back(symbol);
                    }
                }
                return output;
            }
    
            ErisSOFRSwapFutureInfo::ErisSOFRSwapFutureInfo(): FutureDataInfo("SOFR Swap Futures"){}; 

            std::vector<std::string> ErisSOFRSwapFutureInfo::getTickers() const {

                std::vector<std::string> output; 
                for (const auto& item : getData()["documents"]) {

                    if (!item.contains("symbol")) continue;
                    if (!item.contains("quoteType")) continue;
                    std::string symbol = item["symbol"];
                    if (symbol.find("CBT") != std::string::npos && symbol.rfind("YI", 0) == 0 && item["quoteType"]=="future") { 

                        output.push_back(symbol);
                    }
                }
                return output;
            }
    
            Quotes::Quotes(std::vector<std::string> tickers): tickers_(tickers) {}; 

            Quotes::Quotes(std::string ticker): tickers_({ticker}) {}; 

            std::shared_ptr<nlohmann::json> Quotes::_processData(std::map<int, nlohmann::json> dataMap) {

                nlohmann::json data0 = dataMap[0]; 
                if (!data0.contains("quoteResponse")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("quoteResponse"); 
                nlohmann::json response = data0["quoteResponse"];
                if (!response.contains("result")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("result"); 
                if (!response["result"].is_array()) throw DataEngineErrorRegistry::DataInterfaces::JSONArrayError("result");
                nlohmann::json result = response["result"]; 
                return std::make_shared<nlohmann::json>(data0); 

            }

            void Quotes::_run() {

                std::string tickerJoin = "";
                for (const std::string& s : tickers_) {

                    tickerJoin += s; 
                    tickerJoin += ",";
                }

                YahooFinanceRequest request(getBaseURLv7() + "quote?symbols=" + tickerJoin); 
                if (request.isSuccess()) {

                    std::map<int, nlohmann::json> dataMap = {{0, *request.getData()}};
                    processData(dataMap);
                    
                } else {

                    setException(request.getException()); 
                    setData(nullptr); 
                    setTimestamp(request.getTimestamp());

                }
            }

    }
}