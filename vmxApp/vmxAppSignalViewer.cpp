/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppSignalViewer.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppSignalViewer.h"


//-------------------------------------------------------------------------------------------




void vmxAppSignalViewer::AddInputSignal(vmxCurve *curve)
{
    unsigned char r,g,b,a;
    curve->GetColor(&r,&g,&b,&a);
    //cout<<"  rgba="<<((int)r)<<","<<((int)g)<<","<<((int)b)<<","<<((int)a);
    curve->Update();
    m_signal_renderer->AddInputData(curve->Get_X_DoubleArray(), curve->Get_Y_DoubleArray(), curve->GetObjectName().Get_C_String(), curve->GetLabelFor_X_Axis().Get_C_String(),
                                    curve->GetLabelFor_Y_Axis().Get_C_String(), r,g,b);
}


void vmxAppSignalViewer::RemoveAllSignals()
{
    m_signal_renderer->RemoveAllData();
}


void vmxAppSignalViewer::Setup(vmxAppMainWidget *main_widget)
{
    m_main_widget = main_widget;
    
    m_signal_renderer = new vmxGUIRendererSignalViewer(m_main_widget);
    this->SetNumberOfRenderers(1);
    this->RegisterRenderer(0,m_signal_renderer);
    
//    vmxGUIConnection *connection = vmxGUIConnection::New(m_main_widget->GetDataListWidget()->GetListWidget(), m_main_widget->GetDataListWidget(), ItemModifiedEvent,
//                                                         m_main_widget->GetDataListWidget()->GetListWidget(), this, vmxAppSignalViewer::Slot_UpdateViewer);

    vmxGUIConnection *connection = vmxGUIConnection::New(m_main_widget->GetDataListWidget()->GetListWidget(), ItemSelectedEvent, //ItemModifiedEvent
                                                         this->GetRenderer(), this, vmxAppSignalViewer::Slot_UpdateViewer);
    
    connection->SetPassedData(m_main_widget->GetDataListWidget(), 0);

}


void vmxAppSignalViewer::Update()
{
    m_signal_renderer->Update();
}


int vmxAppSignalViewer::Slot_UpdateViewer(vmxGUIConnection *connection)
{
    vmxAppSignalViewer *viewer = static_cast<vmxAppSignalViewer *>(connection->GetSlotCallerObject());
    
//    mxDataObject *obj = static_cast<mxDataObject *>(connection->GetPassedData(0));
//
//    vmxCurve curve;
//    if(obj->GetClassName() == curve.GetClassName())
//    {
//        vmxCurve *curve = static_cast<vmxCurve *>(connection->GetPassedData(0));
//        viewer->AddInputSignal(c);
//    }

    //cout<<" vmxAppSignalViewer::Slot_UpdateViewer() ";
    vmxAppDataListWidget *data_list = static_cast<vmxAppDataListWidget *>(connection->GetPassedData());
    
    viewer->RemoveAllSignals();
    
    vmxCurve curve;
    
    
    mxListIterator< mxDataObjectFactory* > itf;
    for(itf.SetToBegin(data_list->GetDataObjectFactories()); itf.IsValid(); itf.MoveToNext())
    {
        //cout<<" ^ ";
        
        if(itf.GetElement()->GetClassName() == curve.GetClassName())
        {
            mxListIterator<mxDataObject*> ito;
            for(ito.SetToBegin(itf.GetElement()->GetCreatedDataObjects()); ito.IsValid(); ito.MoveToNext())
            {
                //cout<<" V ";
                if(ito.GetElement()->GetVisibilityOfComponent(0))
                {
                    //cout<<" W ";
                    vmxCurve *c = static_cast<vmxCurve *> (ito.GetElement());
                    viewer->AddInputSignal(c);
                }
            }
        }
    }

    
    
    
//    //cout<<" vmxAppSignalViewer::Slot_UpdateViewer() ";
//    vmxAppDataListWidget *data_list = static_cast<vmxAppDataListWidget *>(connection->GetSenderSubObject());
//    vmxAppSignalViewer *viewer = static_cast<vmxAppSignalViewer *>(connection->GetReceiverSubObject());
//
//    viewer->RemoveAllSignals();
//
//    vmxCurve curve;
//
//
//    mxListIterator< mxDataObjectFactory* > itf;
//    for(itf.SetToBegin(data_list->GetDataObjectFactories()); itf.IsValid(); itf.MoveToNext())
//    {
//        //cout<<" ^ ";
//
//        if(itf.GetElement()->GetClassName() == curve.GetClassName())
//        {
//            mxListIterator<mxDataObject*> ito;
//            for(ito.SetToBegin(itf.GetElement()->GetCreatedDataObjects()); ito.IsValid(); ito.MoveToNext())
//            {
//                //cout<<" V ";
//                if(ito.GetElement()->GetVisibilityOfComponent(0))
//                {
//                    //cout<<" W ";
//                    vmxCurve *c = static_cast<vmxCurve *> (ito.GetElement());
//                    viewer->AddInputSignal(c);
//                }
//            }
//        }
//    }
    
    viewer->Update();
    
    return 1;
}
                                                         
