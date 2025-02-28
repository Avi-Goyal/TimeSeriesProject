cmake_minimum_required(VERSION 3.14)

# Define the TimeSeriesTransformations library
add_library(TimeSeriesTransformations STATIC
    TimeSeriesTransformations.cpp
    TimeSeriesTransformations.h
)

# Include the current directory for header files
target_include_directories(TimeSeriesTransformations PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
