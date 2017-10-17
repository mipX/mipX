cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME mxImageProcessing )

PROJECT(TARGET_NAME)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../mxCore
)

set( ${TARGET_NAME}_SOURCES
    mxBIPX.cpp
    mxGIPX.cpp
)

set( ${TARGET_NAME}_HEADERS
    mxBIPX.h
    mxGIPX.h
)

set( ${TARGET_NAME}_TEMPLATES
    ${PROJECT_SOURCE_DIR}/../mxCore/mxArray.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxList.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxMatrix.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxRegularGrid.h
)

set( ${TARGET_NAME}_LIBS
    mxCore
)

add_definitions( -DmxBIPX_EXPORTS
    -DmxGIPX_EXPORTS
)

add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} ${${TARGET_NAME}_TEMPLATES})

target_link_libraries( ${TARGET_NAME} ${${TARGET_NAME}_LIBS})

install( FILES ${${TARGET_NAME}_HEADERS} ${${TARGET_NAME}_TEMPLATES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
