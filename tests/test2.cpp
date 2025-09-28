#include <iostream>
#include "../include/cpp-dataengine/core.hpp"

int main()
{
    DataEngineEnvFileLoad load("build"); 
    std::cout << load.getVariable("MONGO_URI") << std::endl;
    return 0;
}