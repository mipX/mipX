/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppImageViewer.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppImageViewer.h"



//-------------------------------------------------------------------------------------------



void vmxAppImageViewer::SetInputImage(vmxImage *image)
{
    image->Update();
    this->m_image_renderer->SetImageData(image->GetVTKImageData(0));
    
    //m_image_renderer->SetDirectionToCoronal();
    //m_image_renderer->SetDirectionToTransversal();
    //m_image_renderer->SetMappingToColor();
    //m_image_renderer->SetMappingToGrayScale();
    m_image_renderer->FitImageToScreen();
    
    m_image = image;
}


void vmxAppImageViewer::SetOverlayedImage(vmxImage *image)
{
    image->Update();
    this->m_image_renderer->SetOverlayedImageData(image->GetVTKImageData(0));
    
    //m_image_renderer->SetDirectionToCoronal();
    //m_image_renderer->SetDirectionToTransversal();
    //m_image_renderer->SetMappingToColor();
    //m_image_renderer->SetMappingToGrayScale();
    m_image_renderer->SetOverlayedImageMappingToColor(0.3);
    m_image_renderer->FitImageToScreen();
}


//void vmxAppImageViewer::RemoveAllSignals()
//{
//    //m_signal_renderer->RemoveAllData();
//}


void vmxAppImageViewer::Setup(vmxAppMainWidget *main_widget)
{
    m_main_widget = main_widget;

    m_image_renderer = new vmxGUIRendererImageViewer(m_main_widget);
    this->SetNumberOfRenderers(1);
    this->RegisterRenderer(0,m_image_renderer);

    //vmxGUIConnection *connection =
//    vmxGUIConnection::New(m_main_widget->GetDataListWidget()->GetListWidget(), m_main_widget->GetDataListWidget(), ItemModifiedEvent,
//                                                         m_main_widget->GetDataListWidget()->GetListWidget(), this, vmxAppImageViewer::Slot_UpdateViewer);
    
    vmxGUIConnection *connection = vmxGUIConnection::New(m_main_widget->GetDataListWidget()->GetListWidget(), ItemSelectedEvent, //ItemModifiedEvent
                                                         this->GetRenderer(), this, vmxAppImageViewer::Slot_UpdateViewer);
    
    //connection->SetPassedData(m_main_widget->GetDataListWidget()->GetListWidget()->GetLastSelectedItemData(), 0);


    connection->SetPassedAddress(m_main_widget->GetDataListWidget()->GetListWidget()->GetLastSelectedItemData(), 0);
}


void vmxAppImageViewer::Update()
{
    //m_image_renderer->Update();
}


int vmxAppImageViewer::Slot_UpdateViewer(vmxGUIConnection *connection)
{
    vmxAppImageViewer *viewer = static_cast<vmxAppImageViewer *>(connection->GetSlotCallerObject());
    
    
    mxDataObject *obj = static_cast<mxDataObject *>(*(connection->GetPassedAddress(0)));
    
    if(!obj) return 0;
    
    //    cout<<"  "<<obj->GetClassName().Get_C_String()<<"  ";
    
    vmxImage image;
    if(obj->GetClassName() == image.GetClassName())
    {
        //        cout<<" # ";
        vmxImage *img = static_cast<vmxImage *>(obj);
        viewer->SetInputImage(img);
    }

    
    
    
    
//    mxDataObject *obj = static_cast<mxDataObject *>(connection->GetPassedData(0));
//
//    vmxImage image;
//    if(obj->GetClassName() == image.GetClassName())
//    {
//        vmxImage *img = static_cast<vmxImage *>(connection->GetPassedData(0));
//        viewer->SetInputImage(img);
//    }
    

    
    
//    cout<<" vmxAppImageViewer::Slot_UpdateViewer() ";
//    vmxAppDataListWidget *data_list = static_cast<vmxAppDataListWidget *>(connection->GetSenderSubObject());
//    vmxAppImageViewer *viewer = static_cast<vmxAppImageViewer *>(connection->GetReceiverSubObject());
//
//    //viewer->RemoveAllSignals();
//
//    vmxImage image;
//
//    mxArray< mxDataObject* > selected_data_objects;
//    data_list->GetSelectedDataObjects(selected_data_objects);
//
//    for(int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
//    {
//        if(selected_data_objects[i]->GetClassName()==image.GetClassName())
//        {
//            vmxImage *img = static_cast<vmxImage *> (selected_data_objects[i]);
//            viewer->SetInputImage(img);
//        }
//    }
    
    viewer->Update();
    
    return 1;
}
                                                         
