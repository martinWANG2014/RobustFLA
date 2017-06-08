cmake_minimum_required(VERSION 3.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(init)
project(RobustFLA)

######## Complier message
set(CMAKE_BUILD_TYPE Release)
message(STATUS "System: ${CMAKE_SYSTEM}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

######## Set output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
endif()

######## Cplex Dependency
find_package(CPLEX REQUIRED)
if(CPLEX_FOUND)
    include_directories(${CPLEX_INCLUDE_DIR})
endif()

if(CPLEX_CONCERT_FOUND)
    include_directories(${CPLEX_CONCERT_INCLUDE_DIR})
endif()

if(CPLEX_ILOCPLEX_FOUND)
    include_directories(${CPLEX_ILOCPLEX_INCLUDE_DIR})
endif()


######## Boost dependency
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.55.0 COMPONENTS system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

#
#find_package(ZLIB REQUIRED)
#if(ZLIB_FOUND)
#    include_directories(${ZLIB_INCLUDE_DIRS})
#endif()

set(HEADER_FILES include/predefine.h include/ProcessClock.h include/Position.h include/Node.h include/FLevel.h include/Flight.h include/Route.h include/Network.h include/Input.h include/BasicSolver.h include/NodePoint.h include/Configuration.h include/EnumerationSolver.h include/GlobalDefinition.h include/MonteCarloSolver.h include/HoeffdingSolver.h include/GaussianSolver.h)
set(SOURCE_FILES main.cpp src/ProcessClock.cpp src/Position.cpp src/Node.cpp src/FLevel.cpp src/Flight.cpp src/Route.cpp src/Network.cpp src/Input.cpp src/BasicSolver.cpp src/NodePoint.cpp src/Configuration.cpp src/EnumerationSolver.cpp src/MonteCarloSolver.cpp src/HoeffdingSolver.cpp src/GaussianSolver.cpp )
add_executable(RobustFLA ${HEADER_FILES} ${SOURCE_FILES})