/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppHistogramViewer.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppHistogramViewer.h"



//-------------------------------------------------------------------------------------------



void vmxAppHistogramViewer::SetInputImage(vmxImage *image, unsigned int t, unsigned int s)
{
    if(image->IsEmpty())
    {
        m_histogram_for_slice.SetNumberOfSamples(1);
        m_histogram_for_slice.Set(0, 0,0);
        return;
    }

    uint64_t minimum_value = (*image)(t,s,0,0), maximum_value = (*image)(t,s,0,0);
    for(unsigned int r=0; r<(*image).GetDimension_R(); r++)
    {
        for(unsigned int c=0; c<(*image).GetDimension_C(); c++)
        {
            if((*image)(t,s,r,c)<minimum_value) minimum_value = (*image)(t,s,r,c);
            if((*image)(t,s,r,c)>maximum_value) maximum_value = (*image)(t,s,r,c);
        }
    }

    unsigned int value_full_range = ((int)maximum_value) - ((int)minimum_value) + 1;

    m_histogram_for_slice.SetNumberOfSamples(value_full_range);
    double initial_value = 0;
    m_histogram_for_slice.FillIn_X_ValuesAscending(minimum_value,1);
    m_histogram_for_slice.Get_Y_Values()->FillInWith(initial_value);
    for(unsigned int r=0; r<(*image).GetDimension_R(); r++)
    {
        for(unsigned int c=0; c<(*image).GetDimension_C(); c++)
        {
            unsigned int local_index = (*image)(t,s,r,c)-minimum_value;
            m_histogram_for_slice.Set(local_index, (*image)(t,s,r,c), (m_histogram_for_slice.Y(local_index)+1)) ;
        }
    }

    m_signal_renderer->RemoveAllData();
    
    unsigned char r,g,b,a;
    m_histogram_for_slice.GetColor(&r,&g,&b,&a);
    m_histogram_for_slice.Update();
    m_signal_renderer->AddInputData(m_histogram_for_slice.Get_X_DoubleArray(), m_histogram_for_slice.Get_Y_DoubleArray(), m_histogram_for_slice.GetObjectName().Get_C_String(),
                                    m_histogram_for_slice.GetLabelFor_X_Axis().Get_C_String(), m_histogram_for_slice.GetLabelFor_Y_Axis().Get_C_String(), r,g,b);

}


void vmxAppHistogramViewer::RemoveAllSignals()
{
    m_signal_renderer->RemoveAllData();
}


void vmxAppHistogramViewer::Setup(vmxAppMainWidget *main_widget)
{
    m_main_widget = main_widget;
    
    m_signal_renderer = new vmxGUIRendererSignalViewer(m_main_widget);
    this->SetNumberOfRenderers(1);
    this->RegisterRenderer(0,m_signal_renderer);
    
    
//    vmxGUIConnection *connection = vmxGUIConnection::New(m_main_widget->GetDataListWidget()->GetListWidget(), ItemSelectedEvent,
//                                                         this->GetRenderer(), this, vmxAppHistogramViewer::Slot_UpdateViewer);
//
//    connection->SetPassedAddress(m_main_widget->GetDataListWidget()->GetListWidget()->GetLastSelectedItemData(), 0);
    
}


void vmxAppHistogramViewer::Update()
{
    m_signal_renderer->Update();
}


int vmxAppHistogramViewer::Slot_UpdateViewer(vmxGUIConnection *connection)
{
    vmxAppHistogramViewer *viewer = static_cast<vmxAppHistogramViewer *>(connection->GetSlotCallerObject());
    
    mxDataObject *obj = static_cast<mxDataObject *>(*(connection->GetPassedAddress(0)));
    if(!obj) return 0;

    vmxImage image;
    if(obj->GetClassName() == image.GetClassName())
    {
        vmxImage *img = static_cast<vmxImage *>(obj);
        
        int *t = connection->GetPassedDataInt(0);
        int *s = connection->GetPassedDataInt(1);
        if(s && t)
        {
            viewer->SetInputImage(img,*t,*s);
        }
        else viewer->SetInputImage(img,0,0);
    }
    
    
    viewer->Update();
    
    return 1;
}
                                                         
