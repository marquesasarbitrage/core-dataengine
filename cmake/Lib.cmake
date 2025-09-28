add_library(cpp-dataengine STATIC src/http.cpp src/envload.cpp src/mongo.cpp src/errors.cpp src/tools.cpp)

target_link_libraries(cpp-dataengine PUBLIC pybind11::embed)
target_link_libraries(cpp-dataengine PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(cpp-dataengine PUBLIC CURL::libcurl)

target_include_directories(cpp-dataengine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/mongocxx-src)
target_link_libraries(cpp-dataengine PUBLIC bsoncxx_shared)
target_link_libraries(cpp-dataengine PUBLIC mongocxx_shared)

target_include_directories(cpp-dataengine PUBLIC include)



