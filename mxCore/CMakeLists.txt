cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( LIB_NAME mxCore )

PROJECT(LIB_NAME)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../mxCore
)

set( ${LIB_NAME}_SOURCES
    mxGeometry.cpp
    mxImage.cpp
    mxIndex.cpp
    mxObject.cpp
    mxString.cpp
    mxVoxel.cpp
)

set( ${LIB_NAME}_HEADERS
    mxBasicImage.h
    mxGeometry.h
    mxImage.h
    mxIndex.h
    mxObject.h
    mxString.h
    mxVoxel.h
)

set( ${LIB_NAME}_TEMPLATES
    mxArray.h
    mxList.h
    mxMatrix.h
    mxRegularGrid.h
)

add_definitions( -DmxGeometry_EXPORTS
    -DmxImage_EXPORTS
    -DmxIndex_EXPORTS
    -DmxObject_EXPORTS
    -DmxString_EXPORTS
    -DmxVoxel_EXPORTS
)

add_library( ${LIB_NAME} SHARED ${${LIB_NAME}_SOURCES} ${${LIB_NAME}_HEADERS} ${${LIB_NAME}_TEMPLATES})

target_link_libraries( ${LIB_NAME} )

install( FILES ${${LIB_NAME}_HEADERS} ${${LIB_NAME}_TEMPLATES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
