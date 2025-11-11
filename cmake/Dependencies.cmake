include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(pybind11 URL https://github.com/pybind/pybind11/archive/refs/tags/v3.0.1.tar.gz)
FetchContent_MakeAvailable(pybind11)

FetchContent_Declare(curl URL https://curl.se/download/curl-8.5.0.tar.gz)
FetchContent_MakeAvailable(curl)

FetchContent_Declare(mongocxx URL https://github.com/mongodb/mongo-cxx-driver/releases/download/r4.1.2/mongo-cxx-driver-r4.1.2.tar.gz)
FetchContent_MakeAvailable(mongocxx)




