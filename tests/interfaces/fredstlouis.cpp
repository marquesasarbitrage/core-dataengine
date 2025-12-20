#include <iostream>
#include <cassert>
#include "../../include/core-dataengine/interfaces/fredstlouis.hpp"

void testTimeSerie() {

    DataInterfaces::FredStLouis::SOFRFixing sofrFixing = DataInterfaces::FredStLouis::SOFRFixing(); 
    sofrFixing.run();
    assert(sofrFixing.getTimeSerie().size()==1);

    DataInterfaces::FredStLouis::HistoricalSOFRFixing historicalSOFRFixing = DataInterfaces::FredStLouis::HistoricalSOFRFixing(10);
    historicalSOFRFixing.run();
    assert(historicalSOFRFixing.getTimeSerie().size()==10);

};

int main() {

    testTimeSerie();
    std::cout << "All tests for the Fred St Louis interface has been passed." << std::endl;
    return 0; 
}