cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME mxStandard )

PROJECT(TARGET_NAME)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../mxStandard
)

set( ${TARGET_NAME}_SOURCES
    mxString.cpp
)

set( ${TARGET_NAME}_HEADERS
    mxArray.h
    mxFileOperations.h
    mxList.h
    mxMatrix.h
    mxRegularGrid.h
    mxScopedPointer.h
    mxString.h
)

add_definitions( -DmxString_EXPORTS
)

add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} )

target_link_libraries( ${TARGET_NAME} )

install( FILES ${${TARGET_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
