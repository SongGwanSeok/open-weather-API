cmake_minimum_required(VERSION 3.23)
project(open-weather-API)

set(CMAKE_CXX_STANDARD 14)

add_executable(open-weather-API main.cpp)

# Link the curl library
target_link_libraries(open-weather-API curl)
