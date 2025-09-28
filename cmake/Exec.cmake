add_executable(test1 ${CMAKE_CURRENT_SOURCE_DIR}/tests/test1.cpp)
target_link_libraries(test1 PUBLIC cpp-dataengine)

add_executable(test2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/test2.cpp)
target_link_libraries(test2 PUBLIC cpp-dataengine)