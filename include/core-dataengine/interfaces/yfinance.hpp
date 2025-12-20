#pragma once 
#include <iostream>
#include "../../../include/core-dataengine/interfaces/base.hpp"
#include "../../../include/core-dataengine/tools.hpp"
#include "core-datetime/tools.hpp"

namespace DataInterfaces {

    namespace YahooFinance {

        class YfinanceInterface : public DataInterfaces::DataInterface {

            public: 
                YfinanceInterface(); 
                virtual ~YfinanceInterface() = default;

            protected: 
                std::string getBaseURLv1() const;
                std::string getBaseURLv7() const;
                virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) override = 0;
                virtual void _run() override = 0;
        
        };

        class FutureDataInfo : public YfinanceInterface {

            public: 
                FutureDataInfo(std::string query); 
                virtual ~FutureDataInfo() = default;
            
            protected: 
                virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) override;
                virtual void _run() override;

            private: 
                std::string query_;
        };

        class SOFRFutureInfo : public FutureDataInfo {

            public: 
                SOFRFutureInfo(); 
                ~SOFRFutureInfo() = default; 

                std::vector<std::string> getTickers() const;
        };

        class ErisSOFRSwapFutureInfo : public FutureDataInfo {

            public: 
                ErisSOFRSwapFutureInfo(); 
                ~ErisSOFRSwapFutureInfo() = default; 

                std::vector<std::string> getTickers() const;
        };

        class Quotes : public YfinanceInterface {

            public: 
                Quotes(std::vector<std::string> tickers); 
                Quotes(std::string ticker); 
                virtual ~Quotes() = default;
            
            protected: 
                virtual std::shared_ptr<nlohmann::json> _processData(std::map<int, nlohmann::json> dataMap) override;
                virtual void _run() override;

            private: 
                std::vector<std::string> tickers_;
        };

    };

};