
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

PROJECT(MIPX_examples_GUI)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory( example_GUI_3DTrackballCameraViewer )
add_subdirectory( example_GUI_ImageViewer )
add_subdirectory( example_GUI_OrthogonalImageViewer )
add_subdirectory( example_GUI_OverlayImageViewer )
add_subdirectory( example_GUI_ParallelImageViewer )
add_subdirectory( example_GUI_SignalViewer )
add_subdirectory( example_GUI_TableViewer )
add_subdirectory( example_GUI_Widgets )
