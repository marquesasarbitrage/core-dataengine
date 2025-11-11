#include <iostream>
#include "../include/core-dataengine/tools.hpp"

int main()
{
    DataEngineTools::DotEnv load("build"); 
    std::cout << load.getVariable("MONGO_URI") << std::endl;
    return 0;
}