cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

if(MIPX_BUILD_LOADER_DICOM)

    set( TARGET_NAME xmxLoaderDicom )

    PROJECT(TARGET_NAME)

    find_package(VTK REQUIRED)
    include(${VTK_USE_FILE})


    option(MIPX_USE_VTK_WITH_QT "Turn this 'On' in case you have built VTK with Qt support")

    if(MIPX_USE_VTK_WITH_QT)
        set(CMAKE_AUTOMOC ON)
        find_package(Qt5Widgets REQUIRED QUIET)
        qt5_wrap_ui(UISrcs ${UI_FILES} )
    endif()


    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${GDCM_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/../mxCore
        ${PROJECT_SOURCE_DIR}/../mxStandard
    )

    set( ${TARGET_NAME}_SOURCES
        xmxLoaderDicom.cpp
    )

    set( ${TARGET_NAME}_HEADERS
        xmxLoaderDicom.h
    )

    set( ${TARGET_NAME}_LIBS
        mxCore
        mxStandard
    )

    add_definitions( -DxmxLoaderDicom_EXPORTS
    )

    if(MIPX_USE_VTK_WITH_QT)
        add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} ${UISrcs} ${QT_WRAP})
        qt5_use_modules(${TARGET_NAME} Core Gui)
    else()
        add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} )
    endif()

    # add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} )

    target_link_libraries(${TARGET_NAME}
        ${${TARGET_NAME}_LIBS}
        ${VTK_LIBRARIES}
        ${GDCM_LIBS}
    )


    install( FILES ${${TARGET_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
    install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
    install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

endif()
