/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessGraph.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppProcessGraph.h"






//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessGraph_CreateGraphFromImage::vmxAppProcessGraph_CreateGraphFromImage()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Graph>>Create Graph From Image");
    this->GetConfig()->SetTitleLong("Create Graph From Image");
    //this->GetConfig()->SetHelp("Calculate the distensibility of Aorta from segmented cross-sectional 2D+time (CINE) images.");
    //this->DeclareBatchCall("AorticPWV", "", "AorticDistensibility", "<Image>");
}


void vmxAppProcessGraph_CreateGraphFromImage::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            mxString name;
            name.Assign("grp_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            vmxGraph g_temp;
            vmxGraph *graph = static_cast< vmxGraph* > (this->GetAppMainWidget()->GetDataListWidget()->Create(g_temp.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!graph) return;
            
            
            mxGraphProcessing gp;
            gp.CreateGraphFromImage(*image, (*graph));
            
            graph->Update();
            graph->SetTimeIndex(0);

        }
    }
}


void vmxAppProcessGraph_CreateGraphFromImage::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------





vmxAppProcessGraph_CreateGraphFromPointList::vmxAppProcessGraph_CreateGraphFromPointList()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Graph>>Create Graph From Point List");
    this->GetConfig()->SetTitleLong("Create Graph From Point List");
    //this->GetConfig()->SetHelp("Calculate the distensibility of Aorta from segmented cross-sectional 2D+time (CINE) images.");
    //this->DeclareBatchCall("AorticPWV", "", "AorticDistensibility", "<Image>");
}


void vmxAppProcessGraph_CreateGraphFromPointList::Execute()
{
    //this->GetAppMainWidget()->m
    
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxPointList poli;
        if(selected_data_objects[i]->GetClassName()==poli.GetClassName())
        {
            mxString name;
            name.Assign("grp_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxPointList *pl = static_cast<vmxPointList*>(selected_data_objects[i]);
            if(!pl) return;
            
            vmxGraph g_temp;
            
//            vmxPointList::Iterator pli;
//            for(pli.SetToBegin(pl->GetListOfPoints()); pli.IsValid(); pli.MoveToNext())
//            {
//                g_temp.Add
//                
//            }
//            
//            
//            
//            
//            vmxGraph *graph = static_cast< vmxGraph* > (this->GetAppMainWidget()->GetDataListWidget()->Create(g_temp.GetClassName().Get_C_String(), name.Get_C_String()));
//            if(!graph) return;
//            
//            
//            mxGraphProcessing gp;
//            gp.CreateGraphFromImage(*image, (*graph));
//            
//            graph->Update();
//            graph->SetTimeIndex(0);
            
        }
    }
}


void vmxAppProcessGraph_CreateGraphFromPointList::StartInMainThread()
{
    this->Execute();
}
