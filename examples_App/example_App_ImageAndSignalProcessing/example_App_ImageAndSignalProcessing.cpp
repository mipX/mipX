/*=========================================================================
 
 Program:   mipx
 Module:    example_App_ImageAndSignalProcessing.cpp
 
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
 
 This example shows how to write new functions for image processing.
 It also provides viewing of signals.
 
 */


#include "mxGraphProcessing.h"
#include "mxBIP.h"
#include "mxGIP.h"
#include "mxProfileProcessing.h"


#include "vmxCurve.h"
#include "vmxGraph.h"
#include "vmxImage.h"
#include "vmxMesh.h"
#include "vmxPointList.h"
#include "vmxProfile.h"

#include "vmxGUILabel.h"
#include "vmxGUIButtonGroup.h"

#include "vmxAppWidget.h"
#include "vmxAppImageViewer.h"
#include "vmxAppSignalViewer.h"
#include "vmxAppDataLoader.h"


#include <vtkMath.h>


#include "ImageProcessing.h"







class MainApp : public vmxAppMainWidget
{
public:
    
    vmxAppSignalViewer signal_viewer;
    
    // Custom made image processing.
    ImageProcessingFunctionList image_processing_function_list;
    
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
        this->LoadFunctionFactoryList(&image_processing_function_list);
        
        // register signal viewer.
        this->RegisterViewer(&signal_viewer);
        
        
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

