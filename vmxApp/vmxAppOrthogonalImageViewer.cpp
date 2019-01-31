/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppOrthogonalImageViewer.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppOrthogonalImageViewer.h"


//-------------------------------------------------------------------------------------------




void vmxAppOrthogonalImageViewer::SetInputImage(vmxImage *image)
{
    image->Update();
    this->m_renderer_TSC[0]->SetImageData(image->GetVTKImageData(0));
    this->m_renderer_TSC[1]->SetImageData(image->GetVTKImageData(0));
    this->m_renderer_TSC[2]->SetImageData(image->GetVTKImageData(0));
    
    
    //m_image_renderer->SetMappingToColor();
    //m_image_renderer->SetMappingToGrayScale();
    m_renderer_TSC[0]->FitImageToScreen();
    m_renderer_TSC[1]->FitImageToScreen();
    m_renderer_TSC[2]->FitImageToScreen();
}


void vmxAppOrthogonalImageViewer::Setup(vmxAppMainWidget *main_widget)
{
    m_main_widget = main_widget;
    
    m_renderer_TSC[0] = new vmxGUIRendererImageViewer(m_main_widget);
    m_renderer_TSC[1] = new vmxGUIRendererImageViewer(m_main_widget);
    m_renderer_TSC[2] = new vmxGUIRendererImageViewer(m_main_widget);
    
    this->SetNumberOfRenderers(3);
    this->RegisterRenderer(0,m_renderer_TSC[0]);
    this->RegisterRenderer(1,m_renderer_TSC[1]);
    this->RegisterRenderer(2,m_renderer_TSC[2]);
    
    m_renderer_TSC[0]->SetDirectionToTransversal();
    m_renderer_TSC[1]->SetDirectionToCoronal();
    m_renderer_TSC[2]->SetDirectionToSagittal();
    
    
//    {
//        vmxGUIConnection *c = vmxGUIConnection::New(m_renderer_TSC[0], ImagePickedEvent, m_renderer_TSC[1], vmxGUIRendererImageViewer::Slot_Pick);
//        c->SetPassedDataInt(m_renderer_TSC[0]->GetPickedIndexes());
//        //c->SetPassedDataDouble(m_array_of_image_renderers[0]->GetPickedWorldPosition());
//    }
//    {
//        vmxGUIConnection *c = vmxGUIConnection::New(m_renderer_TSC[0], ImagePickedEvent, m_renderer_TSC[2], vmxGUIRendererImageViewer::Slot_Pick);
//        c->SetPassedDataInt(m_renderer_TSC[0]->GetPickedIndexes());
//        //c->SetPassedDataDouble(m_array_of_image_renderers[0]->GetPickedWorldPosition());
//    }
//    {
//        vmxGUIConnection *c = vmxGUIConnection::New(m_renderer_TSC[1], ImagePickedEvent, m_renderer_TSC[0], vmxGUIRendererImageViewer::Slot_Pick);
//        c->SetPassedDataInt(m_renderer_TSC[1]->GetPickedIndexes());
//        //c->SetPassedDataDouble(m_array_of_image_renderers[1]->GetPickedWorldPosition());
//    }
//    {
//        vmxGUIConnection *c = vmxGUIConnection::New(m_renderer_TSC[1], ImagePickedEvent, m_renderer_TSC[2], vmxGUIRendererImageViewer::Slot_Pick);
//        c->SetPassedDataInt(m_renderer_TSC[1]->GetPickedIndexes());
//        //c->SetPassedDataDouble(m_array_of_image_renderers[1]->GetPickedWorldPosition());
//    }
//    {
//        vmxGUIConnection *c = vmxGUIConnection::New(m_renderer_TSC[2], ImagePickedEvent, m_renderer_TSC[0], vmxGUIRendererImageViewer::Slot_Pick);
//        c->SetPassedDataInt(m_renderer_TSC[2]->GetPickedIndexes());
//        //c->SetPassedDataDouble(m_array_of_image_renderers[2]->GetPickedWorldPosition());
//    }
//    {
//        vmxGUIConnection *c = vmxGUIConnection::New(m_renderer_TSC[2], ImagePickedEvent, m_renderer_TSC[1], vmxGUIRendererImageViewer::Slot_Pick);
//        c->SetPassedDataInt(m_renderer_TSC[2]->GetPickedIndexes());
//        //c->SetPassedDataDouble(m_array_of_image_renderers[2]->GetPickedWorldPosition());
//    }
//
//    {
//        //vmxGUIConnection *connection =
//        vmxGUIConnection::New(m_main_widget->GetDataListWidget()->GetListWidget(), m_main_widget->GetDataListWidget(), ItemModifiedEvent,
//                              m_main_widget->GetDataListWidget()->GetListWidget(), this, vmxAppOrthogonalImageViewer::Slot_UpdateViewer);
//    }
}


void vmxAppOrthogonalImageViewer::Update()
{
    //m_renderer_TSC[0]->Update();
}


int vmxAppOrthogonalImageViewer::Slot_UpdateViewer(vmxGUIConnection *connection)
{
    cout<<" vmxAppOrthogonalImageViewer::Slot_UpdateViewer() ";
//    vmxAppDataListWidget *data_list = static_cast<vmxAppDataListWidget *>(connection->GetSenderSubObject());
//    vmxAppOrthogonalImageViewer *viewer = static_cast<vmxAppOrthogonalImageViewer *>(connection->GetReceiverSubObject());
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
//    
//    viewer->Update();
    
    return 1;
}
                                                         
