/*=========================================================================
 
 Program:   mipx
 Module:    App_PWVPaPd.cpp
 
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
 
 This example shows the app to extract Ps and Pd pressure waves from screen grabs and calculate PWV (using premeasured distance).
 
 */


#include "mxBIP.h"
#include "mxGIP.h"
#include "mxGeometry.h"

#include "vmxCurve.h"

#include "vmxAppWidget.h"
#include "vmxAppImageViewer.h"
#include "vmxAppSignalViewer.h"
#include "vmxImage.h"

#include "vmxGUIButtonGroup.h"

#include <vtkMath.h>
#include <vtkPNGReader.h>
#include <vtkImageExtractComponents.h>


#include "PWVPaPd.h"


class MainApp : public vmxAppMainWidget
{
public:
    
//    mxScopedPointer< vmxImage > image;
//    mxScopedPointer< vmxImage > image2;
    
    
    vmxAppImageViewer m_image_viewer1;
    vmxAppImageViewer m_image_viewer2;
    
    vmxAppSignalViewer m_signal_viewer;
    
    PWVPaPdFunctionList m_pwv_pa_pd_functions;
    
    
//    vmxGUIButtonGroup m_create_Pa_curve_push_button;
//    vmxGUIButtonGroup m_create_Pd_curve_push_button;
    
    
public:
    
    
//    static int Slot_CreatePaCurveButtonClicked(vmxGUIConnection *connection)
//    {
//        MainApp *ma = static_cast<MainApp*> (connection->GetSlotCallerObject());
//
//        std::cout<<" "<<ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[0]<<", "<<ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[1]<<", "<<ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[2];
//
//        mxGeometry g;
//        g.SetDimensions(ma->image->GetDimension_S(), ma->image->GetDimension_R(), ma->image->GetDimension_C());
//        int value = 0;
//        for(unsigned int radius = 0; radius<300 && !value; radius++)
//        {
//            int rn,cn;
//            for(g.ForCircle(ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[1], ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[0],radius); g.GetCircle(radius+1, rn,cn); )
//            {
//                if(ma->image->operator()(0,rn,cn) >= 100 )
//                {
//                    value = ma->image->operator()(0,rn,cn);
//                    break;
//                }
//            }
//        }
//
//        std::cout<<std::endl<<" value = "<<value<<std::endl;
//
//        if(!value) return 0;
//
//        vmxCurve *curve = static_cast<vmxCurve*> (ma->GetDataListWidget()->Create("vmxCurve","Pa"));
//
//        mxList<int> list_c, list_r;
//        for( int c=0; c<ma->image->GetDimension_C(); c++)
//        {
//            for( int r=0; r<ma->image->GetDimension_R(); r++)
//            {
//                if(ma->image->operator()(r,c) == value )
//                {
//                    list_c.AddToEnd(c);
//                    list_r.AddToEnd(r);
//                }
//            }
//        }
//
//        curve->SetNumberOfSamples(list_c.GetNumberOfElements());
//        mxListIterator<int> itc, itr;
//        int i=0;
//        for(itc.SetToBegin(list_c), itr.SetToBegin(list_r), i=0; itc.IsValid() && itr.IsValid(); itc.MoveToNext(), itr.MoveToNext(), i++)
//        {
//            curve->X(i) = itc.GetElement();
//            curve->Y(i) = itr.GetElement();
//        }
//
//        curve->SetColor(255, 0,0,1);
//
//        curve->Update();
//
//        ma->RedoPlacement();
//
//        return 1;
//    };

//
//    static int Slot_CreatePdCurveButtonClicked(vmxGUIConnection *connection)
//    {
//        MainApp *ma = static_cast<MainApp*> (connection->GetSlotCallerObject());
//
//        std::cout<<" "<<ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[0]<<", "<<ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[1]<<", "<<ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[2];
//
//        mxGeometry g;
//        g.SetDimensions(ma->image2->GetDimension_S(), ma->image2->GetDimension_R(), ma->image2->GetDimension_C());
//        int value = 0;
//        for(unsigned int radius = 0; radius<300 && !value; radius++)
//        {
//            int rn,cn;
//            for(g.ForCircle(ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[1], ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[0],radius); g.GetCircle(radius+1, rn,cn); )
//            {
//                if(ma->image2->operator()(0,rn,cn) >= 100 )
//                {
//                    value = ma->image2->operator()(0,rn,cn);
//                    break;
//                }
//            }
//        }
//
//        std::cout<<std::endl<<" value = "<<value<<std::endl;
//
//        if(!value) return 0;
//
//        vmxCurve *curve = static_cast<vmxCurve*> (ma->GetDataListWidget()->Create("vmxCurve","Pd"));
//
//        mxList<int> list_c, list_r;
//        for( int c=0; c<ma->image2->GetDimension_C(); c++)
//        {
//            for( int r=0; r<ma->image2->GetDimension_R(); r++)
//            {
//                if(ma->image2->operator()(r,c) == value )
//                {
//                    list_c.AddToEnd(c);
//                    list_r.AddToEnd(r);
//                }
//            }
//        }
//
//        curve->SetNumberOfSamples(list_c.GetNumberOfElements());
//        mxListIterator<int> itc, itr;
//        int i=0;
//        for(itc.SetToBegin(list_c), itr.SetToBegin(list_r), i=0; itc.IsValid() && itr.IsValid(); itc.MoveToNext(), itr.MoveToNext(), i++)
//        {
//            curve->X(i) = itc.GetElement();
//            curve->Y(i) = itr.GetElement();
//        }
//
//        curve->SetColor(0,255,0,1);
//
//        curve->Update();
//
//        ma->RedoPlacement();
//
//        return 1;
//    };
//
    
    
    
    MainApp()
    {
        this->SetDataListWidgetToDefault();
        //this->SetRenderer3DToDefaultTrackballCamera();
        
        this->LoadFunctionFactoryList(&m_pwv_pa_pd_functions);
        

        
        vmxImage temp_image;
        //vmxImage *img1 = static_cast<vmxImage*> (
        this->GetDataListWidget()->Create(temp_image.GetClassName().Get_C_String(),"Image1");//);
        //vmxImage *img2 = static_cast<vmxImage*> (
        this->GetDataListWidget()->Create(temp_image.GetClassName().Get_C_String(),"Image2");//);

        
        m_image_viewer1.SetObjectName("ImageViewer1");
        this->RegisterViewer(&m_image_viewer1);
        
        
        m_image_viewer2.SetObjectName("ImageViewer2");
        this->RegisterViewer(&m_image_viewer2);

        
        this->RegisterViewer(&m_signal_viewer);

        
        
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

