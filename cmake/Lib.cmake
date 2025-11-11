add_library(core-dataengine
    STATIC 
        src/http.cpp 
        src/mongo.cpp 
        src/errors.cpp 
        src/tools.cpp)

target_link_libraries(core-dataengine PRIVATE pybind11::embed)
target_link_libraries(core-dataengine PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(core-dataengine PRIVATE CURL::libcurl)
target_include_directories(core-dataengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/mongocxx-src)
target_link_libraries(core-dataengine PRIVATE bsoncxx_shared)
target_link_libraries(core-dataengine PRIVATE mongocxx_shared)

target_include_directories(core-dataengine PUBLIC include)



