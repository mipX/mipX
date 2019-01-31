cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME mxProcessing )

PROJECT(TARGET_NAME)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../mxCore
    ${PROJECT_SOURCE_DIR}/../mxStandard
)

set( ${TARGET_NAME}_SOURCES
    mxBIP.cpp
    mxCurveProcessing.cpp
    mxGIP.cpp
    mxGraphProcessing.cpp
    mxSkeletonization.cpp
)

set( ${TARGET_NAME}_HEADERS
    mxBIP.h
    mxCurveProcessing.h
    mxGIP.h
    mxGraphProcessing.h
    mxSkeletonization.h
)

set( ${TARGET_NAME}_LIBS
    mxCore
)

add_definitions( -DmxBIP_EXPORTS
    -DmxCurveProcessing_EXPORTS
    -DmxGIP_EXPORTS
    -DmxGraphProcessing_EXPORTS
    -DmxSkeletonization_EXPORTS
)

add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} )

target_link_libraries( ${TARGET_NAME} ${${TARGET_NAME}_LIBS})

install( FILES ${${TARGET_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
