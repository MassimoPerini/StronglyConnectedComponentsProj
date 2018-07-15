hunter_add_package(Boost COMPONENTS graph)
find_package(Boost CONFIG REQUIRED graph)

add_library(SCCAlgorithms
        src/scc_algorithms_pearce.cpp
        src/scc_algorithms_nuutila.cpp
        src/scc_utils.cpp
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

option(SCC_ALGORITHM_BUILD_TEST "Build also library tests" ON)
if(SCC_ALGORITHM_BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
