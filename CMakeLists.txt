cmake_minimum_required(VERSION 3.9.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


#set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
#set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")
#set(CMAKE_VERBOSE_MAKEFILE TRUE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -v -stdlib=libc++")

#set(CXX_STANDARD_REQUIRED)


# Hunter, a cross-platform cmake package manager
# www.github.com/ruslo/hunter
include(HunterGate)
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.22.6.tar.gz"
        SHA1 "de6f58277ce220c208badf93c12606f458edb4c2"
)




# StronglyConnectedComponentsProj
project(scc)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(libsccalgorithms)

add_subdirectory(app)

