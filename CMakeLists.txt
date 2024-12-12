cmake_minimum_required(VERSION 3.12.0)
project(TEAM-05-NBODY LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Useful compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


# Programming model selectable options. Default IF NOT CACHED: SERIAL. Use cached value otherwise.
set(MODEL "SERIAL" CACHE STRING "Select the programming model: SERIAL, MPI, OPENMP")
message(STATUS "MODEL ... ${MODEL}")
if (MODEL STREQUAL "SERIAL")
    message(STATUS "-- Using serial implementation")
    #Including serial implementation sources.
    file(GLOB SERIAL_SRC_FILES "src/serial/*.cpp")
    list(APPEND SRC_FILES ${SERIAL_SRC_FILES})

elseif (MODEL STREQUAL "MPI")
    message(STATUS "-- Using MPI implementation")
    # Locating MPI compiler.
    find_package(MPI REQUIRED)
    set(CMAKE_CXX_COMPILER "${MPI_CXX_COMPILER}")
    #Including MPI implementation sources.
    file(GLOB MPI_SRC_FILES "src/mpi/*.cpp")
    list(APPEND SRC_FILES ${MPI_SRC_FILES})

elseif (MODEL STREQUAL "OPENMP")
    message(STATUS "-- Using OpenMP implementation")
    #Including OpenMP implementation sources.
    file(GLOB OPENMP_SRC_FILES "src/openmp/*.cpp")
    list(APPEND SRC_FILES ${OPENMP_SRC_FILES})

else ()
    message(FATAL_ERROR "Invalid MODEL: ${MODEL}. Must be one of: SERIAL, MPI, OPENMP.")
endif ()
message(STATUS)


# Including own headers.
include_directories(include, third_party)

# Including source files.
set(MAIN_FILE "src/main.cpp")

# Building executable.
add_executable(nbody ${MAIN_FILE} ${SRC_FILES})