/*=========================================================================
 
 Program:   mipx
 Module:    example_001_BasicVisualization.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt

 Licensed under the BSD License 2.0.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.

 =========================================================================*/


/*
 
 This example demonstrates the basic use of data list widget to construct data objects. The advantage is that the user can
 visualy manipulate the data using the data list widget.
 
 */




#include "vmxDataListWidget.h"
#include "vmxImage.h"
#include "vmxMesh.h"




int main()
{
    // Create the main widget. This is the first step that should be done in each application. The widget contains in itself
    // a render window, a renderer for GUI objects, a renderer for 3D data view, and the interactor.
    vmxGUIMainWidget main_widget;

    
    //----- Data List Widget -----
    vmxDataListWidget v_tree;
    v_tree.GetListWidget()->SetMainWidget(&main_widget);
    v_tree.GetListWidget()->SetColor(1,1,0);
    v_tree.GetListWidget()->SetFontSize(18);
    v_tree.GetListWidget()->SetVisibility(1);
    v_tree.GetListWidget()->SetPlacementToUpperRight();
    //----------
    
    
    // Register the vmxImage16UFactory with the data list widget.
    // The widget will later use this factory to construct objects of the given type.
    v_tree.AddFactory(new vmxImage16UFactory);
    v_tree.AddFactory(new vmxImage32UFactory);
    
    
    cout<<"Try 16U";
    vmxImage16U *vimg16 = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U","my_image16"));
    vimg16->SetDimensions(1,10,10,10);
    for(unsigned int i=0; i<vimg16->GetNumberOfDataElements(); i++)
    {
        vimg16->Set(i,i);
    }
    vimg16->SetVisibility(1);
    vimg16->SaveToVTK16UFile("/Users/danilobabin/-DIP_IMAGES/test_image_mipx.vtk");
    cout<<" OK ";

    // Create an image using the data list widget. The data list widget assigns to the image the
    // interactor of the main widget.
    vmxImage16U *vimg = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U","my_image"));
    vimg->SetMappingToOpaqueGrayScale(); //vimg->SetMappingToOpaqueColor();
    //vimg->LoadVTKFile("/Users/danilobabin/-DIP_IMAGES/-HEART/LAA/2016_12_23/dicom_set_grayscale.vtk");
    vimg->LoadVTKFile("/Users/danilobabin/-DIP_IMAGES/test_image_mipx.vtk");
    vimg->SetVisibility(1);
    
    
    cout<<"Try 32U";
    vmxImage32U *vimg32 = static_cast<vmxImage32U*> (v_tree.Create("vmxImage32U","my_image32"));
    vimg32->SetDimensions(1,10,10,10);
    vimg32->FillInWith(5);
    vimg32->SetVisibility(1);
    cout<<" OK ";
    
    
    
//    vmxImage16U *vimg2 = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U","my_image2"));
//    //vimg2->SetRenderer(main_widget.GetRenderer_3D());
//    mxDataObject *data_object = static_cast<mxDataObject*>(vimg);
//    vimg2->CopyFromDataObject(data_object);
//    //vimg2->Copy(vimg);
//    vimg2->SetMappingToOpaqueColor();
//    vimg2->SetVisibility(1);
//    cout<<vimg2->GetSpacing_S()<<","<<vimg2->GetSpacing_R()<<","<<vimg2->GetSpacing_C()<<endl;
    
    
    
    
    // Register the vmxMeshFactory with the data list widget.
    // The widget will later use this factory to construct objects of the given type.
    v_tree.AddFactory(new vmxMeshFactory);

    // Create a mesh using the data list widget.
    vmxMesh *mesh = static_cast<vmxMesh*> (v_tree.Create("vmxMesh","my_mesh"));
    //mesh->CreatePolyData(vimg, 50);
    mesh->CreatePolyData(vimg16, 500);
    mesh->CreateActorByLookupTableScalarColoring(0,vimg16->GetDimension_C()*vimg16->GetDimension_R()*vimg16->GetDimension_S());
    mesh->SetVisibility(1);
    //mesh->SetColor(1,0,0);
    //mesh->SaveToSTLFiles("/Users/danilobabin/-DIP_IMAGES/test_mesh.stl");
    
    


    
   
    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
    main_widget.StartInteraction();


    
    return 1;
}







