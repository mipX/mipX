cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME vmxAppCore )

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
    ${PROJECT_SOURCE_DIR}/../mxCore
#    ${PROJECT_SOURCE_DIR}/../vmxCore
    ${PROJECT_SOURCE_DIR}/../vmxGUI
)

set( ${TARGET_NAME}_SOURCES
    mxDataContainer.cpp
    mxFunctionConfig.cpp
    vmxDataListWidget.cpp
)

set( ${TARGET_NAME}_HEADERS
    mxDataContainer.h
    mxFunctionConfig.h
    vmxDataListWidget.h
)

set( ${TARGET_NAME}_TEMPLATES
    ${PROJECT_SOURCE_DIR}/../mxCore/mxArray.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxList.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxMatrix.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxRegularGrid.h
)

set( ${TARGET_NAME}_LIBS
    mxCore
    vmxGUI
)

add_definitions( -DmxDataContainer_EXPORTS
    -DmxFunctionConfig_EXPORTS
    -DvmxDataListWidget_EXPORTS
)


if(MIPX_USE_VTK_WITH_QT)
    add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} ${${TARGET_NAME}_TEMPLATES} ${UISrcs} ${QT_WRAP})
    qt5_use_modules(${TARGET_NAME} Core Gui)
else()
    add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} ${${TARGET_NAME}_TEMPLATES})
endif()


target_link_libraries(${TARGET_NAME}
    ${VTK_LIBRARIES}
    ${${TARGET_NAME}_LIBS}
)


install( FILES ${${TARGET_NAME}_HEADERS} ${${TARGET_NAME}_TEMPLATES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
