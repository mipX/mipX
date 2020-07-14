cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME mxCore )

PROJECT(TARGET_NAME)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../mxStandard
    ${PROJECT_SOURCE_DIR}/../mxCore
)

set( ${TARGET_NAME}_SOURCES
    mxCurve.cpp
    mxDataObject.cpp
    mxDataObjectTree.cpp
    mxFunctionObject.cpp
    mxGeometry.cpp
    mxGraph.cpp
    mxImage.cpp
    mxImageSliceTag.cpp
    mxIndex.cpp
    mxObject.cpp
    mxPoint.cpp
    mxPointList.cpp
    mxPosition.cpp
    mxProfile.cpp
    mxSkeleton.cpp
    mxTable.cpp
    mxVoxel.cpp
)

set( ${TARGET_NAME}_HEADERS
    mxConnectedComponent3DStatic.h
    mxCurve.h
    mxDataObject.h
    mxDataObjectFactory.h
    mxDataObjectTree.h
    mxFunctionObject.h
    mxGeometry.h
    mxGraph.h
    mxImage.h
    mxImageSliceTag.h
    mxIndex.h
    mxObject.h
    mxPoint.h
    mxPointList.h
    mxPosition.h
    mxProfile.h
    mxSkeleton.h
    mxTable.h
    mxVoxel.h
)

set( ${TARGET_NAME}_LIBS
    mxStandard
)

add_definitions( -DmxCurve_EXPORTS
    -DmxDataObject_EXPORTS
	-DmxDataObjectFactory_EXPORTS
    -DmxDataObjectTree_EXPORTS
    -DmxFunctionObject_EXPORTS
    -DmxFunctionObjectProgressIndicator_EXPORTS
    -DmxGeometry_EXPORTS
    -DmxGraph_EXPORTS
    -DmxImage_EXPORTS
    -DmxImageSliceTag_EXPORTS
    -DmxIndex_EXPORTS
    -DmxObject_EXPORTS
    -DmxPoint_EXPORTS
    -DmxPointList_EXPORTS
    -DmxPosition_EXPORTS
    -DmxProfile_EXPORTS
    -DmxSkeleton_EXPORTS
    -DmxTable_EXPORTS
    -DmxVoxel_EXPORTS
)

add_library( ${TARGET_NAME} STATIC ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} )

target_link_libraries(${TARGET_NAME}
    ${${TARGET_NAME}_LIBS}
)

install( FILES ${${TARGET_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
