# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)

project ("Dstarlite")

cmake_minimum_required (VERSION 3.8)


# Add source to this project's executable.
add_executable (Dstarlite "main.cpp" "dstar.cpp" "dstar.h" "dstar_draw.cpp" "dstar_env.cpp" "dstar_env.h")


