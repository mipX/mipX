/*=========================================================================
 
 Program:   mipx
 Module:    App_Laa.cpp
 
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
#include "vmxAppDataLoader.h"

#include "vmxGUIButtonGroup.h"

#include <vtkMath.h>
#include <vtkPNGReader.h>
#include <vtkImageExtractComponents.h>


#include "LaaUI.h"


class MainApp : public vmxAppMainWidget
{
public:
    
//    mxScopedPointer< vmxImage > image;
//    mxScopedPointer< vmxImage > image2;
    
    
    //vmxAppImageViewer m_image_viewer1;
    //vmxAppImageViewer m_image_viewer2;
    
    
    LaaUIFunctionList m_laa_ui_functions;

    vmxAppDataLoader m_app_data_loader;
    
    
    
    
public:
    
    
    MainApp()
    {

        // Set the data list widget to default.
        this->SetDataListWidgetToDefault();
        
        // Set the 3D renderer to default.
        this->SetRenderer3DToDefaultTrackballCamera();
        
        // Use data loader
        this->LoadFunctionFactoryList(&m_app_data_loader);
        
        // Load custom made image processing functions.
        this->LoadFunctionFactoryList(&m_laa_ui_functions);
        

        

        
        
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

