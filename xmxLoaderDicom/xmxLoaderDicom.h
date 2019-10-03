/*=========================================================================
 
 Program:   mipx
 Module:    xmxLoaderDicom.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(xmxLoaderDicom_USE_SOURCE_CODE)
    #define xmxLoaderDicom_API
#else
    #if defined(_MSC_VER)
        #ifdef xmxLoaderDicom_EXPORTS
            #define xmxLoaderDicom_API __declspec(dllexport)
        #else
            #define xmxLoaderDicom_API __declspec(dllimport)
        #endif
    #else
        #ifdef xmxLoaderDicom_EXPORTS
            #define xmxLoaderDicom_API __attribute__((visibility("default")))
        #else
            #define xmxLoaderDicom_API
        #endif
    #endif
#endif



#ifndef xmxLoaderDicom_H
    #define xmxLoaderDicom_H



//#include "mxArray.h"
//#include "mxDataObject.h"
//#include "mxString.h"

#include "mxFileOperations.h"
#include "mxImage.h"
#include "mxImageSliceTag.h"

#include <gdcmTag.h>
#include <gdcmTrace.h>
#include <gdcmVR.h>
#include <gdcmReader.h>
#include <gdcmFileMetaInformation.h>
#include <gdcmFile.h>
#include <gdcmTesting.h>
#include <gdcmMediaStorage.h>
#include <gdcmFilename.h>
#include <gdcmTesting.h>
#include <gdcmSystem.h>
#include <gdcmTrace.h>
#include <gdcmDataElement.h>
#include <gdcmDataSet.h>
#include <gdcmDirectory.h>
#include <gdcmIPPSorter.h>
#include <gdcmFilenameGenerator.h>
#include <gdcmAttribute.h>
#include <gdcmImageReader.h>
#include <gdcmImageWriter.h>
#include <gdcmCSAHeader.h>
#include <gdcmGlobal.h>
#include <gdcmDicts.h>

#include "gdcmReader.h"
#include "gdcmPrivateTag.h"
#include "gdcmAttribute.h"
#include "gdcmImageWriter.h"

//#include <vtkGDCMImageReader2.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>

#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>

#include <DICOMAppHelper.h>
#include <DICOMParser.h>

#include "tinydir.h"



/// Dicom loader.

class xmxLoaderDicom_API xmxLoaderDicom
{

public:
    
    mxString m_path_directory;
    
    mxString m_mipx_path_directory;

	/// Constructor.
	xmxLoaderDicom();

	/// Destructor.
	virtual ~xmxLoaderDicom();
    
    /// List the contents of dicom directory tree into directory tree of text file lists.
    int DicomDirectoryToTextFileDirectoryTree(const char *directory_path);

    /// Load an image from file.
    int LoadSeriesFromDicomFileListFile(const char *file_name, mxImage *image);
    
};


#endif
