/*=========================================================================
 
 Program:   mipx
 Module:    vmxImagePlaneWidget.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/


// For VTK OpenGL errors see http://www.vtk.org/Wiki/VTK/OpenGL_Errors
// When transitioning existing code and applications to VTK 6.1 you may see reports for previously
// undetected OpenGL errors in your codes or ctest output which will need to be cleaned up.
// Apple Mac OSX
// On the Mac if you ask VTK to render into an "invalid drawable" you will find that all subsequent OpenGL
// calls fail (until the drawable becomes valid) with INVALID_FRAMEBUFFER_OPERATION reported. The "invalid drawable"
// issue is not new (http://vtk.markmail.org/search/?q=invalid%20drawable). However, until now one could ignore this
// as OpenGL errors were not detected and reported. Going forward you will need to fix your code or disable new gl
// error detection and reporting. There are three approaches that could be taken:
// - Restructure your code so that windows are visible prior to VTK rendering. This is the recommended approach.
// - Make use of the new vtkRenderWindow::IsDrawable method to detect invalid drawable and avoid asking VTK to render
// until the drawable is valid. This is useful if one can't ensure window visibility for some reason.
// - As a last resort, you may disable OpenGL error detection via the VTK_REPORT_OPENGL_ERRORS CMake variable, allowing you to ignore the bug.


#if defined(vmxImagePlaneWidget_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxImagePlaneWidget_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxImagePlaneWidget_EXPORTS
            #define vmxImagePlaneWidget_API __declspec(dllexport)
        #else
            #define vmxImagePlaneWidget_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxImagePlaneWidget_EXPORTS
            #define vmxImagePlaneWidget_API __attribute__((visibility("default")))
        #else
            #define vmxImagePlaneWidget_API
        #endif
    #endif
#endif



#ifndef vmxImagePlaneWidget_H
	#define vmxImagePlaneWidget_H



#include <chrono>
#include <vtkImagePlaneWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObserverMediator.h>


/// VTK level image plane widget for multi-plane reconstruction.

class vmxImagePlaneWidget_API vmxImagePlaneWidget : public vtkImagePlaneWidget
{
    
protected:
    
    // for detecting double click.
    int m_number_of_clicks;
    
    // for detecting double click.
    int m_left_button_down_previous_position[2];
    
    /// Indicator that will be set when the double click is detected.
    int m_is_double_clicked;
    
    using clock             = std::chrono::system_clock;
    using time_point_type   = std::chrono::time_point < clock, std::chrono::milliseconds > ;
    
    time_point_type m_time_of_left_click_in_ms, m_time_of_left_click_in_ms2;


    
public:
    
	/// Constructor.
	vmxImagePlaneWidget();

	/// Destructor.
	~vmxImagePlaneWidget();
    
    /// Create new object instance.
    static vmxImagePlaneWidget* New();

    /// Re-implementation.
    void OnLeftButtonDown();


};




#endif
