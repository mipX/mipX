# Generated by YCM Generator at 2019-10-25 06:02:08.316079

# This file is NOT licensed under the GPLv3, which is the license for the rest
# of YouCompleteMe.
#
# Here's the license text for this file:
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>

import os
import ycm_core

flags = [
    '-x',
    'c++',
    '-DMIPX_FILES_DATA_PATH="/home/levara/versioned/mipx/new/Files/Data/"',
    '-DMIPX_FILES_ICONS_PATH="/home/levara/versioned/mipx/new/Files/Icons/"',
    '-DMIPX_FILES_XTK_PATH="/home/levara/versioned/mipx/new/Files/XTK/"',
    '-DQT_CORE_LIB',
    '-DQT_GUI_LIB',
    '-DQT_NO_DEBUG',
    '-DQT_SQL_LIB',
    '-DQT_WIDGETS_LIB',
    '-Dkiss_fft_scalar=double',
    '-DmxBIP_EXPORTS',
    '-DmxCore_EXPORTS',
    '-DmxCurveProcessing_EXPORTS',
    '-DmxCurve_EXPORTS',
    '-DmxDataContainer_EXPORTS',
    '-DmxDataObjectTree_EXPORTS',
    '-DmxDataObject_EXPORTS',
    '-DmxFunctionConfig_EXPORTS',
    '-DmxFunctionObjectProgressIndicator_EXPORTS',
    '-DmxFunctionObject_EXPORTS',
    '-DmxGIP_EXPORTS',
    '-DmxGeometry_EXPORTS',
    '-DmxGraphProcessing_EXPORTS',
    '-DmxGraph_EXPORTS',
    '-DmxImageSliceTag_EXPORTS',
    '-DmxImage_EXPORTS',
    '-DmxIndex_EXPORTS',
    '-DmxObject_EXPORTS',
    '-DmxPointList_EXPORTS',
    '-DmxPoint_EXPORTS',
    '-DmxPosition_EXPORTS',
    '-DmxProcessing_EXPORTS',
    '-DmxProfileProcessing_EXPORTS',
    '-DmxProfile_EXPORTS',
    '-DmxSkeleton_EXPORTS',
    '-DmxSkeletonization_EXPORTS',
    '-DmxStandard_EXPORTS',
    '-DmxString_EXPORTS',
    '-DmxTable_EXPORTS',
    '-DmxVectorProcessing_EXPORTS',
    '-DmxVoxel_EXPORTS',
    '-DvmxAppDataListWidget_EXPORTS',
    '-DvmxAppDataLoader_EXPORTS',
    '-DvmxAppFunctionWidget_EXPORTS',
    '-DvmxAppHistogramViewer_EXPORTS',
    '-DvmxAppImageViewer_EXPORTS',
    '-DvmxAppOrthogonalImageViewer_EXPORTS',
    '-DvmxAppReportWidget_EXPORTS',
    '-DvmxAppSignalViewer_EXPORTS',
    '-DvmxAppTableViewer_EXPORTS',
    '-DvmxAppWidget_EXPORTS',
    '-DvmxApp_EXPORTS',
    '-DvmxCore_EXPORTS',
    '-DvmxCurve_EXPORTS',
    '-DvmxGUIButtonGroup_EXPORTS',
    '-DvmxGUIClipBoard_EXPORTS',
    '-DvmxGUIEventDetector_EXPORTS',
    '-DvmxGUIFilesDialog_EXPORTS',
    '-DvmxGUIIconButtonGroup_EXPORTS',
    '-DvmxGUIInputWidget_EXPORTS',
    '-DvmxGUIInteractorStyle_EXPORTS',
    '-DvmxGUILabel_EXPORTS',
    '-DvmxGUIListWidget_EXPORTS',
    '-DvmxGUIMenuBar_EXPORTS',
    '-DvmxGUIMenu_EXPORTS',
    '-DvmxGUIRenderer3DTrackballCamera_EXPORTS',
    '-DvmxGUIRendererImageViewer_EXPORTS',
    '-DvmxGUIRendererSignalViewer_EXPORTS',
    '-DvmxGUIRendererTableViewer_EXPORTS',
    '-DvmxGUIWidget_EXPORTS',
    '-DvmxGUI_EXPORTS',
    '-DvmxGraph_EXPORTS',
    '-DvmxImageData_EXPORTS',
    '-DvmxImagePlaneWidget_EXPORTS',
    '-DvmxImagePlanes_EXPORTS',
    '-DvmxImage_EXPORTS',
    '-DvmxMesh_EXPORTS',
    '-DvmxPointList_EXPORTS',
    '-DvmxProfile_EXPORTS',
    '-DvmxSkeleton_EXPORTS',
    '-DvmxTable_EXPORTS',
    '-DvtkDomainsChemistry_AUTOINIT=2(vtkDomainsChemistryOpenGL2,vtkDomainsParallelChemistry)',
    '-DvtkFiltersCore_AUTOINIT=1(vtkFiltersParallelDIY2)',
    '-DvtkFiltersFlowPaths_AUTOINIT=1(vtkFiltersParallelFlowPaths)',
    '-DvtkFiltersParallel_AUTOINIT=2(vtkFiltersParallelDIY2,vtkFiltersParallelGeometry)',
    '-DvtkIOExodus_AUTOINIT=1(vtkIOParallelExodus)',
    '-DvtkIOExport_AUTOINIT=2(vtkIOExportOpenGL2,vtkIOExportPDF)',
    '-DvtkIOGeometry_AUTOINIT=1(vtkIOMPIParallel)',
    '-DvtkIOImage_AUTOINIT=1(vtkIOMPIImage)',
    '-DvtkIOParallel_AUTOINIT=1(vtkIOMPIParallel)',
    '-DvtkIOSQL_AUTOINIT=2(vtkIOMySQL,vtkIOPostgreSQL)',
    '-DvtkIOXdmf3_AUTOINIT=1(vtkIOParallelXdmf3)',
    '-DvtkRenderingContext2D_AUTOINIT=1(vtkRenderingContextOpenGL2)',
    '-DvtkRenderingCore_AUTOINIT=3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)',
    '-DvtkRenderingFreeType_AUTOINIT=2(vtkRenderingFreeTypeFontConfig,vtkRenderingMatplotlib)',
    '-DvtkRenderingLICOpenGL2_AUTOINIT=1(vtkRenderingParallelLIC)',
    '-DvtkRenderingOpenGL2_AUTOINIT=1(vtkRenderingGL2PSOpenGL2)',
    '-DvtkRenderingVolume_AUTOINIT=1(vtkRenderingVolumeOpenGL2)',
    '-I/home/levara/versioned/mipx/new',
    '-I/home/levara/versioned/mipx/new/Apps',
    '-I/home/levara/versioned/mipx/new/Apps/App_PWVPaPd',
    '-I/home/levara/versioned/mipx/new/examples_GUI',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_3DTrackballCameraViewer',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_3DTrackballCameraViewer/../../mxCore',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_3DTrackballCameraViewer/../../mxStandard',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_3DTrackballCameraViewer/../../vmxGUI',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_ImageViewer',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_ImageViewer/../../mxCore',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_ImageViewer/../../mxStandard',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_ImageViewer/../../vmxGUI',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_OrthogonalImageViewer',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_OrthogonalImageViewer/../../mxCore',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_OrthogonalImageViewer/../../mxStandard',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_OrthogonalImageViewer/../../vmxGUI',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_OverlayImageViewer',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_OverlayImageViewer/../../mxCore',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_OverlayImageViewer/../../mxStandard',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_OverlayImageViewer/../../vmxGUI',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_ParallelImageViewer',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_ParallelImageViewer/../../mxCore',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_ParallelImageViewer/../../mxStandard',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_ParallelImageViewer/../../vmxGUI',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_SignalViewer',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_SignalViewer/../../mxCore',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_SignalViewer/../../mxStandard',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_SignalViewer/../../vmxGUI',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_TableViewer',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_TableViewer/../../mxCore',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_TableViewer/../../mxStandard',
    '-I/home/levara/versioned/mipx/new/examples_GUI/example_GUI_TableViewer/../../vmxGUI',
    '-I/home/levara/versioned/mipx/new/mxCore',
    '-I/home/levara/versioned/mipx/new/mxCore/../mxCore',
    '-I/home/levara/versioned/mipx/new/mxCore/../mxStandard',
    '-I/home/levara/versioned/mipx/new/mxProcessing',
    '-I/home/levara/versioned/mipx/new/mxProcessing/../mxCore',
    '-I/home/levara/versioned/mipx/new/mxProcessing/../mxStandard',
    '-I/home/levara/versioned/mipx/new/mxStandard',
    '-I/home/levara/versioned/mipx/new/mxStandard/../mxStandard',
    '-I/home/levara/versioned/mipx/new/vmxApp',
    '-I/home/levara/versioned/mipx/new/vmxApp/../mxCore',
    '-I/home/levara/versioned/mipx/new/vmxApp/../mxProcessing',
    '-I/home/levara/versioned/mipx/new/vmxApp/../mxStandard',
    '-I/home/levara/versioned/mipx/new/vmxApp/../vmxCore',
    '-I/home/levara/versioned/mipx/new/vmxApp/../vmxGUI',
    '-I/home/levara/versioned/mipx/new/vmxCore',
    '-I/home/levara/versioned/mipx/new/vmxCore/../mxCore',
    '-I/home/levara/versioned/mipx/new/vmxCore/../mxStandard',
    '-I/home/levara/versioned/mipx/new/vmxGUI',
    '-I/home/levara/versioned/mipx/new/vmxGUI/../mxStandard',
    '-I/home/levara/versioned/mipx/new/vmxGUI/../vmxGUI',
    '-I/tmp/tmpM10Fsi',
    '-I/tmp/tmpM10Fsi/Apps',
    '-I/tmp/tmpM10Fsi/Apps/App_PWVPaPd',
    '-I/tmp/tmpM10Fsi/examples_GUI',
    '-I/tmp/tmpM10Fsi/examples_GUI/example_GUI_3DTrackballCameraViewer',
    '-I/tmp/tmpM10Fsi/examples_GUI/example_GUI_ImageViewer',
    '-I/tmp/tmpM10Fsi/examples_GUI/example_GUI_OrthogonalImageViewer',
    '-I/tmp/tmpM10Fsi/examples_GUI/example_GUI_OverlayImageViewer',
    '-I/tmp/tmpM10Fsi/examples_GUI/example_GUI_ParallelImageViewer',
    '-I/tmp/tmpM10Fsi/examples_GUI/example_GUI_SignalViewer',
    '-I/tmp/tmpM10Fsi/examples_GUI/example_GUI_TableViewer',
    '-I/tmp/tmpM10Fsi/mxCore',
    '-I/tmp/tmpM10Fsi/mxProcessing',
    '-I/tmp/tmpM10Fsi/mxStandard',
    '-I/tmp/tmpM10Fsi/vmxApp',
    '-I/tmp/tmpM10Fsi/vmxCore',
    '-I/tmp/tmpM10Fsi/vmxGUI',
    '-I/usr/include/double-conversion',
    '-I/usr/include/eigen3',
    '-I/usr/include/freetype2',
    '-I/usr/include/libxml2',
    '-I/usr/include/python3.7m',
    '-isystem', '/usr/include/qt',
    '-isystem', '/usr/include/qt/QtCore',
    '-isystem', '/usr/include/qt/QtGui',
    '-isystem', '/usr/include/qt/QtSql',
    '-isystem', '/usr/include/qt/QtWidgets',
    '-isystem', '/usr/include/vtk',
    '-isystem', '/usr/lib/qt/mkspecs/linux-g++',
]


