cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME mxCore )

PROJECT(TARGET_NAME)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../mxCore
)

set( ${TARGET_NAME}_SOURCES
    mxCurve.cpp
    mxDataObject.cpp
    mxDataObjectTree.cpp
    mxFunctionObject.cpp
    mxGeometry.cpp
    mxImage.cpp
    mxIndex.cpp
    mxObject.cpp
    mxPoint.cpp
    mxPosition.cpp
    mxString.cpp
    mxVoxel.cpp
)

set( ${TARGET_NAME}_HEADERS
    mxBasicImage.h
    mxCurve.h
    mxDataObject.h
    mxDataObjectFactory.h
    mxDataObjectTree.h
    mxFunctionObject.h
    mxGeometry.h
    mxImage.h
    mxIndex.h
    mxObject.h
    mxPoint.h
    mxPosition.h
    mxString.h
    mxVoxel.h
)

set( ${TARGET_NAME}_TEMPLATES
    mxArray.h
    mxList.h
    mxMatrix.h
    mxRegularGrid.h
)

add_definitions( -DmxCurve_EXPORTS
    -DmxDataObject_EXPORTS
    -DmxDataObjectTree_EXPORTS
    -DmxFunctionObject_EXPORTS
    -DmxFunctionObjectProgressIndicator_EXPORTS
    -DmxGeometry_EXPORTS
    -DmxImage_EXPORTS
    -DmxIndex_EXPORTS
    -DmxObject_EXPORTS
    -DmxPoint_EXPORTS
    -DmxPosition_EXPORTS
    -DmxString_EXPORTS
    -DmxVoxel_EXPORTS
)

add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} ${${TARGET_NAME}_TEMPLATES})

target_link_libraries( ${TARGET_NAME} )

install( FILES ${${TARGET_NAME}_HEADERS} ${${TARGET_NAME}_TEMPLATES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
