cmake_minimum_required(VERSION 3.14)
project(TimeSeriesProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(TimeSeriesTransformations)
add_subdirectory(TimeSeriesTransformations-Test)
add_subdirectory(TimeSeriesTransformationsApplication)