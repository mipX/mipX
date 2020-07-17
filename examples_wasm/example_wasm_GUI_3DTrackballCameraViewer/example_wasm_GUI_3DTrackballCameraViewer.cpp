/*=========================================================================
  Program:   Visualization Toolkit
  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/

#include "vtkActor.h"
#include "vtkConeSource.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkNew.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkSDL2OpenGLRenderWindow.h"
#include "vtkSDL2RenderWindowInteractor.h"

#include <iostream>
//#include "mxImage.h"
#include "mxString.h"
#include "mxPoint.h"
#include "mxDataObject.h"
#include "mxObject.h"
//#include "mxBasicImage.h"
#include "mxImage.h"

//------------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  std::cout << "It werks" << std::endl;
  mxImage img;
  img.SetDimensions(1,10,10,10);
  img.FillInWith(1);
  //for (unsigned int s = 0; s < img.GetDimension_S(); ++s) {
    //for (unsigned int r = 0; r < img.GetDimension_R(); ++r) {
      //for (unsigned int c = 0; c < img.GetDimension_C(); ++c) {
        //img(s,r,c) = 1;
      //}
    //}
  //}

  int sum=0;
  for (unsigned int s = 0; s < img.GetDimension_S(); ++s) {
    for (unsigned int r = 0; r < img.GetDimension_R(); ++r) {
      for (unsigned int c = 0; c < img.GetDimension_C(); ++c) {
        sum += img(s,r,c);
      }
    }
  }

  std::cout << "Sum of the image is: " << sum << std::endl;
  
  mxString test;
  test.Assign("Test mxStringa");
  std::cout << test << std::endl;

  mxPoint p;
  p.S() = 100;
  p.R() = 90;
  p.C() = 80;

  std::cout << "Point: " << p.S() << " " << p.R() << " " << p.C() << std::endl;
    
  mxDataObject obj;
  obj.GetObjectName().Assign("Neki object");

  std::cout << "mxDO: " << obj.GetObjectName() << std::endl;
  //
  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkSDL2OpenGLRenderWindow> renderWindow;
  renderWindow->SetMultiSamples(0);
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkSDL2RenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  vtkNew<vtkInteractorStyleTrackballCamera> style;
  renderWindowInteractor->SetInteractorStyle(style);
  style->SetDefaultRenderer(renderer);

  // Create pipeline
  vtkNew<vtkConeSource> coneSource;
  coneSource->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(coneSource->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  // Add the actors to the scene
  renderer->AddActor(actor);

  // Start rendering app
  renderer->SetBackground(0.2, 0.3, 0.4);
  renderWindow->SetSize(300, 300);
  renderWindow->Render();

  // Start event loop
  renderWindowInteractor->Start();

  return 0;
}
