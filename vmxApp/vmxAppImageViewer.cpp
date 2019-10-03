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



void vmxAppImageViewer::DisconnectFromDefaultDataListWidget()
{
    vmxGUIConnection::Delete(m_main_widget->GetDataListWidget()->GetListWidget(), ItemSelectedEvent, this->GetRenderer(), vmxAppImageViewer::Slot_UpdateViewer);
}


void vmxAppImageViewer::GetSeedPointList(mxPointList *pl)
{
    pl->Reset();
    double c,r,s,v;
    for(int i=0; i<this->m_image_renderer->GetNumberOfPickedPositions(); i++)
    {
        this->m_image_renderer->GetPickedPosition(i, c,r,s,v);
        pl->AddPosition(s, r, c);
    }
}


void vmxAppImageViewer::SetInputImage(vmxImage *image)
{
    image->Update();
    this->m_image_renderer->SetImageData(image->GetVTKImageData(0));
    
    image->SetMappingToOpaqueGrayScale();
    m_image_renderer->SetMappingToGrayScale();
    
    m_image_renderer->FitImageToScreen();
    
    m_image = image;
    
//    uint64_t min, max;
//    min = 0;
//    max = 2^(sizeof(T)) - 1;
//    if(m_image)
//    {
//        if(!m_image->IsEmpty())
//        {
//            this->m_image->GetVoxelMinAndMaxValues(min, max);
//        }
//    }
//    this->BuildLookupTable(min,max, 0,1, 1,1, 0,0, 0,0.66);
//    if(this->m_interactor) this->m_interactor->Render();

}


void vmxAppImageViewer::SetOverlayedImage(vmxImage *image)
{
    if(image)
    {
        image->Update();
        this->m_image_renderer->SetOverlayedImageData(image->GetVTKImageData(0));
        
        m_image_renderer->SetOverlayedImageMappingToBinary(); //SetOverlayedImageMappingToColor(0.3);
        m_image_renderer->FitImageToScreen();
        return;
    }
}


void vmxAppImageViewer::Setup(vmxAppMainWidget *main_widget)
{
    m_main_widget = main_widget;

    m_image_renderer = new vmxGUIRendererImageViewer(m_main_widget);
    this->SetNumberOfRenderers(1);
    this->RegisterRenderer(0,m_image_renderer);
    
    m_command_seeds_to_point_list = m_image_renderer->AddCommand("Seeds to PointList");
    
    vmxGUIConnection::New(m_image_renderer, LeftButtonDropEvent, m_image_renderer, this, vmxAppImageViewer::Slot_OnDrop);
    vmxGUIConnection::New(m_image_renderer, KeyPressEvent, m_image_renderer, this, vmxAppImageViewer::Slot_OnKeyPress);
    vmxGUIConnection::New(m_image_renderer, LeftButtonUpEvent, m_image_renderer, this, vmxAppImageViewer::Slot_OnLeftButtonUp);
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
    
    vmxImage image;
    if(obj->GetClassName() == image.GetClassName())
    {
        vmxImage *img = static_cast<vmxImage *>(obj);
        viewer->SetInputImage(img);
    }

    viewer->Update();
    
    return 1;
}



int vmxAppImageViewer::Slot_OnDrop(vmxGUIConnection *connection)
{
    vmxAppImageViewer *app_viewer = static_cast<vmxAppImageViewer *>(connection->GetSlotCallerObject());
    vmxGUIRendererImageViewer *gui_viewer = static_cast<vmxGUIRendererImageViewer *>(connection->GetSenderObject());
    
    mxString data_string;
    
    if(!gui_viewer->GetClipBoard()->FetchData(&data_string)) return 0;
    
    mxString s;
    s.Assign(vmxGUIItemSeparationChar1);
    s.Append(data_string);
    mxString item_name;
    s.ExtractString(vmxGUIItemSeparationChar1, vmxGUIItemSeparationChar1, item_name);
    
    mxDataObject *obj = app_viewer->m_main_widget->GetDataListWidget()->GetDataObject(item_name.Get_C_String());
    if(!obj) return 0;
    
    vmxImage img;
    if(obj->GetClassName()==img.GetClassName())
    {
        vmxImage *image = static_cast< vmxImage* >(obj);
        if(image)
        {
            app_viewer->SetInputImage(image);
            
            app_viewer->m_image_renderer->SetMappingToGrayScale();
        }
    }
    return 1;
}



int vmxAppImageViewer::Slot_OnKeyPress(vmxGUIConnection *connection)
{
    vmxAppImageViewer *app_viewer = static_cast<vmxAppImageViewer *>(connection->GetSlotCallerObject());
    vmxGUIRendererImageViewer *gui_viewer = static_cast<vmxGUIRendererImageViewer *>(connection->GetSenderObject());

    vtkRenderWindowInteractor *rwi = gui_viewer->m_interactor_style->GetInteractor();
    std::string key = rwi->GetKeySym();
    
    if(key.compare("s") == 0)
    {
        if(!gui_viewer->m_image_data) return 1;
        
        if(gui_viewer->m_index_time==app_viewer->m_image->GetDimension_T()-1) gui_viewer->m_index_time = 0;
        else gui_viewer->m_index_time = gui_viewer->m_index_time+1;
        gui_viewer->SetImageData(app_viewer->m_image->GetVTKImageData(gui_viewer->m_index_time));
        
        gui_viewer->DisplayProperties();
        
        // Don't forget to invoke event, otherwise connections won't work.
        gui_viewer->InvokeEvent(ImageSliceChangeEvent, gui_viewer, NULL);
        
        rwi->Render();
        
        return 1;
    }
    if(key.compare("x") == 0)
    {
        if(!gui_viewer->m_image_data) return 1;
        
        if(gui_viewer->m_index_time==0) gui_viewer->m_index_time = app_viewer->m_image->GetDimension_T() - 1;
        else gui_viewer->m_index_time = gui_viewer->m_index_time-1;
        gui_viewer->SetImageData(app_viewer->m_image->GetVTKImageData(gui_viewer->m_index_time));
        
        gui_viewer->DisplayProperties();
        
        // Don't forget to invoke event, otherwise connections won't work.
        gui_viewer->InvokeEvent(ImageSliceChangeEvent, gui_viewer, NULL);
        
        rwi->Render();
        
        return 1;
    }
    
    return 1;

}



int vmxAppImageViewer::Slot_OnLeftButtonUp(vmxGUIConnection *connection)
{
    vmxAppImageViewer *app_viewer = static_cast<vmxAppImageViewer *>(connection->GetSlotCallerObject());
    vmxGUIRendererImageViewer *gui_viewer = static_cast<vmxGUIRendererImageViewer *>(connection->GetSenderObject());
    
    vtkRenderWindowInteractor *rwi = gui_viewer->m_interactor_style->GetInteractor();
    
    vtkTextActor *ta = gui_viewer->GetCommand(rwi->GetEventPosition()[0],rwi->GetEventPosition()[1]);
    
    if(ta == app_viewer->m_command_seeds_to_point_list)
    {
        if(gui_viewer->GetNumberOfPickedPositions()>0)
        {
            vmxPointList *pl = static_cast< vmxPointList* >( app_viewer->m_main_widget->GetDataListWidget()->Create("vmxPointList", "Seeds") );
            
            if(pl)
            {
                app_viewer->GetSeedPointList(pl);
                pl->Update();
            }
        }
    }
    
    return 1;
}
