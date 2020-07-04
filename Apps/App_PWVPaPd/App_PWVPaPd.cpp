/*=========================================================================
 
 Program:   mipx
 Module:    App_PWVPaPd.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt

 Licensed under the BSD License 2.0.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.

 =========================================================================*/


/*
 
 This example shows the app to extract Ps and Pd pressure waves from screen grabs and calculate PWV (using premeasured distance).
 
 */


#include "mxBIP.h"
#include "mxGIP.h"
#include "mxGeometry.h"

#include "vmxCurve.h"

#include "vmxAppWidget.h"
#include "vmxAppImageViewer.h"
#include "vmxAppSignalViewer.h"
#include "vmxImage.h"

#include "vmxGUIButtonGroup.h"

#include <vtkMath.h>
#include <vtkPNGReader.h>
#include <vtkImageExtractComponents.h>


#include "PWVPaPd.h"


class MainApp : public vmxAppMainWidget
{
public:
    
    
    vmxAppImageViewer m_image_viewer1;
    vmxAppImageViewer m_image_viewer2;
    
    vmxAppSignalViewer m_signal_viewer;
    
    PWVPaPdFunctionList m_pwv_pa_pd_functions;
    
    
public:
    
    
    
    
    
    MainApp()
    {
        this->SetDataListWidgetToDefault();
        //this->SetRenderer3DToDefaultTrackballCamera();
        
        this->LoadFunctionFactoryList(&m_pwv_pa_pd_functions);
        

        
        vmxImage temp_image;
        //vmxImage *img1 = static_cast<vmxImage*> (
        this->GetDataListWidget()->Create(temp_image.GetClassName().Get_C_String(),"Image1");//);
        //vmxImage *img2 = static_cast<vmxImage*> (
        this->GetDataListWidget()->Create(temp_image.GetClassName().Get_C_String(),"Image2");//);

        
        m_image_viewer1.SetObjectName("ImageViewer1");
        this->RegisterViewer(&m_image_viewer1);
        
        
        m_image_viewer2.SetObjectName("ImageViewer2");
        this->RegisterViewer(&m_image_viewer2);

        
        this->RegisterViewer(&m_signal_viewer);

        
        
    };
    
    
    ~MainApp(){};
    
    
    
    
};





int main()
{
    
    // Create the main widget. This is the first step that should be done in each application. The widget contains in itself
    // a render window, a renderer for GUI objects, a renderer for 3D data view, and the interactor.
    MainApp main_widget;
    
    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
    main_widget.StartInteraction();
    
    return 1;
}

