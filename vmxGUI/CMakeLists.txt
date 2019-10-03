cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME vmxGUI )

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
    ${PROJECT_SOURCE_DIR}/../mxStandard
    ${PROJECT_SOURCE_DIR}/../vmxGUI
)

set( ${TARGET_NAME}_SOURCES
    tinyfiledialogs.c
    vmxGUIButtonGroup.cpp
    vmxGUIFilesDialog.cpp
    vmxGUIIconButtonGroup.cpp
    vmxGUIInputWidget.cpp
    vmxGUIInteractorStyle.cpp
    #vmxGUIInteractorStyleImage.cpp
    #vmxGUIInteractorStyleTrackballCamera.cpp
    vmxGUILabel.cpp
    vmxGUIListWidget.cpp
    #vmxGUIMainWidget.cpp
    vmxGUIMenu.cpp
    vmxGUIMenuBar.cpp
    #vmxGUIRenderer.cpp
    vmxGUIRenderer3DTrackballCamera.cpp
    vmxGUIRendererImageViewer.cpp
    vmxGUIRendererSignalViewer.cpp
    vmxGUIRendererTableViewer.cpp
    vmxGUIWidget.cpp
)

set( ${TARGET_NAME}_HEADERS
    tinydir.h
    tinyfiledialogs.h
    vmxGUIButtonGroup.h
    vmxGUIClipBoard.h
    vmxGUIEventDetector.h
    vmxGUIFilesDialog.h
    vmxGUIIconButtonGroup.h
    vmxGUIInputWidget.h
    vmxGUIInteractorStyle.h
    #vmxGUIInteractorStyleImage.h
    #vmxGUIInteractorStyleTrackballCamera.h
    vmxGUILabel.h
    vmxGUIListWidget.h
    #vmxGUIMainWidget.h
    vmxGUIMenu.h
    vmxGUIMenuBar.h
    #vmxGUIRenderer.h
    vmxGUIRenderer3DTrackballCamera.h
    vmxGUIRendererImageViewer.h
    vmxGUIRendererSignalViewer.h
    vmxGUIRendererTableViewer.h
    vmxGUIWidget.h
)

set( ${TARGET_NAME}_LIBS
    mxStandard
)

add_definitions( -DvmxGUIButtonGroup_EXPORTS
    -DvmxGUIClipBoard_EXPORTS
    -DvmxGUIEventDetector_EXPORTS
    -DvmxGUIFilesDialog_EXPORTS
    -DvmxGUIInputWidget_EXPORTS
    -DvmxGUIIconButtonGroup_EXPORTS
    -DvmxGUIInteractorStyle_EXPORTS
    #-DvmxGUIInteractorStyleImage_EXPORTS
    #-DvmxGUIInteractorStyleTrackballCamera_EXPORTS
    -DvmxGUILabel_EXPORTS
    -DvmxGUIListWidget_EXPORTS
    #-DvmxGUIMainWidget_EXPORTS
    -DvmxGUIMenu_EXPORTS
    -DvmxGUIMenuBar_EXPORTS
    #-DvmxGUIRenderer_EXPORTS
    -DvmxGUIRenderer3DTrackballCamera_EXPORTS
    -DvmxGUIRendererImageViewer_EXPORTS
    -DvmxGUIRendererSignalViewer_EXPORTS
    -DvmxGUIRendererTableViewer_EXPORTS
    -DvmxGUIWidget_EXPORTS
)



if(MIPX_USE_VTK_WITH_QT)
    add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} ${UISrcs} ${QT_WRAP})
    qt5_use_modules(${TARGET_NAME} Core Gui)
else()
    add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} )
endif()


target_link_libraries(${TARGET_NAME}
    ${VTK_LIBRARIES}
    ${${TARGET_NAME}_LIBS}
)


install( FILES ${${TARGET_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
