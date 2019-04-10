/*=========================================================================
 
 Program:   mipx
 Module:    example_App_PaPd.cpp
 
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



class MainApp : public vmxAppMainWidget
{
public:
    
    mxScopedPointer< vmxImage > image;
    mxScopedPointer< vmxImage > image2;
    
    
    vmxAppImageViewer m_image_viewer;
    vmxAppImageViewer m_image_viewer2;
    
    vmxAppSignalViewer m_signal_viewer;
    
    
    vmxGUIButtonGroup m_create_Pa_curve_push_button;
    vmxGUIButtonGroup m_create_Pd_curve_push_button;
    
    
public:
    
    
    static int Slot_CreatePaCurveButtonClicked(vmxGUIConnection *connection)
    {
        MainApp *ma = static_cast<MainApp*> (connection->GetSlotCallerObject());
        
        std::cout<<" "<<ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[0]<<", "<<ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[1]<<", "<<ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[2];
        
        mxGeometry g;
        g.SetDimensions(ma->image->GetDimension_S(), ma->image->GetDimension_R(), ma->image->GetDimension_C());
        int value = 0;
        for(unsigned int radius = 0; radius<300 && !value; radius++)
        {
            int rn,cn;
            for(g.ForCircle(ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[1], ma->m_image_viewer.GetRenderer()->GetPickedIndexes()[0],radius); g.GetCircle(radius+1, rn,cn); )
            {
                if(ma->image->operator()(0,rn,cn) >= 100 )
                {
                    value = ma->image->operator()(0,rn,cn);
                    break;
                }
            }
        }
        
        std::cout<<std::endl<<" value = "<<value<<std::endl;
        
        if(!value) return 0;
        
        vmxCurve *curve = static_cast<vmxCurve*> (ma->GetDataListWidget()->Create("vmxCurve","Pa"));
        
        mxList<int> list_c, list_r;
        for( int c=0; c<ma->image->GetDimension_C(); c++)
        {
            for( int r=0; r<ma->image->GetDimension_R(); r++)
            {
                if(ma->image->operator()(r,c) == value )
                {
                    list_c.AddToEnd(c);
                    list_r.AddToEnd(r);
                }
            }
        }
        
        curve->SetNumberOfSamples(list_c.GetNumberOfElements());
        mxListIterator<int> itc, itr;
        int i=0;
        for(itc.SetToBegin(list_c), itr.SetToBegin(list_r), i=0; itc.IsValid() && itr.IsValid(); itc.MoveToNext(), itr.MoveToNext(), i++)
        {
            curve->X(i) = itc.GetElement();
            curve->Y(i) = itr.GetElement();
        }
        
        curve->SetColor(255, 0,0,1);
        
        curve->Update();
        
        ma->RedoPlacement();
        
        return 1;
    };

    
    static int Slot_CreatePdCurveButtonClicked(vmxGUIConnection *connection)
    {
        MainApp *ma = static_cast<MainApp*> (connection->GetSlotCallerObject());
        
        std::cout<<" "<<ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[0]<<", "<<ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[1]<<", "<<ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[2];
        
        mxGeometry g;
        g.SetDimensions(ma->image2->GetDimension_S(), ma->image2->GetDimension_R(), ma->image2->GetDimension_C());
        int value = 0;
        for(unsigned int radius = 0; radius<300 && !value; radius++)
        {
            int rn,cn;
            for(g.ForCircle(ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[1], ma->m_image_viewer2.GetRenderer()->GetPickedIndexes()[0],radius); g.GetCircle(radius+1, rn,cn); )
            {
                if(ma->image2->operator()(0,rn,cn) >= 100 )
                {
                    value = ma->image2->operator()(0,rn,cn);
                    break;
                }
            }
        }
        
        std::cout<<std::endl<<" value = "<<value<<std::endl;
        
        if(!value) return 0;
        
        vmxCurve *curve = static_cast<vmxCurve*> (ma->GetDataListWidget()->Create("vmxCurve","Pd"));
        
        mxList<int> list_c, list_r;
        for( int c=0; c<ma->image2->GetDimension_C(); c++)
        {
            for( int r=0; r<ma->image2->GetDimension_R(); r++)
            {
                if(ma->image2->operator()(r,c) == value )
                {
                    list_c.AddToEnd(c);
                    list_r.AddToEnd(r);
                }
            }
        }
        
        curve->SetNumberOfSamples(list_c.GetNumberOfElements());
        mxListIterator<int> itc, itr;
        int i=0;
        for(itc.SetToBegin(list_c), itr.SetToBegin(list_r), i=0; itc.IsValid() && itr.IsValid(); itc.MoveToNext(), itr.MoveToNext(), i++)
        {
            curve->X(i) = itc.GetElement();
            curve->Y(i) = itr.GetElement();
        }
        
        curve->SetColor(0,255,0,1);
        
        curve->Update();
        
        ma->RedoPlacement();
        
        return 1;
    };

    
    
    
    MainApp()
    {
        this->SetDataListWidgetToDefault();
        //this->SetRenderer3DToDefaultTrackballCamera();
        
        // Register the vmxImage16UFactory with the data list widget.
        // The widget will later use this factory to construct objects of the given type.
        this->GetDataListWidget()->AddFactory(new vmxImage16UFactory);
        this->GetDataListWidget()->AddFactory(new vmxCurveFactory);
        
        
        image = new vmxImage();
        image2 = new vmxImage();
        
        
        
        vtkSmartPointer<vtkPNGReader> reader = vtkSmartPointer<vtkPNGReader>::New();
        reader->SetFileName("/Users/danilobabin/Dropbox/-DIP_IMAGES/-CORONARY_FFR_PWV/2019_03_20_FFR_LAD_65.72.mm/FFR LAD nakon adenozina 400 mcg.png");
        reader->Update();
        
        vtkSmartPointer<vtkImageExtractComponents> extract_red_filter = vtkSmartPointer<vtkImageExtractComponents>::New();
        extract_red_filter->SetInputConnection(reader->GetOutputPort());
        extract_red_filter->SetComponents(0);
        extract_red_filter->Update();
        
        
        vtkSmartPointer<vtkImageExtractComponents> extract_green_filter = vtkSmartPointer<vtkImageExtractComponents>::New();
        extract_green_filter->SetInputConnection(reader->GetOutputPort());
        extract_green_filter->SetComponents(1);
        extract_green_filter->Update();
        
        
        vtkSmartPointer< vtkImageData > image_red = vtkSmartPointer< vtkImageData >::New();
        image_red->DeepCopy(extract_red_filter->GetOutput());
        
        vtkSmartPointer< vtkImageData > image_green = vtkSmartPointer< vtkImageData >::New();
        image_green->DeepCopy(extract_green_filter->GetOutput());
        
        image->SetDimensions(1,1,image_red->GetDimensions()[1], image_red->GetDimensions()[0]);
        {
            int i=0;
            for(int r=0; r<image->GetDimension_R(); r++)
            {
                for(int c=0; c<image->GetDimension_C(); c++)
                {
                    image->operator()(r,c) = *((unsigned short*) image_red->GetScalarPointer(c, r, 0));
                    i++;
                }
            }
        }

        
        image2->SetDimensions(1,1,image_green->GetDimensions()[1], image_green->GetDimensions()[0]);
        {
            int i=0;
            for(int r=0; r<image2->GetDimension_R(); r++)
            {
                for(int c=0; c<image2->GetDimension_C(); c++)
                {
                    image2->operator()(r,c) = *((unsigned short*) image_green->GetScalarPointer(c, r, 0));
                    i++;
                }
            }
        }
        
       
        this->RegisterViewer(&m_image_viewer);
        m_image_viewer.GetRenderer()->SetImageData(image_red);
        m_image_viewer.GetRenderer()->SetDirectionToTransversal();
        m_image_viewer.GetRenderer()->SetMappingToGrayScale();
        m_image_viewer.GetRenderer()->FitImageToScreen();

        
        
        this->RegisterViewer(&m_image_viewer2);
        m_image_viewer2.GetRenderer()->SetImageData(image_green);
        m_image_viewer2.GetRenderer()->SetDirectionToTransversal();
        m_image_viewer2.GetRenderer()->SetMappingToGrayScale();
        m_image_viewer2.GetRenderer()->FitImageToScreen();

        
        this->RegisterViewer(&m_signal_viewer);

        
        
        

        
        m_create_Pa_curve_push_button.SetMainWidget(this);
        m_create_Pa_curve_push_button.SetButtonTypeToPushButton();
        m_create_Pa_curve_push_button.SetDescriptionText(" ");
        m_create_Pa_curve_push_button.AddItem("Create Curve Pa");
        m_create_Pa_curve_push_button.SetColor(1,1,0);
        m_create_Pa_curve_push_button.SetFontSize(20);
        m_create_Pa_curve_push_button.SetPlacementToLowerLeft();
        m_create_Pa_curve_push_button.SetPlacementOrderToBelow();
        m_create_Pa_curve_push_button.SetVisibility(1);

        m_create_Pd_curve_push_button.SetMainWidget(this);
        m_create_Pd_curve_push_button.SetButtonTypeToPushButton();
        m_create_Pd_curve_push_button.SetDescriptionText(" ");
        m_create_Pd_curve_push_button.AddItem("Create Curve Pd");
        m_create_Pd_curve_push_button.SetColor(1,1,0);
        m_create_Pd_curve_push_button.SetFontSize(20);
        m_create_Pd_curve_push_button.SetPlacementToLowerLeft();
        m_create_Pd_curve_push_button.SetPlacementOrderToBelow();
        m_create_Pd_curve_push_button.SetVisibility(1);

        
        {
            //vmxGUIConnection *c =
            vmxGUIConnection::New(&m_create_Pa_curve_push_button, ButtonClickedEvent, &m_create_Pa_curve_push_button, this, MainApp::Slot_CreatePaCurveButtonClicked);
        }
        {
            //vmxGUIConnection *c =
            vmxGUIConnection::New(&m_create_Pd_curve_push_button, ButtonClickedEvent, &m_create_Pd_curve_push_button, this, MainApp::Slot_CreatePdCurveButtonClicked);
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

