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


#include "mxBIP.h"
#include "mxGIP.h"

#include "vmxCurve.h"

#include "vmxAppWidget.h"
#include "vmxAppImageViewer.h"
#include "vmxAppHistogramViewer.h"
#include "vmxImage.h"

#include <vtkMath.h>




int main()
{
    
    // Create the main widget. This is the first step that should be done in each application. The widget contains in itself
    // a render window, a renderer for GUI objects, a renderer for 3D data view, and the interactor.
    vmxAppMainWidget main_widget;


    // Set up the main widget to use its default structures.
    main_widget.SetDataListWidgetToDefault();
    //main_widget.SetRenderer3DToDefaultTrackballCamera();


    // Register the vmxImage16UFactory with the data list widget.
    // The widget will later use this factory to construct objects of the given type.
    main_widget.GetDataListWidget()->AddFactory(new vmxImage16UFactory);


    // Create an image using the data list widget. The data list widget assigns to the image the
    // interactor of the main widget.
    vmxImage *image = static_cast<vmxImage*> (main_widget.GetDataListWidget()->Create("vmxImage16U","brain vessels"));
    
    image->LoadVTKFile("/Users/danilobabin/Code/MIPX_DATA/brain_vessels_avm_3DRA.vtk");

    //image->SetVisibilityOfComponent(0,1);

    vmxAppImageViewer image_viewer;
    main_widget.RegisterViewer(&image_viewer);
    //image_viewer.GetRenderer()->SetMappingToColor();


    vmxAppHistogramViewer histogram_viewer;
    main_widget.RegisterViewer(&histogram_viewer);

        
    
    vmxGUIConnection *connection = vmxGUIConnection::New(image_viewer.GetRenderer(), ImageSliceChangeEvent, histogram_viewer.GetRenderer(),
                                                         &histogram_viewer, vmxAppHistogramViewer::Slot_UpdateViewer);
    connection->SetPassedAddress((void**)image_viewer.GetImage());
    connection->SetPassedDataInt((int*)(&(image_viewer.GetRenderer()->m_index_slice)));

    
    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
    main_widget.StartInteraction();
    
    return 1;
}


