cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)



if(MIPX_BUILD_LOADER_NIFTI)

    set( TARGET_NAME xmxLoaderNifti )

    PROJECT(TARGET_NAME)

    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${NIFTICLIB_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/../mxCore
        ${PROJECT_SOURCE_DIR}/../mxStandard
    )

    set( ${TARGET_NAME}_SOURCES
        xmxLoaderNifti.cpp
    )

    set( ${TARGET_NAME}_HEADERS
        xmxLoaderNifti.h
    )

    set( ${TARGET_NAME}_LIBS
        mxCore
        mxStandard
    )

    add_definitions( -DxmxLoaderNifti_EXPORTS
    )

    add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} )

    target_link_libraries(${TARGET_NAME}
        ${${TARGET_NAME}_LIBS}
        ${NIFTICLIB_LIBS}
    )


    install( FILES ${${TARGET_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
    install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
    install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

endif()
