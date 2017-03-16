cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0043 NEW)

set( LIB_NAME vmxCore )

PROJECT(LIB_NAME)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED QUIET)

qt5_wrap_ui(UISrcs ${UI_FILES} )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../mxCore
    ${PROJECT_SOURCE_DIR}/../vmxCore
)

set( ${LIB_NAME}_SOURCES
    vmxImage.cpp vmxInteractorStyleTrackballCamera.cpp vmxMenu.cpp vmxTextInput.cpp
)

set( ${LIB_NAME}_HEADERS
    vmxImage.h vmxInteractorStyleTrackballCamera.h vmxMenu.h vmxTextInput.h
)

set( ${LIB_NAME}_TEMPLATES
    ${PROJECT_SOURCE_DIR}/../mxCore/mxArray.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxList.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxMatrix.h
    ${PROJECT_SOURCE_DIR}/../mxCore/mxRegularGrid.h
)

add_definitions( -DvmxImage_EXPORTS
)

add_library( ${LIB_NAME} SHARED ${${LIB_NAME}_SOURCES} ${${LIB_NAME}_HEADERS} ${${LIB_NAME}_TEMPLATES} ${UISrcs} ${QT_WRAP})

set( ${LIB_NAME}_VTK_LIBS vtkalglib vtkChartsCore vtkCommonColor vtkCommonDataModel vtkCommonMath vtkCommonCore vtksys vtkCommonMisc vtkCommonSystem vtkCommonTransforms vtkInfovisCore vtkFiltersExtraction vtkCommonExecutionModel vtkFiltersCore vtkFiltersGeneral vtkCommonComputationalGeometry vtkFiltersStatistics vtkImagingFourier vtkImagingCore vtkRenderingContext2D vtkRenderingCore vtkFiltersGeometry vtkFiltersSources vtkIOImage vtkDICOMParser vtkIOCore vtkzlib vtkmetaio vtkjpeg vtkpng vtktiff vtkIOXMLParser vtkexpat vtkRenderingFreeType vtkfreetype vtkftgl vtkRenderingOpenGL vtkImagingHybrid vtkDomainsChemistry vtkIOXML vtkIOGeometry vtkjsoncpp vtkexoIIc vtkNetCDF vtkNetCDF_cxx vtkhdf5_hl vtkhdf5 vtkFiltersAMR vtkParallelCore vtkIOLegacy vtkFiltersFlowPaths vtkFiltersGeneric vtkFiltersHybrid vtkImagingSources vtkFiltersHyperTree vtkFiltersImaging vtkImagingGeneral vtkFiltersModeling vtkFiltersParallel vtkFiltersParallelImaging vtkFiltersProgrammable vtkFiltersSelection vtkFiltersTexture vtkFiltersVerdict verdict vtkGeovisCore vtkInfovisLayout vtkInteractionStyle vtkInteractionWidgets vtkRenderingAnnotation vtkImagingColor vtkRenderingVolume vtkViewsCore vtkproj4 vtkgl2ps vtkGUISupportQt vtkGUISupportQtOpenGL vtkImagingMath vtkImagingMorphological vtkImagingStatistics vtkImagingStencil vtkInteractionImage vtkIOAMR vtkIOEnSight vtkIOExodus vtkIOExport vtkRenderingGL2PS vtkIOImport vtkIOInfovis vtklibxml2 vtkIOLSDyna vtkIOMINC vtkIOMovie vtkoggtheora vtkIONetCDF vtkIOParallel vtkIOPLY vtkIOSQL vtksqlite vtkIOVideo vtkRenderingFreeTypeOpenGL vtkRenderingHybridOpenGL vtkRenderingImage vtkRenderingLabel vtkRenderingLOD vtkRenderingVolumeAMR vtkRenderingVolumeOpenGL vtkViewsContext2D vtkViewsGeovis vtkViewsInfovis
)

qt5_use_modules(${LIB_NAME} Core Gui)


##if(${VTK_VERSION} VERSION_GREATER "6")
#    target_link_libraries( ${LIB_NAME} ${${LIB_NAME}_VTK_LIBS} )#${VTK_LIBRARIES} )
##endif()

if(VTK_LIBRARIES)
  target_link_libraries(${LIB_NAME} ${VTK_LIBRARIES})
else()
  target_link_libraries(${LIB_NAME} vtkHybrid vtkWidgets)
endif()

target_link_libraries(${LIB_NAME} mxCore)

install( FILES ${${LIB_NAME}_HEADERS} ${${LIB_NAME}_TEMPLATES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include )
install( TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install( TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
