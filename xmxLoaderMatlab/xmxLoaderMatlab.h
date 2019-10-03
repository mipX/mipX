/*=========================================================================
 
 Program:   mipx
 Module:    xmxLoaderMatlab.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(xmxLoaderMatlab_USE_SOURCE_CODE)
    #define xmxLoaderMatlab_API
#else
    #if defined(_MSC_VER)
        #ifdef xmxLoaderMatlab_EXPORTS
            #define xmxLoaderMatlab_API __declspec(dllexport)
        #else
            #define xmxLoaderMatlab_API __declspec(dllimport)
        #endif
    #else
        #ifdef xmxLoaderMatlab_EXPORTS
            #define xmxLoaderMatlab_API __attribute__((visibility("default")))
        #else
            #define xmxLoaderMatlab_API
        #endif
    #endif
#endif



#ifndef xmxLoaderMatlab_H
    #define xmxLoaderMatlab_H



//#include "mxArray.h"
//#include "mxDataObject.h"
//#include "mxString.h"

#include "mxImage.h"

#include "matio.h"

//#include <strstream>
//#include <fstream>
//#include <iomanip>


/// 2D curve (x and y=f(x) values) as pair of arrays of x and corresponding y values.

class xmxLoaderMatlab_API xmxLoaderMatlab //: public mxDataObject
{
    
    

public:

	/// Constructor.
	xmxLoaderMatlab();

	/// Destructor.
	virtual ~xmxLoaderMatlab();
    
    /// Load an image from file.
    int LoadImage(const char *file_name, const char *variable_name, mxImage *image);
    
    /// Save an image to file.
    int SaveImage(const char *file_name, mxImage *image, const char *variable_name="I");
    
};


#endif
