add_executable(SCCApp
        src/scc_app_main.cpp)

target_link_libraries(SCCApp
        SCCAlgorithms::sccalgorithms)
