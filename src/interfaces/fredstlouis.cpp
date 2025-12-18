#include "../../include/core-dataengine/interfaces/fredstlouis.hpp"

namespace DataInterfaces {

    namespace FredStLouis {

        FredStLouisInterface::FredStLouisInterface(): envKeyName_("APIKEY_FRED_SAINT_LOUIS"), buildName_("build"){}; 

        std::string FredStLouisInterface::getEnvKeyName() const { return envKeyName_; }

        std::string FredStLouisInterface::getApiKey() const {
            
            DataEngineTools::DotEnv env(getBuildName()); 
            return env.getVariable(getEnvKeyName());
        }

        std::string FredStLouisInterface::getBuildName() const { return buildName_; }

        std::string FredStLouisInterface::getBaseURL() const { return "https://api.stlouisfed.org/fred/"; }

        std::string FredStLouisInterface::getBaseParameters() const {return "?api_key="+getApiKey()+"&file_type=json";}

        void FredStLouisInterface::setEnvKeyName(std::string value) { envKeyName_ = value ;}

        void FredStLouisInterface::setBuildName(std::string value) { buildName_ = value ; }

        TimeSerie::TimeSerie(std::string idSerie, int limit): url_(getURL(idSerie,limit)) {}; 

        TimeSerie::TimeSerie(nlohmann::json data) {

            std::map<int, nlohmann::json> dataMap = {{0, data}};
            processData(dataMap);
        }; 

        std::shared_ptr<nlohmann::json> TimeSerie::_processData(std::map<int, nlohmann::json> dataMap) {

            nlohmann::json data0 = dataMap[0]; 
            if (!data0.contains("observations")) throw DataEngineErrorRegistry::DataInterfaces::JSONKeyError("observations"); 
            if (!data0["observations"].is_array()) throw DataEngineErrorRegistry::DataInterfaces::JSONArrayError("observations"); 
            return std::make_shared<nlohmann::json>(data0);

        }

        void TimeSerie::_run() {

            HTTPRequest request(url_); 
            if (request.isSuccess()) {

                std::map<int, nlohmann::json> dataMap = {{0, *request.getData()}};
                processData(dataMap);
            } else {

                setException(request.getException()); 
                setData(nullptr); 
                setTimestamp(request.getTimestamp());

            }
            
        }

        std::string TimeSerie::getURL(std::string idSerie, int limit) { 

            return getBaseURL() + "series/observations" + getBaseParameters() + "&series_id=" + idSerie + "&limit=" + std::to_string(limit) + "&sort_order=desc";
        }

        std::map<DateTime, double> TimeSerie::getTimeSerie() {

            run();
            std::map<DateTime, double> dataMap_; 
            if (isSuccess()) {

                nlohmann::json obs = getData()["observations"];
                for (const auto& item : obs) {

                    if (!item.contains("date")) continue;
                    if (!item.contains("value")) continue;
                    std::string date = item["date"]; 
                    std::string value = item["value"]; 

                    try { 

                        dataMap_[DateTimeTools::getFromString(date, "YYYY-MM-DD")] = DataEngineTools::stringToDouble(value);

                    } catch (const std::exception& e) {

                        continue;

                    }
                    
                }

            } 

            return dataMap_;
        }

        SOFRFixing::SOFRFixing(): TimeSerie("SOFR", 1){}; 

        EFFRFixing::EFFRFixing(): TimeSerie("DFF", 1){}; 

        HistoricalSOFRFixing::HistoricalSOFRFixing(int limit): TimeSerie("SOFR", std::max(limit,1)){}; 

        HistoricalEFFRFixing::HistoricalEFFRFixing(int limit): TimeSerie("DFF", std::max(limit,1)){}; 


        
    }
}