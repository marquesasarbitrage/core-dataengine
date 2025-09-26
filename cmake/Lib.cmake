add_library(cpp-marketdata STATIC src/http.cpp)

target_link_libraries(cpp-marketdata PUBLIC pybind11::embed)

target_link_libraries(cpp-marketdata PUBLIC nlohmann_json::nlohmann_json)

target_link_libraries(cpp-marketdata PUBLIC CURL::libcurl)

target_include_directories(cpp-marketdata PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/mongocxx-src)
target_link_libraries(cpp-marketdata PUBLIC bsoncxx_shared)
target_link_libraries(cpp-marketdata PUBLIC mongocxx_shared)

target_include_directories(cpp-marketdata PUBLIC include)



