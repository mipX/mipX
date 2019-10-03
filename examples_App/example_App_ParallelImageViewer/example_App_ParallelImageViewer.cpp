/*=========================================================================
 
 Program:   mipx
 Module:    example_App_ParallelImageViewer.cpp
 
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
 
 This example implements viewing of images in parallel. The example does not use the data tree (data list widget)
 to create the images, instead they are created in the main widget and assigned to image renderers.
 
 */


#include "mxGeometry.h"
#include "mxPosition.h"
#include "mxBIP.h"
#include "mxGIP.h"


#include "vmxAppWidget.h"
#include "vmxAppImageViewer.h"
#include "vmxImage.h"

#include <vtkMath.h>









class MainApp : public vmxAppMainWidget
{
    
    mxScopedPointer< vmxImage > image;
    mxScopedPointer< vmxImage > image2;
    
    vmxAppImageViewer image_viewer;
    vmxAppImageViewer image_viewer2;
    
public:
    
    
    MainApp()
    {
        // Note: If you do not want to use the data list widget, you do not have to create it!
        //this->SetDataListWidgetToDefault();
        //this->GetDataListWidget()->AddFactory(new vmxImage16UFactory);
        
        
        // Create images
        image = new vmxImage();
        image2 = new vmxImage();
        
        this->CreateTestImage(image);
        
        mxBIP bip;
        if(!bip.ProfileVolumeTransformForSphere(*image, *image, *image2));

        
        // Register viewer
        this->RegisterViewer(&image_viewer);
        image_viewer.GetRenderer(0)->SetMappingToGrayScale();
        image_viewer.SetInputImage(image);
        image_viewer.GetRenderer()->FitImageToScreen();
        
        // Register viewer
        this->RegisterViewer(&image_viewer2);
        image_viewer2.SetInputImage(image2);
        image_viewer2.GetRenderer(0)->SetMappingToColor();
        image_viewer2.GetRenderer()->FitImageToScreen();
        
        // Create connections to make the viewers display images in parallel.
        {
            vmxGUIConnection *c = vmxGUIConnection::New(image_viewer.GetRenderer(), ImageSliceChangeEvent, image_viewer2.GetRenderer(), image_viewer2.GetRenderer(), vmxGUIRendererImageViewer::Slot_SetIndexSlice);
            c->SetPassedDataInt((int*)(&(image_viewer.GetRenderer()->m_index_slice)));
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(image_viewer2.GetRenderer(), ImageSliceChangeEvent, image_viewer.GetRenderer(), image_viewer2.GetRenderer(), vmxGUIRendererImageViewer::Slot_SetIndexSlice);
            c->SetPassedDataInt((int*)(&(image_viewer2.GetRenderer()->m_index_slice)));
        }
        
        
    };
    
    
    /// Create a test image
    void CreateTestImage(vmxImage *vimg16)
    {
        unsigned int dimension_s = 100, dimension_r = 100, dimension_c = 100;
        
        int cube_size = 10;
        
        vimg16->SetDimensions(1,dimension_s,dimension_r,dimension_c);
        
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

