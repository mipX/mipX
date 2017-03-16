
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

PROJECT(MIPX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

#add_subdirectory( mxCore )
#add_subdirectory( vmxCore )
#add_subdirectory( test_vtk )
add_subdirectory( test_vmxMenu )