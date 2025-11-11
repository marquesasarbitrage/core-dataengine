add_executable(dataengine-test1 ${CMAKE_CURRENT_SOURCE_DIR}/tests/test1.cpp)
target_link_libraries(dataengine-test1 PUBLIC core-dataengine)

add_executable(dataengine-test2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/test2.cpp)
target_link_libraries(dataengine-test2 PUBLIC core-dataengine)