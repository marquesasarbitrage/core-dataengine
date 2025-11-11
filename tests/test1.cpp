#include <iostream>
#include "../include/core-dataengine/http.hpp"


void testHTTP()
{
    HTTPRequest request("https://deribit.com/api/v2/public/get_instruments?currency=any");
    request.run(); 
    if (request.getException()){
        try {
            std::rethrow_exception(request.getException());
        } catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
        }
    }
    std::cout << request.getData() << std::endl; 
}

void testYahooFinance()
{
    YahooFinanceRequest request("https://query1.finance.yahoo.com/v7/finance/quote?symbols=AAPL");
    request.run(); 
    if (request.getException()){
        try {
            std::rethrow_exception(request.getException());
        } catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
        }
    }
    std::cout << request.getData() << std::endl; 
}



int main()
{
    testHTTP(); 
    testYahooFinance();
    return 0; 
}