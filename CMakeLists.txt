# CMakeList.txt : CMake project for 2DClothSim, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("2DClothSim")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (2DClothSim "2DClothSim.cpp" "2DClothSim.h")
target_link_libraries(2DClothSim ${SDL2_LIBRARIES})

# TODO: Add tests and install targets if needed.
