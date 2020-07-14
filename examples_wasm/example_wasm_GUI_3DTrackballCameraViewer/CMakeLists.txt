cmake_minimum_required(VERSION 3.12)

cmake_policy(SET CMP0043 NEW)

set( TARGET_NAME  example_wasm_GUI_3DTrackballCameraViewer)

project(${TARGET_NAME})

# -----------------------------------------------------------------------------
# EMSCRIPTEN only
# -----------------------------------------------------------------------------

if (NOT EMSCRIPTEN)
  message("Skipping example: This needs to run inside an Emscripten build environment")
  return ()
endif ()


# -----------------------------------------------------------------------------
# Handle VTK dependency
# -----------------------------------------------------------------------------

#find_package(VTK
  #COMPONENTS
    #FiltersSources      # VTK pipeline
    #InteractionStyle    # Mouse handling
    #RenderingOpenGL2    # For Rendering
    #RenderingUI         # For SDL2 Window
#)
find_package(VTK)
include(${VTK_USE_FILE})

if (NOT VTK_FOUND)
  message("Skipping example: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif ()

# -----------------------------------------------------------------------------
# WebAssembly build options
# -----------------------------------------------------------------------------

set(emscripten_options)
list(APPEND emscripten_options
  "--bind"
  "-g3"
  "SHELL:-s EXPORT_NAME=vtkApp"
  "SHELL:-s ALLOW_MEMORY_GROWTH=0"
  "SHELL:-s MAXIMUM_MEMORY=16GB"
  "SHELL:-s DEMANGLE_SUPPORT=1"
  "SHELL:-s EMULATE_FUNCTION_POINTER_CASTS=0"
  "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
  "SHELL:-s MODULARIZE=1"
  "SHELL:-s USE_PTHREADS=0"
  "SHELL:-s WASM=1"
  "SHELL:-s ASSERTIONS=1"
  "SHELL:-s SAFE_HEAP=1"
)


# -----------------------------------------------------------------------------
# Build options
# -----------------------------------------------------------------------------

set(OPTIMIZE "NO_OPTIMIZATION" CACHE STRING "Emscripten optimization")
set_property(CACHE OPTIMIZE PROPERTY
  STRINGS
    NO_OPTIMIZATION       # -O0
    LITTLE                # -O1
    MORE                  # -O2
    BEST                  # -O3
    SMALL                 # -Os
    SMALLEST              # -Oz
    SMALLEST_WITH_CLOSURE # -Oz --closure 1
)

if(OPTIMIZE STREQUAL "NO_OPTIMIZATION")
  # Cone.js    659K
  # Cone.wasm  4.9M
  # time => 4 minutes 3 seconds
  list(APPEND emscripten_options
    "-Oz"
  )
elseif(OPTIMIZE STREQUAL "LITTLE")
  # Cone.js    529K
  # Cone.wasm  5.9M
  list(APPEND emscripten_options
    "-O1"
  )
elseif(OPTIMIZE STREQUAL "MORE")
  # Cone.js    529K
  # Cone.wasm  5.3M
  list(APPEND emscripten_options
    "-O2"
  )
elseif(OPTIMIZE STREQUAL "BEST")
  # Cone.js    529K
  # Cone.wasm  4.9M
  # time => 4 minutes 7 seconds
  list(APPEND emscripten_options
    "-O3"
  )
elseif(OPTIMIZE STREQUAL "SMALL")
  # Cone.js    529K
  # Cone.wasm  4.9M
  list(APPEND emscripten_options
    "-Os"
  )
elseif(OPTIMIZE STREQUAL "SMALLEST")
  # Cone.js    659K
  # Cone.wasm  4.9M
  list(APPEND emscripten_options
    "-Oz"
  )
elseif(OPTIMIZE STREQUAL "SMALLEST_WITH_CLOSURE")
  # Cone.js    659K
  # Cone.wasm  4.9M
  list(APPEND emscripten_options
    "-Oz"
    "SHELL:--closure 1"
  )
endif()


# -----------------------------------------------------------------------------
# Compile example code
# -----------------------------------------------------------------------------

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../../mxCore
    ${PROJECT_SOURCE_DIR}/../../mxStandard
    #${PROJECT_SOURCE_DIR}/../../vmxCore
    ${PROJECT_SOURCE_DIR}/../../vmxGUI
)

set( ${TARGET_NAME}_SOURCES
    ${TARGET_NAME}.cpp
)

set( ${TARGET_NAME}_HEADERS
)

set( ${TARGET_NAME}_LIBS
    mxCore
    mxStandard
    vmxGUI
)

add_executable(${TARGET_NAME} MACOSX_BUNDLE
    ${${TARGET_NAME}_SOURCES}
    ${${TARGET_NAME}_HEADERS}
    #${MOC_SOURCE_FILES}
    #${UI_SOURCE_FILES}
    #${RESOURCE_SOURCE_FILES}
    #${HEADER_FILES}
    ${MACOSX_RESOURCE_FILES}
)

#add_executable(tiny 
  #tiny.cpp 
  #mxImage.h
  #mxImage.cpp 
  #mxArray.h
  #mxMatrix.h
  #mxRegularGrid.h
  ##mxBasicImage.cpp 
  #mxObject.h
  #mxObject.cpp
  #mxDataObject.cpp 
  #mxDataObject.h 
  #mxImageSliceTag.cpp 
  #mxImageSliceTag.h
  ##mxRegularGrid.h
  #mxString.cpp
  #mxList.h
  #mxPosition.h
  #mxPosition.cpp
  #mxPoint.h
  #mxPoint.cpp
  #)

target_link_libraries(${TARGET_NAME}
    ${VTK_LIBRARIES}
    ${${TARGET_NAME}_LIBS}
)

#target_link_libraries(tiny
  #PRIVATE
    #VTK::FiltersSources
    #VTK::InteractionStyle
    #VTK::RenderingOpenGL2
    #VTK::RenderingUI
#)

target_compile_options(${TARGET_NAME}
  PUBLIC
    ${emscripten_options}
)

target_link_options(${TARGET_NAME}
  PUBLIC
    ${emscripten_options}
)

# -----------------------------------------------------------------------------
# VTK modules initialization
# -----------------------------------------------------------------------------

vtk_module_autoinit(
  TARGETS  ${TARGET_NAME}
  MODULES  ${VTK_LIBRARIES}
)

# -----------------------------------------------------------------------------
# Copy HTML to build directory
# -----------------------------------------------------------------------------

add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_SOURCE_DIR}/index.html"
      $<TARGET_FILE_DIR:${TARGET_NAME}>
)

#################   STARI CMAKE
#set(src 
   #tiny.cpp
 #)

#set(exports 
   #_main
 #)

## process exported functions
#set(exports_string "")
#list(JOIN exports "," exports_string)

## set compiler and flags
#SET(CMAKE_C_COMPILER emcc)
#SET(CMAKE_CPP_COMPILER em++)
#set( CMAKE_CXX_FLAGS "-s EXPORTED_FUNCTIONS=\"[${exports_string}]\""  )

## specify the project
#add_executable(${project}.html ${src})
