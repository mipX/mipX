/*=========================================================================
 
 Program:   mipx
 Module:    example_GUI_3DTrackballCameraViewer.cpp
 
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
 
 This example is the most basic use case: 3D scene with an actor.
 
 */




#include "vmxGUIMenu.h"
#include "vmxGUIMenuBar.h"
#include "vmxGUIFilesDialog.h"
#include "vmxGUILabel.h"
#include "vmxGUIListWidget.h"
#include "vmxGUIInputWidget.h"
#include "vmxGUIButtonGroup.h"
#include "vmxGUIInteractorStyle.h"
#include "vmxGUIRenderer3DTrackballCamera.h"


#include <vtkSphereSource.h>





class MainApp : public vmxGUIMainWidget
{
    
public:
    
    vmxGUIRenderer3DTrackballCamera *m_3D_renderer;
    
 
    MainApp()
    {
        // Initialization.
        {
            vmxGUIConnection::internal_SetMainWidget(this);
        }
        
        
        m_3D_renderer = new vmxGUIRenderer3DTrackballCamera(this);
        this->AddRenderer(m_3D_renderer);


        //vtkSmartPointer<vtkSphereSource> sphere_source = vtkSmartPointer<vtkSphereSource>::New();
        //sphere_source->SetCenter(0.0, 0.0, 0.0);
        //sphere_source->SetRadius(50.0);
        
        //vtkSmartPointer<vtkPolyDataMapper> sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        //sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());
        
        //vtkSmartPointer<vtkActor> sphere_actor = vtkSmartPointer<vtkActor>::New();
        //sphere_actor->SetMapper(sphere_mapper);


        //m_3D_renderer->GetVTKRenderer()->AddActor(sphere_actor);
        
        //m_3D_renderer->SetPickMarkerVisibility(1);

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

