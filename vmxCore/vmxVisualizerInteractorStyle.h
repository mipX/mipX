/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIInteractorStyleTrackballCamera.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/

#ifndef VMX_VISUALIZER_INTERACTOR_STYLE_H
  #define VMX_VISUALIZER_INTERACTOR_STYLE_H

//#include "vmxVisualizer.h"
#include "mxObject.h"
#include "mxString.h"

#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPNGWriter.h>
#include <vtkRegularPolygonSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkWindowToImageFilter.h>



class vmxVisualizer;
class vmxVisualizerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  private :
    int shit;

  public:
    

    //vtkSmartPointer<vtkWindowToImageFilter> wimg_filter;
    //vtkSmartPointer<vtkPNGWriter> png;
    vtkRenderer  *renderer;
    vtkRenderWindow *renWin;
    vtkRenderWindowInteractor *iren;

    //vmxVisualizer vis;

    /// Constructor 
     vmxVisualizerInteractorStyle();

    /// Destructor
    virtual ~vmxVisualizerInteractorStyle();

    static vmxVisualizerInteractorStyle* New();
    //vtkTypeMacro(vmxVisualizerInteractorStyle, vtkInteractorStyleTrackballCamera);

    vmxVisualizer *vis;

    void setVtkObjects(vtkRenderer *r,
                       vtkRenderWindow *W,
                       vtkRenderWindowInteractor *i);

    void setVisualizer(vmxVisualizer *vis);

    virtual void OnKeyPress();
    //virtual void OnLeftButtonDown();
};





#endif 
