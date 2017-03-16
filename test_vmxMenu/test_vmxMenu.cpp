/*=========================================================================
 
 Program:   mipx
 Module:    test_vmxMenu.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/


#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)



#include <vtkSmartPointer.h>
#include <vtkAxes.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPropPicker.h>
#include <vtkPicker.h>
#include <vtkCellPicker.h>
#include <vtkImagePlaneWidget.h>
#include <vtkLookupTable.h>
#include <vtkSphereSource.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCommand.h>
#include <vtkProperty.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkCallbackCommand.h>


#include <iostream>
#include <sstream>
#include <chrono>

#include "mxArray.h"
#include "mxList.h"

#include "vmxMenu.h"
#include "vmxTextInput.h"





int main()
{
	//===== THIS IS GENERAL VTK PART - NEEDED FOR ALL EXAMPLES =====
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> render_window = vtkSmartPointer<vtkRenderWindow>::New();
	render_window->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(render_window);
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	interactor->SetInteractorStyle(style);
	render_window->SetSize(900, 900);
	//iren->Initialize();//Initialize the interactor here in order for the SplineWighet to work properly
	//==============================================================
    
    

    
    vmxClipBoard cb;
    
    vmxMenu menu;
    menu.SetClipBoard(&cb);
    menu.SetOrigin(0,30);
    menu.SetColor(1,0,0);
    menu.SetFontSize(20);
    menu.AddItem("Hello world",3);
    menu.AddItem("Something new");
    menu.AddItem("Another thing");
    menu.AddItem("Yet another one",1);
    menu.AddItem("Hello world",2);
    menu.AddItem("Something new");
    menu.AddItem("Another thing");
    menu.AddItem("Yet another one");
    menu.AddItem("Hello world}");
    menu.AddItem("Something new>");
    menu.AddItem("Another thing>");
    menu.AddItem("Yet another one");
    menu.AddItem("Hello world",3);
    menu.AddItem("Something new");
    menu.AddItem("Another thing");
    menu.AddItem("Yet another one",3);
    menu.AddItem("Hello world",1);
    menu.AddItem("Something new");
    menu.AddItem("Another thing",2);
    menu.AddItem("Yet another one");
    menu.BuildMenu();
    menu.SetInteractor(interactor);
    //menu.SetPlacementToLowerRight();
    //menu.SetPlacementToUpperRight();
    //menu.SetPlacementToUpperLeft();
    //menu.SetPlacementToLowerCenter();
    //menu.SetPlacementToUpperCenter();
    //menu.SetPlacementToCenterLeft();
    menu.SetPlacementToCenterRight();
    //menu.SetPlacementToRelative(100, 50);
    menu.SetFontSize(16);
    
    
    
    vmxTextInput text_input;
    text_input.SetClipBoard(&cb);
    text_input.SetOrigin(0,30);
    text_input.SetColor(1,0,0);
    text_input.SetFontSize(20);
    text_input.SetInteractor(interactor);
    text_input.SetDescriptionText("Input: ");
    text_input.ShowInputText();
    text_input.SetVisibility(1);
    
    
 

	//===== START THE RENDERING - NEEDED FOR ALL EXAMPLES =====
	render_window->Render();
	interactor->Start();
	//=====
    
    return 1;
}


