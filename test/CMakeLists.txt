hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_executable(scc_test
        scc_test_all.cpp
        scc_test_boostscc.cpp)
target_link_libraries(scc_test GTest::gtest Boost::graph)
