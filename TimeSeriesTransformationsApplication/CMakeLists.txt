cmake_minimum_required(VERSION 3.14)
project(TimeSeriesTransformationsApplication)

# Create the application executable
add_executable(TimeSeriesTransformationsApplication TimeSeriesTransformationsApplication.cpp)

# Link the TimeSeriesTransformations library
target_link_libraries(TimeSeriesTransformationsApplication TimeSeriesTransformations)

# Set the output directory for the application executable
set_target_properties(TimeSeriesTransformationsApplication PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)