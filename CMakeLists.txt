cmake_minimum_required(VERSION 3.10)
project(VoxelyzeExecutor)

set(CMAKE_CXX_STANDARD 14)

# Set path of voxelyze project
set(VOXELYZE ../MCC_Voxelyze)

# Include Voxelyze header files
include_directories(${VOXELYZE})

# Locate the Voxelyze static library
find_library(LIB_VOXELYZE
        NAMES Voxelyze
        PATHS ${VOXELYZE}/$ENV{VOXELYZE_BUILD_DIR})

set(SOURCES
        main.cpp
        ${VOXELYZE}/VX_Object.cpp
        ${VOXELYZE}/VX_Environment.cpp
        ${VOXELYZE}/VX_Sim.cpp
        ${VOXELYZE}/VX_SimGA.cpp)

#add_executable(VoxelyzeExecutor ${SOURCES})
add_executable(VoxelyzeExecutor
        main.cpp)

target_link_libraries(VoxelyzeExecutor
        ${LIB_VOXELYZE})