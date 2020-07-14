cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME vmxApp )

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
    ${PROJECT_SOURCE_DIR}/../mxProcessing
    ${PROJECT_SOURCE_DIR}/../mxStandard
    ${PROJECT_SOURCE_DIR}/../vmxCore
    ${PROJECT_SOURCE_DIR}/../vmxGUI
)

set( ${TARGET_NAME}_SOURCES
    mxDataContainer.cpp
    mxFunctionConfig.cpp
    vmxAppDataLoader.cpp
    vmxAppFunctionWidget.cpp
    vmxAppHistogramViewer.cpp
    vmxAppImageViewer.cpp
    vmxAppOrthogonalImageViewer.cpp
    vmxAppProcessGraph.cpp
    vmxAppProcessImage.cpp
    vmxAppProcessPointList.cpp
    vmxAppProcessProfile.cpp
    vmxAppProcessSkeleton.cpp
    vmxAppReportWidget.cpp
    vmxAppSignalViewer.cpp
    vmxAppSkeletonSliceViewer.cpp
    vmxAppTableViewer.cpp
    vmxAppWidget.cpp
	${PROJECT_SOURCE_DIR}/../vmxGUI/tinyfiledialogs.c
)

set( ${TARGET_NAME}_HEADERS
    mxDataContainer.h
    mxFunctionConfig.h
    vmxAppDataLoader.h
    vmxAppFunctionInterface.h
    vmxAppFunctionWidget.h
    vmxAppHistogramViewer.h
    vmxAppImageViewer.h
    vmxAppOrthogonalImageViewer.h
    vmxAppProcessGraph.h
    vmxAppProcessImage.h
    vmxAppProcessPointList.h
    vmxAppProcessProfile.h
    vmxAppProcessSkeleton.h
    vmxAppReportWidget.h
    vmxAppSignalViewer.h
    vmxAppSkeletonSliceViewer.h
    vmxAppTableViewer.h
    vmxAppWidget.h
	${PROJECT_SOURCE_DIR}/../vmxGUI/tinyfiledialogs.h
)

set( ${TARGET_NAME}_LIBS
    mxCore
    mxProcessing
    mxStandard
    vmxCore
    vmxGUI
)

add_definitions( -DmxDataContainer_EXPORTS
    -DvmxAppDataLoader_EXPORTS
    -DmxFunctionConfig_EXPORTS
    -DvmxAppDataListWidget_EXPORTS
	-DvmxAppFunctionInterface_EXPORTS
    -DvmxAppFunctionWidget_EXPORTS
    -DvmxAppHistogramViewer_EXPORTS
    -DvmxAppImageViewer_EXPORTS
    -DvmxAppOrthogonalImageViewer_EXPORTS
    -DvmxAppSignalViewer_EXPORTS
    -DvmxAppSkeletonSliceViewer_EXPORTS
    -DvmxAppReportWidget_EXPORTS
    -DvmxAppTableViewer_EXPORTS
    -DvmxAppWidget_EXPORTS
)



option(MIPX_BUILD_LOADER_MATLAB "Build MATLAB file loader")
if(MIPX_BUILD_LOADER_MATLAB)
    include_directories( ${LIBMATIO_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/../xmxLoaderMatlab
    )
    set( ${TARGET_NAME}_LIBS
        ${${TARGET_NAME}_LIBS}
        ${LIBMATIO_LIBS}
        xmxLoaderMatlab
    )
endif()

option(MIPX_BUILD_LOADER_NIFTI "Build NIFTI file loader")
if(MIPX_BUILD_LOADER_NIFTI)
    include_directories( ${NIFTICLIB_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/../xmxLoaderNifti
    )
    set( ${TARGET_NAME}_LIBS
        ${${TARGET_NAME}_LIBS}
        ${NIFTICLIB_LIBS}
        xmxLoaderNifti
    )
endif()



option(MIPX_BUILD_LOADER_DICOM "Build DICOM file loader")
if(MIPX_BUILD_LOADER_DICOM)
    include_directories( ${GDCM_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/../xmxLoaderDicom
    )
    set( ${TARGET_NAME}_LIBS
        ${${TARGET_NAME}_LIBS}
        ${GDCM_LIBS}
        xmxLoaderDicom
    )
endif()




if(MIPX_USE_VTK_WITH_QT)
    add_library( ${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} ${UISrcs} ${QT_WRAP})
    qt5_use_modules(${TARGET_NAME} Core Gui)
else()
  add_library( ${TARGET_NAME} STATIC ${${TARGET_NAME}_SOURCES} ${${TARGET_NAME}_HEADERS} )
endif()


target_link_libraries(${TARGET_NAME}
    ${VTK_LIBRARIES}
    ${${TARGET_NAME}_LIBS}
)


install( FILES ${${TARGET_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
