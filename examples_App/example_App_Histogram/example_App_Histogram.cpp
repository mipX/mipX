/*=========================================================================
 
 Program:   mipx
 Module:    example_App_Histogram.cpp
 
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
 
 This example shows the use histogram viewer to see the histogram of each slice in the image.
 
 */

#include "vmxImage.h"


#include "mxGeometry.h"
#include "mxBIP.h"
#include "mxGIP.h"

#include "vmxCurve.h"

#include "vmxAppDataLoader.h"
#include "vmxAppWidget.h"
#include "vmxAppImageViewer.h"
#include "vmxAppHistogramViewer.h"




#include <vtkMath.h>



class MainApp : public vmxAppMainWidget
{
public:
    
    // image viewer
    vmxAppImageViewer image_viewer;
    
    // histogram viewer
    vmxAppHistogramViewer histogram_viewer;
    
    // data loader.
    vmxAppDataLoader m_app_data_loader;
    
    
    MainApp()
    {
        // Set the data list widget to default.
        this->SetDataListWidgetToDefault();
        
        //// set the 3d renderer to default.
        //this->SetRenderer3DToDefaultTrackballCamera();
        
        // load the functions of the data loader
        this->LoadFunctionFactoryList(&m_app_data_loader);
        
        // register image viewer.
        this->RegisterViewer(&image_viewer);
        
        // Register the vmxImage16UFactory with the data list widget.
        // The widget will later use this factory to construct objects of the given type.
        this->GetDataListWidget()->AddFactory(new vmxImage16UFactory);
        
        
        // register histogram viewer
        this->RegisterViewer(&histogram_viewer);
        
        
        // Connect the histogram viewer and the image viewer to change the histogram plot when the displayed slice changes.
        vmxGUIConnection *connection = vmxGUIConnection::New(image_viewer.GetRenderer(), ImageSliceChangeEvent, histogram_viewer.GetRenderer(),
                                                             &histogram_viewer, vmxAppHistogramViewer::Slot_UpdateViewer);
        connection->SetPassedAddress((void**)image_viewer.GetImage());
        connection->SetPassedDataInt((int*)(&(image_viewer.GetRenderer()->m_index_time)),0);
        connection->SetPassedDataInt((int*)(&(image_viewer.GetRenderer()->m_index_slice)),1);

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


