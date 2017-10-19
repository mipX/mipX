/*=========================================================================
 
 Program:   mipx
 Module:    example_002_Skeletonization.cpp
 
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
 
 The example shows the use of skeletonization and general image processing modules.
 
 This example demonstrates the basic use of data list widget to construct data objects. The advantage is that the user can
 visualy manipulate the data using the data list widget.
 
 */


#include "mxBIPX.h"
#include "mxGeometry.h"
#include "mxSkeletonizationX.h"
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
    
    unsigned int dimension_s = 100, dimension_r = 100, dimension_c = 100;
    
    
    cout<<"Try 16U";
    
    // Create an image using the data list widget. The data list widget assigns to the image the
    // interactor of the main widget.
    vmxImage16U *vimg16 = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U","image_of_a_cube"));
    vimg16->SetDimensions(1,dimension_s,dimension_r,dimension_c);
    
    
    
    mxGeometry g;
    g.SetDimensions(vimg16->GetDimension_S(), vimg16->GetDimension_R(), vimg16->GetDimension_C());
    
    int sn, rn,cn;
    for(g.ForCube(vimg16->GetDimension_S()/2, vimg16->GetDimension_R()/2, vimg16->GetDimension_C()/2, 0); g.GetCube(10, sn, rn, cn); )
    {
        vimg16->Set(sn,rn,cn, 255);
    }
    
    vimg16->SetVisibility(1);
    vimg16->SaveToVTK16UFile("/Users/danilobabin/-DIP_IMAGES/test_image_mipx.vtk");
    cout<<" OK ";
    

    // Create an image using the data list widget. The data list widget assigns to the image the
    // interactor of the main widget.
    vmxImage16U *vimg16_2 = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U","distance_transformed"));
    
    mxBIPX bip;
    if(!bip.DistanceTransformForSphere(*vimg16, *vimg16, *vimg16_2))
        cout<<"mxBIPX::DistanceTransformForSphere() returned 0 (fail)!"<<endl;
   
    vimg16_2->SetVisibility(1);
    
//    // Create an image using the data list widget. The data list widget assigns to the image the
//    // interactor of the main widget.
//    vmxImage16U *vimg16_3 = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U","skeletonized"));
//
//    mxSkeletonizationX skel;
//    skel.OrderedSkeletonization(*vimg16_2, *vimg16_3);
//    
//    vimg16_3->SetVisibility(1);
//    
    
    
    
    
//    // Register the vmxMeshFactory with the data list widget.
//    // The widget will later use this factory to construct objects of the given type.
//    v_tree.AddFactory(new vmxMeshFactory);
//
//    // Create a mesh using the data list widget.
//    vmxMesh *mesh = static_cast<vmxMesh*> (v_tree.Create("vmxMesh","my_mesh"));
//    //mesh->CreatePolyData(vimg, 50);
//    mesh->CreatePolyData(vimg16, 500);
//    mesh->CreateActorByLookupTableScalarColoring(0,vimg16->GetDimension_C()*vimg16->GetDimension_R()*vimg16->GetDimension_S());
//    mesh->SetVisibility(1);
//    //mesh->SetColor(1,0,0);
//    //mesh->SaveToSTLFiles("/Users/danilobabin/-DIP_IMAGES/test_mesh.stl");
    
    


    
   
    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
    main_widget.StartInteraction();


    
    return 1;
}







