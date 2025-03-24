cmake_minimum_required(VERSION 3.10)
project(lab2_zymovets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(dependencies/cpp-httplib)
add_subdirectory(dependencies/cpp-dotenv)
add_subdirectory(dependencies/nlohmann/json)
add_subdirectory(0_root)
