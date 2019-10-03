/*=========================================================================
 
 Program:   mipx
 Module:    example_GUI_SignalViewer.cpp
 
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
 
 This example illustrates how to create a simple app with signal viewer to view xy curves (signals).
 
 */




#include "vmxGUIMenu.h"
#include "vmxGUIMenuBar.h"
#include "vmxGUIFilesDialog.h"
#include "vmxGUILabel.h"
#include "vmxGUIListWidget.h"
#include "vmxGUIInputWidget.h"
#include "vmxGUIButtonGroup.h"
#include "vmxGUIInteractorStyle.h"
#include "vmxGUIRendererSignalViewer.h"


#include <vtkGenericDataObjectReader.h>
#include <vtkMath.h>






class MainApp : public vmxGUIMainWidget
{
    
public:
    
    vmxGUIRendererSignalViewer *m_signal_renderer;
    
 
    MainApp()
    {
        // Initialization.
        {
            vmxGUIConnection::internal_SetMainWidget(this);
        }
        
        
        m_signal_renderer = new vmxGUIRendererSignalViewer(this);
        this->AddRenderer(m_signal_renderer);
        
        
        vtkSmartPointer<vtkDoubleArray> array_y = vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> array_x = vtkSmartPointer<vtkDoubleArray>::New();
        
        vtkSmartPointer<vtkDoubleArray> array_y2 = vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> array_x2 = vtkSmartPointer<vtkDoubleArray>::New();

        vtkSmartPointer<vtkDoubleArray> array_y3 = vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> array_x3 = vtkSmartPointer<vtkDoubleArray>::New();

        for (int b = 0; b < 30; b++)   /// Assuming an array of 30 elements
        {
            {
                double val = vtkMath::Random(0.0,2.0);
                array_y->InsertValue(b,val);
                array_x->InsertValue(b,b-10);
            }
            {
                double val = vtkMath::Random(0.0,5.0);
                array_y2->InsertValue(b,val);
                array_x2->InsertValue(b,b+15);
            }
            {
                array_y3->InsertValue(b,b/5);
                array_x3->InsertValue(b,b+5);
            }
        }
        
        
        m_signal_renderer->AddInputData(array_x, array_y,"try","x","y", 255,0,0);
        m_signal_renderer->AddInputData(array_x2, array_y2,"try2","x2","y2", 0,255,0);
        m_signal_renderer->AddInputData(array_x3, array_y3,"try3","x3","y3",0,0,255);
        
        m_signal_renderer->Update();

    };
    
    
    ~MainApp(){};
    
};






int main()
{
    // Create the main app.
    MainApp main_app;
    
    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
    main_app.StartInteraction();
    
    return 1;
};




