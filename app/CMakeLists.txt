hunter_add_package(Boost COMPONENTS graph)
find_package(Boost CONFIG REQUIRED graph)

add_executable(SCCApp
        src/scc_app_main.cpp
        src/scc_reports.cpp)

target_include_directories(SCCApp
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(SCCApp
        PUBLIC
            SCCAlgorithms::sccalgorithms)
