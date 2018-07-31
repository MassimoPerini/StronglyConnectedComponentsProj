hunter_add_package(Boost COMPONENTS graph)

find_package(Boost CONFIG REQUIRED graph)

add_executable(SCCApp
        src/scc_app_main.cpp
        src/get_rss.cpp)

target_include_directories(SCCApp
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(SCCApp
        PUBLIC
            SCCAlgorithms::sccalgorithms)

# link library to extract profiler utilities on Windows psapi.lib
if(WIN32)
    find_library (PSAPI Psapi)
    if(PSAPI)
        target_link_libraries(SCCApp
            PUBLIC
                ${PSAPI})
    else()
        message(FATAL_ERROR "psapi.lib not found. It is needed to create memory reports. borting...")
    endif()
endif()

option(SCC_APP_COVERAGE "Enable if you want to generate coverage reports" OFF)
set(SCC_APP_COVERAGE_ARGV "100 101 2 0.01 0.2 0.2" CACHE STRING "Parameters for program runned in coverage")
if(CMAKE_BUILD_TYPE MATCHES Debug AND SCC_APP_COVERAGE)
    include(CodeCoverage)
    scc_app_add_coverage_to(SCCApp coverage ${SCC_APP_COVERAGE_ARGV})
endif()
