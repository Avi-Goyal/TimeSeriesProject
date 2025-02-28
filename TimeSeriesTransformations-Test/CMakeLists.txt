cmake_minimum_required(VERSION 3.14)
project(TimeSeriesTransformationsTest)

# Create the test executable
add_executable(TimeSeriesTransformationsTest test.cpp)

# Link the TimeSeriesTransformations library
target_link_libraries(TimeSeriesTransformationsTest TimeSeriesTransformations)

# Set the output directory for the test executable
set_target_properties(TimeSeriesTransformationsTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)