# Set this to the absolute path to the folder (NOT the file!) containing the
# compile_commands.json file to use that instead of 'flags'. See here for
# more details: http://clang.llvm.org/docs/JSONCompilationDatabase.html
#
# You can get CMake to generate this file for you by adding:
#   set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
# to your CMakeLists.txt file.
#
# Most projects will NOT need to set this to anything; you can just change the
# 'flags' list of compilation flags. Notice that YCM itself uses that approach.
compilation_database_folder = ''

if os.path.exists( compilation_database_folder ):
  database = ycm_core.CompilationDatabase( compilation_database_folder )
else:
  database = None

SOURCE_EXTENSIONS = [ '.C', '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )


def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return list( flags )
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith( '/' ):
        new_flag = os.path.join( working_directory, flag )

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = path_flag + os.path.join( working_directory, path )
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags


def IsHeaderFile( filename ):
  extension = os.path.splitext( filename )[ 1 ]
  return extension in [ '.H', '.h', '.hxx', '.hpp', '.hh' ]


def GetCompilationInfoForFile( filename ):
  # The compilation_commands.json file generated by CMake does not have entries
  # for header files. So we do our best by asking the db for flags for a
  # corresponding source file, if any. If one exists, the flags for that file
  # should be good enough.
  if IsHeaderFile( filename ):
    basename = os.path.splitext( filename )[ 0 ]
    for extension in SOURCE_EXTENSIONS:
      replacement_file = basename + extension
      if os.path.exists( replacement_file ):
        compilation_info = database.GetCompilationInfoForFile(
          replacement_file )
        if compilation_info.compiler_flags_:
          return compilation_info
    return None
  return database.GetCompilationInfoForFile( filename )


def FlagsForFile( filename, **kwargs ):
  if database:
    # Bear in mind that compilation_info.compiler_flags_ does NOT return a
    # python list, but a "list-like" StringVec object
    compilation_info = GetCompilationInfoForFile( filename )
    if not compilation_info:
      return None

    final_flags = MakeRelativePathsInFlagsAbsolute(
      compilation_info.compiler_flags_,
      compilation_info.compiler_working_dir_ )

  else:
    relative_to = DirectoryOfThisScript()
    final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to )

  return {
    'flags': final_flags,
    'do_cache': True
  }

