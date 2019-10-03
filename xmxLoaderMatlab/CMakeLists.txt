cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

if(MIPX_BUILD_LOADER_MATLAB)

    set( TARGET_NAME xmxLoaderMatlab )

    PROJECT(TARGET_NAME)

    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${LIBMATIO_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/../mxCore
        ${PROJECT_SOURCE_DIR}/../mxStandard
    )

    set( ${TARGET_NAME}_SOURCES
        xmxLoaderMatlab.cpp
    )

    set( ${TARGET_NAME}_HEADERS
        xmxLoaderMatlab.h
    )

    set( ${TARGET_NAME}_LIBS
        mxCore
        mxStandard
    )

    add_definitions( -DxmxLoaderMatlab_EXPORTS
    )

    add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} )

    target_link_libraries(${TARGET_NAME}
        ${${TARGET_NAME}_LIBS}
        ${LIBMATIO_LIBS}
    )

    install( FILES ${${TARGET_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
    install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
    install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

endif()
