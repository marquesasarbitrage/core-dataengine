add_library(core-dataengine
    STATIC 
        src/http.cpp 
        src/mongo.cpp 
        src/errors.cpp 
        src/tools.cpp
        src/interfaces/base.cpp
        src/interfaces/fredstlouis.cpp
        src/interfaces/yfinance.cpp
        src/interfaces/deribit/marketdata.cpp)

target_link_libraries(core-dataengine PUBLIC core-datetime)  
target_link_libraries(core-dataengine PUBLIC pybind11::embed)
target_link_libraries(core-dataengine PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(core-dataengine PRIVATE CURL::libcurl)
target_include_directories(core-dataengine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/mongocxx-src)
target_link_libraries(core-dataengine PUBLIC bsoncxx_shared)
target_link_libraries(core-dataengine PUBLIC mongocxx_shared)

target_include_directories(core-dataengine PUBLIC include)





