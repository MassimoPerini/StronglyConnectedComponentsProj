hunter_add_package(Boost COMPONENTS graph)
find_package(Boost CONFIG REQUIRED graph)

add_executable(SCCApp
        src/scc_app_main.cpp
        src/scc_reports.cpp include/console_formatter.h)

target_include_directories(SCCApp
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(SCCApp
        PUBLIC
            SCCAlgorithms::sccalgorithms)

option(SCC_APP_COVERAGE "Enable if you want to generate coverage reports" OFF)
set(SCC_APP_COVERAGE_ARGV "100 101 2 0.01 0.2 0.2" CACHE STRING "Parameters for program runned in coverage")
if(CMAKE_BUILD_TYPE MATCHES Debug AND SCC_APP_COVERAGE)
    include(CodeCoverage)
    scc_app_add_coverage_to(SCCApp coverage ${SCC_APP_COVERAGE_ARGV})
endif()
