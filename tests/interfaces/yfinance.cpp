#include <iostream>
#include <cassert>
#include "../../include/core-dataengine/interfaces/yfinance.hpp"

void testSOFRInfo() {

    DataInterfaces::YahooFinance::SOFRFutureInfo sofrFutureInfo = DataInterfaces::YahooFinance::SOFRFutureInfo(); 
    sofrFutureInfo.run(); 
    
    assert(sofrFutureInfo.isSuccess()); 

    DataInterfaces::YahooFinance::ErisSOFRSwapFutureInfo sofrSwapFutureInfo = DataInterfaces::YahooFinance::ErisSOFRSwapFutureInfo(); 
    sofrSwapFutureInfo.run(); 
    assert(sofrSwapFutureInfo.isSuccess()); 

    int n1 = sofrSwapFutureInfo.getTickers().size(); 
    int n2 = sofrFutureInfo.getTickers().size(); 

    std::cout << "There are " + std::to_string(n1) + " existing SOFR Swap Future tickers and " +  std::to_string(n2) + " existing SOFR Futures." << std::endl;
}

void testQuotes() {

    DataInterfaces::YahooFinance::SOFRFutureInfo sofrFutureInfo = DataInterfaces::YahooFinance::SOFRFutureInfo(); 
    sofrFutureInfo.run(); 
    assert(sofrFutureInfo.isSuccess()); 
    std::vector<std::string> tickers = sofrFutureInfo.getTickers();
    DataInterfaces::YahooFinance::Quotes quote(tickers[0]); 
    quote.run(); 

    assert(quote.getData().contains(tickers[0]));

    DataInterfaces::YahooFinance::Quotes quotes(tickers); 
    quotes.run(); 

    assert(quotes.getData().size() == tickers.size());

}

int main() {

    testSOFRInfo();
    testQuotes();
    std::cout << "All tests for the Yahoo Finance data interface has been passed." << std::endl;
    return 0; 
}

