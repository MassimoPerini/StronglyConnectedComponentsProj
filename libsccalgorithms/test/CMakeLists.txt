hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_executable(SCCAlgorithms_test
        src/scc_algorithms_test_all.cpp
        src/scc_algorithms_test_boost.cpp)

target_link_libraries(SCCAlgorithms_test
        GTest::gtest
        SCCAlgorithms::sccalgorithms)

add_test(TestLibSccAlgorithms SCCAlgorithms_test)
