hunter_add_package(Boost COMPONENTS graph)
find_package(Boost CONFIG REQUIRED graph)

add_library(SCCAlgorithms
        src/scc_algorithms_pearce.cpp
        src/scc_algorithms_nuutila.cpp
        src/scc_utils.cpp
        src/iterative/scc_algorithms_nuutila1_iterative.cpp
        src/iterative/scc_algorithms_nuutila2_iterative.cpp
        src/scc_algorithms_tarjan.cpp)

add_library(SCCAlgorithms::sccalgorithms
        ALIAS
            SCCAlgorithms)

target_include_directories(SCCAlgorithms
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(SCCAlgorithms
        PUBLIC
            Boost::graph)

option(SCC_ALGORITHMS_COVERAGE "Enable to generate coverage reports on sccalgorithms library" OFF)
if(CMAKE_BUILD_TYPE MATCHES Debug AND SCC_ALGORITHMS_COVERAGE)
    include(CodeCoverage)
    SCC_APP_COMPILE_WITH_COV(SCCAlgorithms)
endif() # SCC_ALGORITHMS_COVERAGE

option(SCC_ALGORITHM_BUILD_TEST "Build also library tests" OFF)
if(SCC_ALGORITHM_BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif() # SCC_ALGORITHM_BUILD_TEST

