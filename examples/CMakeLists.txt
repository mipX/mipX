
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

PROJECT(MIPX_examples)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory( example_BasicVisualization )
add_subdirectory( example_Skeletonization )
add_subdirectory( example_HistogramAsCurve )
