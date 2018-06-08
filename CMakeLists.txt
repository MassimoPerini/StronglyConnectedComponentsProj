cmake_minimum_required(VERSION 3.10)

# Hunter, a cross-platform cmake package manager
# www.github.com/ruslo/hunter
include(cmake/HunterGate.cmake)
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.22.6.tar.gz"
        SHA1 "de6f58277ce220c208badf93c12606f458edb4c2"
)

# StronglyConnectedComponentsProj
project(scc)

set(CMAKE_CXX_STANDARD 17)

# find Boost Graph Library or Download it
hunter_add_package(Boost COMPONENTS graph)
find_package(Boost CONFIG REQUIRED graph)

option(SCC_BUILD_EXAMPLE "Build examples" ON)

if(SCC_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
