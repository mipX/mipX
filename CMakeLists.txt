
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

PROJECT(MIPX)

include (InstallRequiredSystemLibraries)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set( MIPX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set( MIPX_HEADER_DIRS
    ${MIPX_SOURCE_DIR}/mxCore
    ${MIPX_SOURCE_DIR}/mxStandard
    ${MIPX_SOURCE_DIR}/mxProcessing
    #${MIPX_SOURCE_DIR}/vmxApp
    #${MIPX_SOURCE_DIR}/vmxCore
    #${MIPX_SOURCE_DIR}/vmxGUI
)

set( MIPX_LIBS
    mxCore
    mxStandard
    mxProcessing
    #vmxApp
    #vmxCore
    #vmxGUI
)


option(MIPX_BUILD_LOADER_NIFTI "Build NIFTI file loader")
if(MIPX_BUILD_LOADER_NIFTI)

    set(NIFTICLIB_INCLUDE_DIR "" CACHE PATH "Path to nifticlib include files")
    set(NIFTICLIB_LIB_DIR "" CACHE PATH "Path to nifticlib lib directory")

    if(APPLE)
        set( NIFTICLIB_LIBS
            ${NIFTICLIB_LIB_DIR}/libnifticdf.dylib
            #${NIFTICLIB_LIB_DIR}/libnifticdf.2.dylib
            #${NIFTICLIB_LIB_DIR}/libnifticdf.2.0.0.dylib
            ${NIFTICLIB_LIB_DIR}/libniftiio.dylib
            #${NIFTICLIB_LIB_DIR}/libniftiio.2.dylib
            #${NIFTICLIB_LIB_DIR}/libniftiio.2.0.0.dylib
            ${NIFTICLIB_LIB_DIR}/libznz.dylib
            #${NIFTICLIB_LIB_DIR}/libznz.2.dylib
            #${NIFTICLIB_LIB_DIR}/libznz.2.0.0.dylib
        )
    endif()
	if(WIN32)
        set( NIFTICLIB_LIBS
            ${NIFTICLIB_LIB_DIR}/nifticdf.lib
            ${NIFTICLIB_LIB_DIR}/nifticdf.2.lib
            ${NIFTICLIB_LIB_DIR}/niftiio.lib
            ${NIFTICLIB_LIB_DIR}/niftiio.2.lib
            ${NIFTICLIB_LIB_DIR}/znz.lib
            ${NIFTICLIB_LIB_DIR}/znz.2.lib
        )
    endif()

    set( MIPX_HEADER_DIRS
        ${MIPX_HEADER_DIRS}
        ${NIFTICLIB_INCLUDE_DIR}
        ${MIPX_SOURCE_DIR}/xmxLoaderNifti
    )

    set( MIPX_LIBS
        ${MIPX_LIBS}
        ${NIFTICLIB_LIBS}
        xmxLoaderNifti
    )

    add_definitions(-DMIPX_BUILD_LOADER_NIFTI)

endif()


option(MIPX_BUILD_LOADER_MATLAB "Build MATLAB file loader")
if(MIPX_BUILD_LOADER_MATLAB)

    set(LIBMATIO_INCLUDE_DIR "" CACHE PATH "Path to libmatio include files")
    set(LIBMATIO_LIB_DIR "" CACHE PATH "Path to libmatio lib directory")

    if(APPLE)
        set( LIBMATIO_LIBS
            ${LIBMATIO_LIB_DIR}/libmatio.dylib
        )
    endif()
	if(WIN32)
        set( LIBMATIO_LIBS
            ${LIBMATIO_LIB_DIR}/matio.lib
        )
    endif()

    set( MIPX_HEADER_DIRS
        ${MIPX_HEADER_DIRS}
        ${LIBMATIO_INCLUDE_DIR}
        ${MIPX_SOURCE_DIR}/xmxLoaderMatlab
    )

    set( MIPX_LIBS
        ${MIPX_LIBS}
        ${LIBMATIO_LIBS}
        xmxLoaderMatlab
    )

    add_definitions(-DMIPX_BUILD_LOADER_MATLAB)

endif()


option(MIPX_BUILD_LOADER_DICOM "Build DICOM file loader")
if(MIPX_BUILD_LOADER_DICOM)

    set(GDCM_INCLUDE_DIR "" CACHE PATH "Path to GDCM include files")
    set(GDCM_LIB_DIR "" CACHE PATH "Path to GDCM lib directory")

    if(APPLE)
        set( GDCM_LIBS
            ${GDCM_LIB_DIR}/libgdcmcharls.dylib
            ${GDCM_LIB_DIR}/libgdcmCommon.dylib
            ${GDCM_LIB_DIR}/libgdcmDICT.dylib
            ${GDCM_LIB_DIR}/libgdcmDSED.dylib
            ${GDCM_LIB_DIR}/libgdcmexpat.dylib
            ${GDCM_LIB_DIR}/libgdcmIOD.dylib
            ${GDCM_LIB_DIR}/libgdcmjpeg8.dylib
            ${GDCM_LIB_DIR}/libgdcmjpeg12.dylib
            ${GDCM_LIB_DIR}/libgdcmjpeg16.dylib
            ${GDCM_LIB_DIR}/libgdcmMEXD.dylib
            ${GDCM_LIB_DIR}/libgdcmMSFF.dylib
            #${GDCM_LIB_DIR}/libgdcmopenjpeg.dylib
            ${GDCM_LIB_DIR}/libgdcmuuid.dylib
            ${GDCM_LIB_DIR}/libgdcmzlib.dylib
            ${GDCM_LIB_DIR}/libsocketxx.dylib
            ${GDCM_LIB_DIR}/libvtkgdcm.dylib
            #${GDCM_LIB_DIR}/libvtkgdcmPythonD.dylib
        )
    endif()
	if(WIN32)
        set( GDCM_LIBS
            ${GDCM_LIB_DIR}/gdcmcharls.lib
            ${GDCM_LIB_DIR}/gdcmCommon.lib
            ${GDCM_LIB_DIR}/gdcmDICT.lib
            ${GDCM_LIB_DIR}/gdcmDSED.lib
            ${GDCM_LIB_DIR}/gdcmexpat.lib
			${GDCM_LIB_DIR}/gdcmgetopt.lib
            ${GDCM_LIB_DIR}/gdcmIOD.lib
            ${GDCM_LIB_DIR}/gdcmjpeg8.lib
            ${GDCM_LIB_DIR}/gdcmjpeg12.lib
            ${GDCM_LIB_DIR}/gdcmjpeg16.lib
            ${GDCM_LIB_DIR}/gdcmMEXD.lib
            ${GDCM_LIB_DIR}/gdcmMSFF.lib
            #${GDCM_LIB_DIR}/gdcmopenjpeg.lib
            ${GDCM_LIB_DIR}/gdcmuuid.lib
            ${GDCM_LIB_DIR}/gdcmzlib.lib
            ${GDCM_LIB_DIR}/socketxx.lib
            ${GDCM_LIB_DIR}/vtkgdcm.lib
            #${GDCM_LIB_DIR}/libvtkgdcmPythonD.lib
        )
    endif()

    set( MIPX_HEADER_DIRS
        ${MIPX_HEADER_DIRS}
        ${GDCM_INCLUDE_DIR}
        ${MIPX_SOURCE_DIR}/xmxLoaderDicom
    )

    set( MIPX_LIBS
        ${MIPX_LIBS}
        ${GDCM_LIBS}
        xmxLoaderDicom
    )

    add_definitions(-DMIPX_BUILD_LOADER_DICOM)

endif()



add_definitions(-DMIPX_FILES_DATA_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/Files/Data/\")
add_definitions(-DMIPX_FILES_XTK_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/Files/XTK/\")
add_definitions(-DMIPX_FILES_ICONS_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/Files/Icons/\")


add_subdirectory( mxStandard )
add_subdirectory( mxCore )
add_subdirectory( mxProcessing )

#add_subdirectory( vmxApp )
#add_subdirectory( vmxCore )
#add_subdirectory( vmxGUI )

if(MIPX_BUILD_LOADER_DICOM)
    add_subdirectory( xmxLoaderDicom )
endif()

if(MIPX_BUILD_LOADER_NIFTI)
    add_subdirectory( xmxLoaderNifti )
endif()

if(MIPX_BUILD_LOADER_MATLAB)
    add_subdirectory( xmxLoaderMatlab )
endif()

#add_subdirectory( examples_App )
#add_subdirectory( examples_GUI )
add_subdirectory( examples_wasm )

#add_subdirectory( Apps )

#add_subdirectory( aaApps )
#add_subdirectory( aaTests )
