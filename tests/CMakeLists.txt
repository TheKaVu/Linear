project(Linear_test)
add_subdirectory(gtest_lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(GTest
        VecTest.cpp
        MatTest.cpp
        RationalTest.cpp)

target_link_libraries(GTest Linear)

target_link_libraries(GTest gtest gtest_main)