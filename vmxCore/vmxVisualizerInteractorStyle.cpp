#include "vmxVisualizerInteractorStyle.h"
//#include "vtkObjectFactory.h"
#include "vmxVisualizer.h"

//vtkStandardNewMacro(vmxVisualizerInteractorStyle);

/****************** MOUSE INTERACTOR STYLE METHODS ***************************/

void vmxVisualizerInteractorStyle::setVtkObjects(
  vtkRenderer *r,
  vtkRenderWindow *W,
  vtkRenderWindowInteractor *i) 
{
  renderer = r;
  renWin = W;
  iren = i;
};

void vmxVisualizerInteractorStyle::setVisualizer(vmxVisualizer *visualizer) {
  vis = visualizer;
}

vmxVisualizerInteractorStyle::vmxVisualizerInteractorStyle() {
  shit = 0;

}


vmxVisualizerInteractorStyle::~vmxVisualizerInteractorStyle() {
 
}

vmxVisualizerInteractorStyle* vmxVisualizerInteractorStyle::New() {
  return new vmxVisualizerInteractorStyle;
}

void vmxVisualizerInteractorStyle::OnKeyPress() {
  // Event forwarding disabled, forward it manually for keypress you want.
  // Forward events
  //vtkInteractorStyleTrackballCamera::OnKeyPress();

  // Get the keypress
  //vtkRenderWindowInteractor *rwi = this->Interactor;
  char *keypress = iren->GetKeySym();
  mxString key;
  key.Assign(keypress);

  // Output the key that was pressed
  //std::cout << "Pressed " << key << std::endl;

  ////if(key=="p") {
    ////char* screenshot_path = TakeScreenshot();
  ////}
  if(key=="x") {
    vis->ToggleActiveDirectionAxis('x');
  }
  if(key=="y") {
    vis->ToggleActiveDirectionAxis('y');
  }
  if(key=="z") {
    vis->ToggleActiveDirectionAxis('z');
  }
  if(key=="Q") {
    exit(0);
  }

  //Handle arrow keys
  if(key=="Right") {
    //std::cout << "UP" << std::endl;
    vis->IncrementCenterlineBasedPlaneLocation();
  }
  if(key=="Left") {
    //std::cout << "NOT UP" << std::endl;
    vis->DecrementCenterlineBasedPlaneLocation();
  }
  if(key=="Up") {
    //std::cout << "UP" << std::endl;
    vis->IncrementDirectionOnActiveAxis();
  }
  if(key=="Down") {
    //std::cout << "NOT UP" << std::endl;
    vis->DecrementDirectionOnActiveAxis();
  }
  if(key=="Return") {
    std::cout << "Selecting the cutting plane" << std::endl;
    vis->SetSelectedPlaneLocation();
  }
  if(key=="p") {
    std::cout << "Saving screenshot"  << std::endl;
    vis->SaveScreenshot();
  }
    
    
    



  //else if(key=="v")
  //{
  //	show++;
  //	int active=0;
  //	for(int i=0; i<10; i++)
  //		if( active_meshes[i] )
  //			active++;
  //	if(show >= active )
  //		show = -1;

  //	if(show == -1)
  //		for(int i=0; i<10; i++) {
  //			if(active_meshes[i]) {
  //				mesh
  //			}
  //				

  //		}
  //}
};

//void VisualizerMouseInteractorStyle::OnLeftButtonDown() 
//{
  //if(pick_state) {
    //pickPoint();
    //setPickState(0);
  //}
  //else {
    //vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
  //}

  //this->Interactor->Render();
//};

//char* VisualizerMouseInteractorStyle::TakeScreenshot() {
  //wimg_filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
  //png = vtkSmartPointer<vtkPNGWriter>::New();

  //wimg_filter->SetInput(renWin);
  //wimg_filter->SetMagnification(2);
  //wimg_filter->SetInputBufferTypeToRGBA();
  //wimg_filter->Update();

  //char ts[100];
  //getTimestamp(ts);
  //char *filename = new char[300];
  //sprintf(filename, "D:\\Dropbox\\Research\\laa\\work\\LAAvtkApp\\screenshots\\%s.png", ts);
  //bdString text;
  //text = "Saving file ";
  //text.Append(filename);
  //log(text.C_String());

  //iren->Render();

  //png->SetInputConnection(wimg_filter->GetOutputPort());
  //png->SetFileName(filename);
  //png->Update();
  //png->Write();
  //return filename;
//};
