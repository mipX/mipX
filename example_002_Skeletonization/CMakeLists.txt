cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME example_002_Skeletonization )

PROJECT(${TARGET_NAME})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

option(MIPX_USE_VTK_WITH_QT "Turn this 'On' in case you have built VTK with Qt support")

if(MIPX_USE_VTK_WITH_QT)
    set(CMAKE_AUTOMOC ON)
    find_package(Qt5Widgets REQUIRED QUIET)
    qt5_wrap_ui(UISrcs ${UI_FILES} )
endif()


#add_definitions( -DmxCore_USE_SOURCE_CODE
#    -DvmxCore_USE_SOURCE_CODE
#)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../mxCore
    ${PROJECT_SOURCE_DIR}/../mxImageProcessing
    ${PROJECT_SOURCE_DIR}/../vmxAppCore
    ${PROJECT_SOURCE_DIR}/../vmxCore
    ${PROJECT_SOURCE_DIR}/../vmxGUI
)

set( ${TARGET_NAME}_SOURCES
    ${TARGET_NAME}.cpp
)

set( ${TARGET_NAME}_HEADERS
)

set( ${TARGET_NAME}_LIBS
    mxCore
    mxImageProcessing
    vmxAppCore
    vmxCore
    vmxGUI
)

set( MACOSX_RESOURCE_FILES ${CMAKE_SOURCE_DIR}/Files/Icons/mipx_icon_mipx.png )
set( MACOSX_BUNDLE_INFO_STRING "@EXE_NAME@ - Version ${VERSION}" )
set( MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION} )
set( MACOSX_BUNDLE_ICON_FILE "mipx_icon_mipx.png" )
set( MACOSX_BUNDLE_GUI_IDENTIFIER "com.sri.aic" )
set( MACOSX_BUNDLE_BUNDLE_NAME "@EXE_NAME@" )


add_executable(${TARGET_NAME} MACOSX_BUNDLE
    ${${TARGET_NAME}_SOURCES}
    ${${TARGET_NAME}_HEADERS}
    #${MOC_SOURCE_FILES}
    #${UI_SOURCE_FILES}
    #${RESOURCE_SOURCE_FILES}
    #${HEADER_FILES}
    ${MACOSX_RESOURCE_FILES}
)

if(MIPX_USE_VTK_WITH_QT)
    qt5_use_modules(${TARGET_NAME} Core Gui)
endif()


target_link_libraries(${TARGET_NAME}
    ${VTK_LIBRARIES}
    ${${TARGET_NAME}_LIBS}
)


set_source_files_properties(
      ${MACOSX_RESOURCE_FILES}
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
)

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(CODE "
	include(BundleUtilities)
	fixup_bundle( \"${CMAKE_INSTALL_PREFIX}/bin/${TARGET_NAME}.app\" \"\" \"${CMAKE_INSTALL_PREFIX}/bin\" )
    " COMPONENT Runtime
)
