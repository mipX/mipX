
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

PROJECT(MIPX_examples_GUI)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

#add_subdirectory( example_mxImage )
#add_subdirectory( example_wasm_GUI_ParallelImageViewer )
add_subdirectory( example_wasm_GUI_3DTrackballCameraViewer )
