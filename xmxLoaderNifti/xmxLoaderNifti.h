/*=========================================================================
 
 Program:   mipx
 Module:    xmxLoaderNifti.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(xmxLoaderNifti_USE_SOURCE_CODE)
    #define xmxLoaderNifti_API
#else
    #if defined(_MSC_VER)
        #ifdef xmxLoaderNifti_EXPORTS
            #define xmxLoaderNifti_API __declspec(dllexport)
        #else
            #define xmxLoaderNifti_API __declspec(dllimport)
        #endif
    #else
        #ifdef xmxLoaderNifti_EXPORTS
            #define xmxLoaderNifti_API __attribute__((visibility("default")))
        #else
            #define xmxLoaderNifti_API
        #endif
    #endif
#endif



#ifndef xmxLoaderNifti_H
    #define xmxLoaderNifti_H



//#include "mxArray.h"
//#include "mxDataObject.h"
//#include "mxString.h"

#include "mxImage.h"

#include "nifti1.h"
#include "nifti1_io.h"
////#include "nifti2.h"
////#include "nifti2_io.h"
#include "nifticdf.h"
#include "nifti_tool.h"
#include "znzlib.h"

//#include <strstream>
//#include <fstream>
//#include <iomanip>


/// 2D curve (x and y=f(x) values) as pair of arrays of x and corresponding y values.

class xmxLoaderNifti_API xmxLoaderNifti //: public mxDataObject
{
    
protected:
    
//    /// Label for x axis (describes what the x values represent).
//    mxString m_label_for_X_axis;
//
//    /// Label for y axis (describes what the y values represent).
//    mxString m_label_for_Y_axis;
//
//    /// Array of x values.
//    mxArray< double > m_X_values;
//
//    /// Array of y = f(x) values.
//    mxArray< double > m_Y_values;

    

public:

	/// Constructor.
	xmxLoaderNifti();

	/// Destructor.
	virtual ~xmxLoaderNifti();
    
    /// Load an image from file.
    int LoadImage(const char *file_name, mxImage *image);
    
//    
//    /// Load curve from a Matlab M file.
//    int LoadMatlabFile(const char *file_name);
//
//    /// Reset (empty the x and y arrays).
//    virtual void Reset();
//    
//    /// Save to a Matlab M file.
//    void SaveToMatlabFile(const char *file_name);
//    
    
};


#endif
