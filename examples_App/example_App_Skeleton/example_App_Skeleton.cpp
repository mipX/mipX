/*=========================================================================
 
 Program:   mipx
 Module:    example_App_Skeleton.cpp
 
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
 
 The example shows skeleton loading and visualization.
 
 */


#include "mxBIP.h"
#include "mxGeometry.h"
#include "mxGraph.h"
#include "mxGraphProcessing.h"
#include "mxSkeletonization.h"
#include "mxSkeleton.h"

#include "vmxGraph.h"
#include "vmxImage.h"
#include "vmxMesh.h"
#include "vmxSkeleton.h"

#include "vmxAppWidget.h"



int main()
{
    // Create the main widget. This is the first step that should be done in each application. The widget contains in itself
    // a render window, a renderer for GUI objects, a renderer for 3D data view, and the interactor.
    vmxAppMainWidget main_widget;

    // Set up the main widget to use its default structures.
    main_widget.SetDataListWidgetToDefault();
    main_widget.SetRenderer3DToDefaultTrackballCamera();
    
    vmxAppDataListWidget *v_tree = main_widget.GetDataListWidget();
    
    
    // Register the vmxImage16UFactory with the data list widget.
    // The widget will later use this factory to construct objects of the given type.
    v_tree->AddFactory(new vmxImage16UFactory);
    
    v_tree->AddFactory(new vmxSkeletonFactory);
    
    
    // Create an image using the data list widget. The data list widget assigns to the image the
    // interactor of the main widget.
    vmxSkeleton *skl = static_cast<vmxSkeleton*> (v_tree->Create("vmxSkeleton","brain blood vessels"));
    mxString file_name;
    file_name.Assign(MIPX_FILES_DATA_PATH);
    file_name.Append("brain_vessels_radii.skl");
    skl->LoadFromTextFileOfPositions(file_name.Get_C_String());
    skl->Update();
    
    std::cout<<std::endl<<"skl->GetNumberOfLinks()="<<skl->GetNumberOfLinks()<<", skl->GetNumberOfNodes()="<<skl->GetNumberOfNodes()<<std::endl;
    
   
    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
    main_widget.StartInteraction();


    
    return 1;
}







