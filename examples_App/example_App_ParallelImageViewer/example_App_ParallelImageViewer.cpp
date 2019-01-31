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
 
 This example implements viewing of images in parallel.
 
 */


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
    //mxScopedPointer< vmxImage > image3;
    
    vmxAppImageViewer image_viewer;
    vmxAppImageViewer image_viewer2;
    //vmxAppImageViewer image_viewer3;
    
public:
    
    
    //static int Slot_RecalculateImage(vmxGUIConnection *connection);
    
    
    MainApp()
    {
        this->SetDataListWidgetToDefault();
        //    this->SetRenderer3DToDefaultTrackballCamera();
        
        // Register the vmxImage16UFactory with the data list widget.
        // The widget will later use this factory to construct objects of the given type.
        this->GetDataListWidget()->AddFactory(new vmxImage16UFactory);
        
        
        image = new vmxImage();
        image2 = new vmxImage();
        //image3 = new vmxImage();
        
        image->LoadVTKFile("/Users/danilobabin/Dropbox/-DIP_IMAGES/-fUS/From_Gabriel_2018_10_09/Segmentation/D/processed/data.vtk");
        //        image2->LoadVTKFile("/Users/danilobabin/Dropbox/-DIP_IMAGES/-fUS/From_Gabriel_2018_10_09/Segmentation/D/prf/thr/thr4.vtk");
        //        image3->LoadVTKFile("/Users/danilobabin/Dropbox/-DIP_IMAGES/-fUS/From_Gabriel_2018_10_09/Segmentation/D/prf/thr/thr2-4.vtk");
        image2->LoadVTKFile("/Users/danilobabin/Dropbox/-DIP_IMAGES/-fUS/From_Gabriel_2018_10_09/Segmentation/D/prf/thr10/thr5.vtk");
        //image3->LoadVTKFile("/Users/danilobabin/Dropbox/-DIP_IMAGES/-fUS/From_Gabriel_2018_10_09/Segmentation/D/prf/thr10/thr3-5.vtk");
        
        
        
        this->RegisterViewer(&image_viewer);
        image_viewer.GetRenderer(0)->SetMappingToGrayScale();
        //image_viewer.GetRenderer(0)->SetMappingToColor();
        image_viewer.SetInputImage(image);
        image_viewer.GetRenderer()->FitImageToScreen();
        
        
        this->RegisterViewer(&image_viewer2);
        image_viewer2.GetRenderer(0)->SetMappingToGrayScale();
        //image_viewer2.GetRenderer(0)->SetMappingToColor();
        image_viewer2.SetInputImage(image2);
        //image_viewer2.SetOverlayedImage(image2);
        //image_viewer2.GetRenderer()->SetOverlayedImageMappingToColor(0.3);
        image_viewer2.GetRenderer()->FitImageToScreen();
        
        
//        this->RegisterViewer(&image_viewer3);
//        image_viewer3.GetRenderer(0)->SetMappingToGrayScale();
//
//        //image_viewer3.SetInputImage(image3);
//        image_viewer3.SetInputImage(image);
//        image_viewer3.SetOverlayedImage(image3);
//        image_viewer3.GetRenderer()->SetOverlayedImageMappingToBinary();//image_viewer3.GetRenderer()->SetOverlayedImageMappingToColor(0.3);
//
//        image_viewer3.GetRenderer()->FitImageToScreen();
//
        //vmxGUIConnection *connection = vmxGUIConnection::New(image_viewer3.GetRenderer(), KeyPressEvent,
        //                                                     this, MainApp::Slot_RecalculateImage);
        //connection->SetPassedData(this);
        
        {
            vmxGUIConnection *c = vmxGUIConnection::New(image_viewer.GetRenderer(), ImageSliceChangeEvent, image_viewer2.GetRenderer(), image_viewer2.GetRenderer(), vmxGUIRendererImageViewer::Slot_SetIndexSlice);
            c->SetPassedDataInt((int*)(&(image_viewer.GetRenderer()->m_index_slice)));
        }
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(image_viewer.GetRenderer(), ImageSliceChangeEvent, image_viewer3.GetRenderer(), vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(image_viewer.GetRenderer()->m_index_slice)));
//        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(image_viewer2.GetRenderer(), ImageSliceChangeEvent, image_viewer.GetRenderer(), image_viewer2.GetRenderer(), vmxGUIRendererImageViewer::Slot_SetIndexSlice);
            c->SetPassedDataInt((int*)(&(image_viewer2.GetRenderer()->m_index_slice)));
        }
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(image_viewer2.GetRenderer(), ImageSliceChangeEvent, image_viewer3.GetRenderer(), vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(image_viewer2.GetRenderer()->m_index_slice)));
//        }
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(image_viewer3.GetRenderer(), ImageSliceChangeEvent, image_viewer.GetRenderer(), vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(image_viewer3.GetRenderer()->m_index_slice)));
//        }
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(image_viewer3.GetRenderer(), ImageSliceChangeEvent, image_viewer2.GetRenderer(), vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(image_viewer3.GetRenderer()->m_index_slice)));
//        }
        
        
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

