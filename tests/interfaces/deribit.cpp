#include <iostream>
#include <cassert>
#include "../../include/core-dataengine/interfaces/deribit/marketdata.hpp"

void testMarketData() {

    DataInterfaces::DeribitMarketData::OrderBook ob = DataInterfaces::DeribitMarketData::OrderBook("BTC-PERPETUAL",10);
    ob.run();
    assert(ob.getData()["instrument_name"] == "BTC-PERPETUAL");
    assert(ob.getData().contains("bids"));
    assert(ob.getData().contains("asks"));
    assert(ob.getData()["bids"].is_array());
    assert(ob.getData()["asks"].is_array());

    DataInterfaces::DeribitMarketData::Instruments instruments = DataInterfaces::DeribitMarketData::Instruments(); 
    instruments.run(); 
    assert(instruments.getData()["BTC-PERPETUAL"]["instrument_name"] == "BTC-PERPETUAL");
    assert(instruments.getData()["BTC-PERPETUAL"].contains("price_index"));
    assert(instruments.getData()["BTC-PERPETUAL"].contains("kind"));
    
    DataInterfaces::DeribitMarketData::SummmaryOrderBook summaryOB = DataInterfaces::DeribitMarketData::SummmaryOrderBook(); 
    summaryOB.run(); 
    assert(summaryOB.getData()["BTC-PERPETUAL"]["instrument_name"] == "BTC-PERPETUAL");
    assert(summaryOB.getData()["BTC-PERPETUAL"].contains("bid_price"));
    assert(summaryOB.getData()["BTC-PERPETUAL"].contains("ask_price"));

    

};

int main() {

    testMarketData();
    std::cout << "All tests for the Deribit data interface has been passed." << std::endl;
    return 0; 
}