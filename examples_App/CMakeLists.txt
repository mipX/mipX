
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

PROJECT(examples_App)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory( example_App_Graphs )
add_subdirectory( example_App_Histogram )
add_subdirectory( example_App_ImageAndSignalProcessing )
add_subdirectory( example_App_ParallelImageViewer )
add_subdirectory( example_App_Skeleton )
add_subdirectory( example_App_TableViewer )
