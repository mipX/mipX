/*=========================================================================
 
 Program:   mipx
 Module:    example_HistogramAsCurve.cpp
 
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


#include "mxBIP.h"
#include "mxCurve.h"
#include "mxGIP.h"
#include "vmxDataListWidget.h"
#include "vmxImage.h"




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
    vmxImage16U *vimg16 = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U","cube_and_sphere"));
    vimg16->SetDimensions(1,dimension_s,dimension_r,dimension_c);

    //vimg16->LoadVTKFile("/Users/danilobabin/-DIP_IMAGES/-CEREBRAL VESSELS/02 - RP05 - Roothooft Bjorn Jozef - AVM - _OK_but_not_overlapping/vesselsThr70Masked.vtk");
    
    int cube_size = 10;
    
    
    mxGeometry g;
    g.SetDimensions(vimg16->GetDimension_S(), vimg16->GetDimension_R(), vimg16->GetDimension_C());
    
    int sn, rn,cn;
    for(g.ForCube(vimg16->GetDimension_S()/2, vimg16->GetDimension_R()/2, vimg16->GetDimension_C()/2, 0); g.GetCube(cube_size, sn, rn, cn); )
    {
        vimg16->Set(sn,rn,cn, 255);
    }
    for(g.ForSphere(vimg16->GetDimension_S()/2+cube_size*2, vimg16->GetDimension_R()/2, vimg16->GetDimension_C()/2, 0); g.GetSphere((cube_size*cube_size), sn, rn, cn); )
    {
        vimg16->Set(sn,rn,cn, 255);
    }

    
    vimg16->SetVisibility(1);
    vimg16->SaveToVTK16UFile("/Users/danilobabin/-DIP_IMAGES/test_image_mipx.vtk");
    cout<<" OK ";
    

    // Create an image using the data list widget. The data list widget assigns to the image the
    // interactor of the main widget.
    vmxImage16U *vimg16_2 = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U","distance_transformed"));
    
    mxBIP bip;
    //if(!bip.DistanceTransformForSphere(*vimg16, *vimg16, *vimg16_2)) cout<<"mxBIP::DistanceTransformForSphere() returned 0 (fail)!"<<endl;

    if(!bip.ProfileVolumeTransformForSphere(*vimg16, *vimg16, *vimg16_2))
        cout<<"mxBIP::DistanceTransformForSphere() returned 0 (fail)!"<<endl;
   
    vimg16_2->SetVisibility(1);
    
  
    cout<<"  Distance transform done  ";
    
    
    
    
    mxCurve histogram;
    mxGIP gip;
    
    gip.Histogram((*vimg16_2), histogram);
    histogram.SaveToMatlabFile("/Users/danilobabin/-DIP_IMAGES/test_histogram_mipx.m");
    
    cout<<" Histogram saved! ";
    
    
    mxCurve histogram2;
    if(!histogram2.LoadMatlabFile("/Users/danilobabin/-DIP_IMAGES/test_histogram_mipx.m"))
    {
        cout<<" Error loading curve from Matlab file  ";
        return 0;
    }
    
    for(unsigned int s=0; s<histogram2.GetNumberOfSamples(); s++)
    {
        cout<<" "<<histogram2.Y(s);
    }
    
    
    cout<<"  Histogram loaded! ";



    
   
    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
    main_widget.StartInteraction();


    
    return 1;
}







