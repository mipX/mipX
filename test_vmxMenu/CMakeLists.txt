cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( APP_NAME test_vmxMenu )

PROJECT(${APP_NAME})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED QUIET)

qt5_wrap_ui(UISrcs ${UI_FILES} )

add_definitions( -DmxCore_USE_SOURCE_CODE
    -DvmxCore_USE_SOURCE_CODE
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../mxCore
    ${PROJECT_SOURCE_DIR}/../vmxCore
)

set( ${APP_NAME}_SOURCES
    ${APP_NAME}.cpp
    ${PROJECT_SOURCE_DIR}/../vmxCore/vmxMenu.cpp
    ${PROJECT_SOURCE_DIR}/../vmxCore/vmxTextInput.cpp
    ${PROJECT_SOURCE_DIR}/../mxCore/mxString.cpp
)

set( ${APP_NAME}_HEADERS
    ${PROJECT_SOURCE_DIR}/../vmxCore/vmxClipBoard.h
    ${PROJECT_SOURCE_DIR}/../vmxCore/vmxMenu.h
    ${PROJECT_SOURCE_DIR}/../vmxCore/vmxTextInput.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxString.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxList.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxArray.h
)

set( ${APP_NAME}_LIBS
    mxCore
    vmxCore
)

set( MACOSX_RESOURCE_FILES ${CMAKE_SOURCE_DIR}/Files/Icons/mipx_icon_mipx.png )
set( MACOSX_BUNDLE_INFO_STRING "@EXE_NAME@ - Version ${VERSION}" )
set( MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION} )
set( MACOSX_BUNDLE_ICON_FILE "mipx_icon_mipx.png" )
set( MACOSX_BUNDLE_GUI_IDENTIFIER "com.sri.aic" )
set( MACOSX_BUNDLE_BUNDLE_NAME "@EXE_NAME@" )

add_executable(${APP_NAME} MACOSX_BUNDLE
    ${${APP_NAME}_SOURCES}
    ${${APP_NAME}_HEADERS}
    #${MOC_SOURCE_FILES}
    #${UI_SOURCE_FILES}
    #${RESOURCE_SOURCE_FILES}
    #${HEADER_FILES}
    ${MACOSX_RESOURCE_FILES}
)

qt5_use_modules(${APP_NAME} Core Gui)

target_link_libraries(${APP_NAME}
    ${VTK_LIBRARIES}
    #${APP_NAME}_LIBS
)

set_source_files_properties(
      ${MACOSX_RESOURCE_FILES}
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
)

install(TARGETS ${APP_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(CODE "
	include(BundleUtilities)
	fixup_bundle( \"${CMAKE_INSTALL_PREFIX}/bin/${APP_NAME}.app\" \"\" \"${CMAKE_INSTALL_PREFIX}/bin\" )
	" COMPONENT Runtime)
