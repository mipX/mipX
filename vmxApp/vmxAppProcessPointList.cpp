/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessPointList.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppProcessPointList.h"






//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessPointList_InsertValuesFromCurve::vmxAppProcessPointList_InsertValuesFromCurve()
{
    this->GetConfig()->SetNumberOfSteps(1);
    this->GetConfig()->SetTitleInMenu("PointList>>Insert values from curve");
    this->GetConfig()->SetTitleLong("Insert point list values from curve");
    //this->GetConfig()->SetHelp("Calculate the distensibility of Aorta from segmented cross-sectional 2D+time (CINE) images.");
    //this->DeclareBatchCall("AorticPWV", "", "AorticDistensibility", "<Image>");
}


void vmxAppProcessPointList_InsertValuesFromCurve::Step_0()
{
    this->GetConfig()->AddInputDataObjects("Point list and curve: ");
    
}


void vmxAppProcessPointList_InsertValuesFromCurve::Execute()
{
    vmxPointList pl_temp;
    mxArray<mxDataObject*> a_pl = this->GetData()->GetDataObjectsOfType(pl_temp.GetClassName().Get_C_String());
    if(a_pl.IsEmpty())
    {
        std::cout<<std::endl<<"vmxAppProcessPointList_InsertValuesFromCurve::Execute(): error: no point list entered."<<std::endl;
        return;
    }
    
    vmxCurve c_temp;
    mxArray<mxDataObject*> a_c = this->GetData()->GetDataObjectsOfType(c_temp.GetClassName().Get_C_String());
    if(a_c.IsEmpty())
    {
        std::cout<<std::endl<<"vmxAppProcessPointList_InsertValuesFromCurve::Execute(): error: no curve entered."<<std::endl;
        return;
    }
    
    vmxCurve *input_curve =  static_cast< vmxCurve* >( a_c[0] );
    vmxPointList *input_pl =  static_cast< vmxPointList* >( a_pl[0] );

    pl_temp.Copy(input_pl);
    
    int number_of_samples = 100;
    
    pl_temp.InterpolateUsingCardinalSpline(number_of_samples);//input_curve->GetNumberOfSamples());
    
    unsigned int FFR_index;
    pl_temp.AddScalarTag("FFR", FFR_index);
    
    
    double di = ((double)input_curve->GetNumberOfSamples()) / ((double)number_of_samples);
    
    vmxPointList::Iterator itpl;
    int i=0;
    for(itpl.SetToBegin(pl_temp.GetListOfPoints()), i=0; itpl.IsValid() && i<number_of_samples; itpl.MoveToNext(), i++)
    {
        int curve_index = (int) (((double)i) * di);
        itpl.GetElementAddress()->m_label.AssignNumber(input_curve->Y( curve_index ));
        itpl.GetElementAddress()->m_position.V(FFR_index) = input_curve->Y( curve_index );
    }
    
    vmxPointList *pl = static_cast< vmxPointList* > (this->GetAppMainWidget()->GetDataListWidget()->Create(pl_temp.GetClassName().Get_C_String(), "interpolated" ));
    if(!pl) return;
    
    pl->Copy(&pl_temp);
    pl->SetColorMappingToScalars("FFR");
    pl->UpdateScalars(FFR_index);
    
//
//    for(int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
//    {
//        vmxImage img;
//        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
//        {
//            mxString name;
//            name.Assign("grp_");
//            name.Append(selected_data_objects[i]->GetObjectName());
//
//            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
//            if(!image) return;
//
//            vmxGraph g_temp;
//            vmxGraph *graph = static_cast< vmxGraph* > (this->GetAppMainWidget()->GetDataListWidget()->Create(g_temp.GetClassName().Get_C_String(), name.Get_C_String()));
//            if(!graph) return;
//
//
//            mxGraphProcessing gp;
//            gp.CreateGraphFromImage(*image, (*graph));
//
//            graph->Update();
//            graph->SetTimeIndex(0);
//
//        }
//    }
}


void vmxAppProcessPointList_InsertValuesFromCurve::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------



