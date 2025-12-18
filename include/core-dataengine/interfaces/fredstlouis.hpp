#pragma once 
#include <iostream>
#include "base.hpp"
#include "../../../include/core-dataengine/tools.hpp"
#include "core-datetime/tools.hpp"

namespace DataInterfaces {

    namespace FredStLouis {

        class FredStLouisInterface : public DataInterfaces::DataInterface {

            public: 
                FredStLouisInterface(); 
                virtual ~FredStLouisInterface() = default;

                std::string getEnvKeyName() const; 
                std::string getBuildName() const; 
                void setEnvKeyName(std::string value); 
                void setBuildName(std::string value); 

            protected: 
                std::string getApiKey() const; 
                std::string getBaseURL() const;
                std::string getBaseParameters() const;
                virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) override = 0;
                virtual void _run() override = 0;
            
            private: 
                std::string envKeyName_; 
                std::string buildName_;

        };

        class TimeSerie : public FredStLouisInterface {

            public: 
                TimeSerie(std::string idSerie, int limit); 
                TimeSerie(nlohmann::json data);
                virtual ~TimeSerie() = default;

                std::map<DateTime, double> getTimeSerie();

            protected: 
                virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) override;
                virtual void _run() override;

            private: 
                std::string getURL(std::string idSerie, int limit);
                std::string url_;


        };

        class SOFRFixing : public TimeSerie {public: SOFRFixing(); ~SOFRFixing() = default; };

        class HistoricalSOFRFixing: public TimeSerie {public: HistoricalSOFRFixing(int limit); ~HistoricalSOFRFixing() = default; };

        class EFFRFixing : public TimeSerie {public: EFFRFixing(); ~EFFRFixing() = default; };

        class HistoricalEFFRFixing: public TimeSerie {public: HistoricalEFFRFixing(int limit); ~HistoricalEFFRFixing() = default; };
    }
}

