#include "vmxVisualizer.h"
#include <time.h>

void normalizeVector(double vec[3]) 
{
	double magnitude = std::sqrt(  pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2) );
  vec[0] /= magnitude;
  vec[1] /= magnitude;
  vec[2] /= magnitude;
}


//void vmxVisualizer::setColors() {
    //colors[0][0] = 0.341;    
    //colors[0][1] = 0.827;    
    //colors[0][2] = 0.133;    
    //colors[1][0] = 0.133;    
    //colors[1][1] = 0.780;    
    //colors[1][2] = 0.827;    
    //colors[2][0] = 0.133;    
    //colors[2][1] = 0.145;    
    //colors[2][2] = 0.827;    
    //colors[3][0] = 0.827;    
    //colors[3][1] = 0.133;    
    //colors[3][2] = 0.733;    
    //colors[4][0] = 0.318;    
    //colors[4][1] = 0.063;    
    //colors[4][2] = 0.396;    
    //colors[5][0] = 0.980;    
    //colors[5][1] = 0.047;    
    //colors[5][2] = 0.000;    
    //colors[6][0] = 0.980;    
    //colors[6][1] = 0.573;    
    //colors[6][2] = 0.000;    
    //colors[7][0] = 0.200;    
    //colors[7][1] = 0.859;    
    //colors[7][2] = 0.000;    
    //colors[8][0] = 0.000;    
    //colors[8][1] = 0.600;    
    //colors[8][2] = 0.878;    
    //colors[9][0] = 0.302;    
    //colors[9][1] = 0.580;    
    //colors[9][2] = 0.298;    
    //colors[10][0] = 0.706;   
    //colors[10][1] = 0.431;   
    //colors[10][2] = 0.588;   
    //colors[11][0] = 0.831;   
    //colors[11][1] = 0.980;   
    //colors[11][2] = 0.000;   
//}

void vmxVisualizer::setColors() {
  /// Lifted from: http://ksrowell.com/blog-visualizing-data/2012/02/02/optimal-colors-for-graphs/
  /// Uses rgb in range 0-255
  //red
    colors[0][0]  = 255;
    colors[0][1]  = 0; 
    colors[0][2]  = 0; 
  //blue
    colors[1][0]  = 57;   
    colors[1][1]  = 106;  
    colors[1][2]  = 177; 
  //orange
    colors[2][0]  = 218; 
    colors[2][1]  = 124;  
    colors[2][2]  = 48;  
  //green               
    colors[3][0]  = 62;   
    colors[3][1]  = 150; 
    colors[3][2]  = 81;  
  //red                     
    colors[4][0]  = 204; 
    colors[4][1]  = 37;  
    colors[4][2]  = 41;  
  //gray                  
    colors[5][0]  = 83;   
    colors[5][1]  = 81;  
    colors[5][2]  = 84;   
  //magenta                 
    colors[6][0]  = 107;  
    colors[6][1]  = 76;  
    colors[6][2]  = 154; 
  //darkred                  
    colors[7][0]  = 146;    
    colors[7][1]  = 36;     
    colors[7][2]  = 40;    
  //oker                  
    colors[8][0]  = 148;     
    colors[8][1]  = 139;     
    colors[8][2]  = 61;      
  //cyan
    colors[9][0]  = 34;    
    colors[9][1]  = 192;    
    colors[9][2]  = 241;    
  //red
    colors[10][0] = 0.706;   
    colors[10][1] = 0.431;   
    colors[10][2] = 0.588;   

    colors[11][0] = 0.831;   
    colors[11][1] = 0.980;   
    colors[11][2] = 0.000;   

    for (int i = 0; i < 10; ++i) {
      colors[i][0] /= 255;
      colors[i][1] /= 255;
      colors[i][2] /= 255;
    }
}

int vmxVisualizer::getEnvBool( const char * var ) {
     const char * val = std::getenv( var );
     if (val == 0)
       return 0;
     else if ( val[0] == '1' )
         return 1;
     else {
         return 0;
     }
};

const char * vmxVisualizer::getEnv( const char * var ) {
     const char * val = ::getenv( var );
     if ( val == 0 ) {
         return "";
     }
     else {
         return val;
     }
};

vmxVisualizer::vmxVisualizer() {
    if(this->getEnvBool("mipxDISABLE_VISUALIZE"))
      return;
  //
    //Interactor
    style = vtkSmartPointer<vmxVisualizerInteractorStyle>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = main_widget.GetInteractor();
    interactor->SetInteractorStyle(style);
    //Register vtk objects
    style->setVtkObjects(main_widget.GetRenderer_3D(), main_widget.GetRenderWindow(), interactor);
    style->setVisualizer(this);
    //
    // Initialize meshes to 0
    for(int i=0; i<10; i++) {
      active_meshes[i] = 0;
    }
    // Initialize planes to 0
    for(int i=0; i<10; i++) {
      active_planes[i] = 0;
      planes[i] = NULL;
      plane_actors[i] = NULL;
    }
    // Initalize centerline based plane. -1 -> centerline not used
    active_centerline_plane = -1;
    radius_image = NULL;
    image_for_centerline = NULL;
    //
    // Initialize lines to 0
    for(int i=0; i<100; i++) {
      active_lines[i] = 0;
      lines[i] = NULL;
      line_actors[i] = NULL;
    }

    main_widget.GetRenderer_3D()->SetBackground(0.1,0.1,0.1);
    main_widget.GetRenderer_3D()->SetBackground2(0,0,0.3);
    main_widget.GetRenderer_3D()->SetGradientBackground(1);

    
    //----- Data List Widget -----
    v_tree.GetListWidget()->SetMainWidget(&main_widget);
    v_tree.GetListWidget()->SetColor(1,1,0);
    v_tree.GetListWidget()->SetFontSize(18);
    v_tree.GetListWidget()->SetVisibility(1);
    v_tree.GetListWidget()->SetPlacementToUpperRight();
    
    this->setColors();
    
    // Register the vmxImage16UFactory with the data list widget.
    // The widget will later use this factory to construct objects of the given type.
    v_tree.AddFactory(new vmxImage16UFactory);
    for (int i = 0; i < 10; i++) {
      images[i] = NULL;
    }
    
    // Register the vmxMeshFactory with the data list widget.
    // The widget will later use this factory to construct objects of the given type.
    v_tree.AddFactory(new vmxMeshFactory);

    // Create a mesh using the data list widget.
    for (int i = 0; i < 10; i++) {
      meshes[i] = NULL;
    }

    saved_screenshots = 0;

    //plane = NULL;
};


vmxVisualizer::~vmxVisualizer() {
};

void vmxVisualizer::SetMesh(int number, mxImage &img, int mesh_value, double opacity, const char *img_name)  
{
  this->SetMesh(number, img, mesh_value, opacity, colors[number][0], colors[number][1], colors[number][2], img_name);
}


void vmxVisualizer::SetMesh(int number, mxImage &img, int mesh_value, double opacity, double red, double green, double blue, const char *img_name) 
{
    if(this->getEnvBool("mipxDISABLE_VISUALIZE"))
      return;
  active_meshes[number] = 1;
  //
  // Release previous mesh at number if necessary 
  if (images[number]) {
    v_tree.Release(images[number]);
    v_tree.Release(meshes[number]);
  }
  mxString mesh_name;
  mesh_name.Append(img_name);
  mesh_name.Append("_mesh");

  // Create image plane
  images[number] = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U",img_name));
  images[number]->CopyFromDataObject(&img);
  images[number]->SetMappingToOpaqueGrayScale(); //vimg->SetMappingToOpaqueColor();
  images[number]->SetVisibility(0);

  // Create mesh 
  meshes[number] = static_cast<vmxMesh*> (v_tree.Create("vmxMesh",mesh_name.Get_C_String()));
  meshes[number]->CreatePolyData16(images[number], mesh_value);
  meshes[number]->CreateActorByLookupTableScalarColoring(0,255);
  meshes[number]->SetVisibility(1);
  meshes[number]->SetOpacity(opacity);
  meshes[number]->SetColor(red, green, blue);


};

void vmxVisualizer::SetMeshByLabels(int number, mxImage &img, int mesh_value, double opacity, const char *img_name) 
{
    if(this->getEnvBool("mipxDISABLE_VISUALIZE"))
      return;
  active_meshes[number] = 1;
  //
  // Release previous mesh at number if necessary 
  if (images[number]) {
    v_tree.Release(images[number]);
    v_tree.Release(meshes[number]);
  }
  mxString mesh_name;
  mesh_name.Append(img_name);
  mesh_name.Append("_mesh");

  // Create image plane
  images[number] = static_cast<vmxImage16U*> (v_tree.Create("vmxImage16U",img_name));
  images[number]->CopyFromDataObject(&img);
  images[number]->SetMappingToOpaqueGrayScale(); //vimg->SetMappingToOpaqueColor();
  images[number]->SetVisibility(0);

  // Create mesh 
  meshes[number] = static_cast<vmxMesh*> (v_tree.Create("vmxMesh",mesh_name.Get_C_String()));
  meshes[number]->CreatePolyData16(images[number], mesh_value);
  meshes[number]->CreateActorByDifferentlyColoringDisjointComponents();
  meshes[number]->SetVisibility(1);
  meshes[number]->SetOpacity(opacity);


};


void vmxVisualizer::ShowImage(int number){
    if(this->getEnvBool("mipxDISABLE_VISUALIZE"))
      return;
  if (images[number]) {
    images[number]->SetVisibility(1);
  }
};
void vmxVisualizer::HideImage(int number){
    if(this->getEnvBool("mipxDISABLE_VISUALIZE"))
      return;
  if (images[number]) {
    images[number]->SetVisibility(0);
  }
};

void vmxVisualizer::ShowMesh(int number){
    if(this->getEnvBool("mipxDISABLE_VISUALIZE"))
      return;
  if (meshes[number]) {
    meshes[number]->SetVisibility(1);
  }
};
void vmxVisualizer::HideMesh(int number){
    if(this->getEnvBool("mipxDISABLE_VISUALIZE"))
      return;
  if (meshes[number]) {
    meshes[number]->SetVisibility(0);
  }
};

void vmxVisualizer::SetLine(int number, mxImage &image, double *normal_xyz, double *center_xyz, int width) 
{
  //
  //get realworld coordinates for center of the plane
  double realworld_center_xyz[3];
  image.WorldCoordinatesFromPoint(center_xyz[2], 
                                    center_xyz[1],
                                    center_xyz[0],
                                    realworld_center_xyz[2],
                                    realworld_center_xyz[1],
                                    realworld_center_xyz[0]);
  //
  //unsigned int imgcenter[3];
  //double realworld_imgcenter[3];
  //imgcenter[2] = image.GetDimension_S()/2;
  //imgcenter[1] = image.GetDimension_R()/2;
  //imgcenter[0] = image.GetDimension_C()/2;
  //image.WorldCoordinatesFromIndexes(imgcenter[2], 
                                    //imgcenter[1],
                                    //imgcenter[0],
                                    //realworld_imgcenter[2],
                                    //realworld_imgcenter[1],
                                    //realworld_imgcenter[0]);
  double tempnormal[3];
  double realworld_normal_xyz[3];
  for(int i=0; i<3; i++) 
    tempnormal[i] = center_xyz[i] +  100*normal_xyz[i];
  image.WorldCoordinatesFromPoint(tempnormal[2], 
                                    tempnormal[1],
                                    tempnormal[0],
                                    realworld_normal_xyz[2],
                                    realworld_normal_xyz[1],
                                    realworld_normal_xyz[0]);
  //image.WorldCoordinatesFromIndexes(normal_xyz[2], 
                                    //normal_xyz[1],
                                    //normal_xyz[0],
                                    //realworld_normal_xyz[2],
                                    //realworld_normal_xyz[1],
                                    //realworld_normal_xyz[0]);
  //for(int i=0; i<3; i++) {
    //realworld_normal_xyz[i] -= realworld_center_xyz[i];
    //normalizeVector(realworld_normal_xyz);
  //}



  if(lines[number] == NULL) {
    vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
    lineSource->SetPoint1(realworld_center_xyz);
    lineSource->SetPoint2(realworld_normal_xyz);
    lineSource->Update();
    lines[number] = lineSource;


    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(lineSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

    actor->GetProperty()->SetLineWidth(width);
    //actor->GetProperty()->SetColor(colors[number][0],colors[number][1],colors[number][2]);
    //actor->GetProperty()->SetColor(colors[number%10][0],colors[number%10][1],colors[number%10][2]);
    if (number && number < 10 ) {
      actor->GetProperty()->SetColor(colors[number%10][0],colors[number%10][1],colors[number%10][2]);
      //actor->GetProperty()->SetColor(0,1,0);
    } else if (number && number >= 10 ) {
      actor->GetProperty()->SetColor(0,0,0);
    } else {
      actor->GetProperty()->SetColor(0,0,1);
    }

    actor->GetProperty()->SetOpacity(1);

    actor->SetMapper(mapper);
    main_widget.GetRenderer_3D()->AddActor(actor);
    line_actors[number] = actor;
  }
  else {
    lines[number]->SetPoint1(realworld_center_xyz);
    lines[number]->SetPoint2(realworld_normal_xyz);
    //lines[number]->Update();
  }

};

void vmxVisualizer::SetLine(int number, mxImage &image, mxPosition normal, mxPosition center, int width) 
{
  double normal_xyz[3];
  double center_xyz[3];
  normal_xyz[2] = normal.S();
  normal_xyz[1] = normal.R();
  normal_xyz[0] = normal.C();
  center_xyz[2] = center.S();
  center_xyz[1] = center.R();
  center_xyz[0] = center.C();
  this->SetLine(number, image, normal_xyz, center_xyz, width);
}


void vmxVisualizer::SetPlane(int number, mxImage &image, double *normal_xyz, double *center_xyz, int radius) 
{
  //
  //get realworld coordinates for center of the plane
  double realworld_center_xyz[3];
  image.WorldCoordinatesFromPoint(center_xyz[2], 
                                    center_xyz[1],
                                    center_xyz[0],
                                    realworld_center_xyz[2],
                                    realworld_center_xyz[1],
                                    realworld_center_xyz[0]);
  //
  //unsigned int imgcenter[3];
  //double realworld_imgcenter[3];
  //imgcenter[2] = image.GetDimension_S()/2;
  //imgcenter[1] = image.GetDimension_R()/2;
  //imgcenter[0] = image.GetDimension_C()/2;
  //image.WorldCoordinatesFromIndexes(imgcenter[2], 
                                    //imgcenter[1],
                                    //imgcenter[0],
                                    //realworld_imgcenter[2],
                                    //realworld_imgcenter[1],
                                    //realworld_imgcenter[0]);
  double tempnormal[3];
  double realworld_normal_xyz[3];
  for(int i=0; i<3; i++) 
    tempnormal[i] = center_xyz[i] +  100*normal_xyz[i];
  image.WorldCoordinatesFromPoint(tempnormal[2], 
                                    tempnormal[1],
                                    tempnormal[0],
                                    realworld_normal_xyz[2],
                                    realworld_normal_xyz[1],
                                    realworld_normal_xyz[0]);
  //image.WorldCoordinatesFromIndexes(normal_xyz[2], 
                                    //normal_xyz[1],
                                    //normal_xyz[0],
                                    //realworld_normal_xyz[2],
                                    //realworld_normal_xyz[1],
                                    //realworld_normal_xyz[0]);
  for(int i=0; i<3; i++) 
    realworld_normal_xyz[i] -= realworld_center_xyz[i];
  //normalizeVector(realworld_normal_xyz);



  if(planes[number] == NULL) {
    vtkSmartPointer<vtkRegularPolygonSource> planeSource = vtkSmartPointer<vtkRegularPolygonSource>::New();
    planeSource->SetCenter(realworld_center_xyz);
    planeSource->SetRadius(radius);
    planeSource->SetNumberOfSides(36);
    planeSource->SetNormal(realworld_normal_xyz);
    planeSource->Update();
    planes[number] = planeSource;


    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(planeSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->GetProperty()->SetColor(colors[number][0],colors[number][1],colors[number][2]);
    actor->GetProperty()->SetOpacity(1);

    actor->SetMapper(mapper);
    main_widget.GetRenderer_3D()->AddActor(actor);
    plane_actors[number] = actor;
  }
  else {
    planes[number]->SetCenter(realworld_center_xyz);
    planes[number]->SetRadius(radius);
    planes[number]->SetNormal(realworld_normal_xyz);
  }

};

void vmxVisualizer::SetPlane(int number, mxImage &image, mxPosition normal, mxPosition center, int radius) 
{
  double normal_xyz[3];
  double center_xyz[3];
  normal_xyz[2] = normal.S();
  normal_xyz[1] = normal.R();
  normal_xyz[0] = normal.C();
  center_xyz[2] = center.S();
  center_xyz[1] = center.R();
  center_xyz[0] = center.C();
  this->SetPlane(number, image, normal_xyz, center_xyz, radius);
}

void vmxVisualizer::SetPlaneWithCenterlineIndex(int number, int centerline_position_index, int radius) {
  if( centerline_position_index >= centerline_centers.GetNumberOfElements() ) {
    centerline_position_index = centerline_centers.GetNumberOfElements() -1;
    return;
  }
  else if ( centerline_position_index <= 0 ) {
    centerline_position_index = 0;
    return;
  }

  centerline_plane_index = centerline_position_index;
  active_centerline_plane = number;
  centerline_plane_radius = radius;
  mxPosition center_pos = centerline_centers[centerline_position_index];
  mxPosition dir_pos = centerline_directions[centerline_position_index];

  int plane_radius;
  if (radius_image == NULL) 
    plane_radius = centerline_plane_radius;
  else
    plane_radius = std::sqrt((double)radius_image->Get(center_pos.S(), center_pos.R(), center_pos.C()))*2.5;
  this->SetPlane(active_centerline_plane, *image_for_centerline, dir_pos, center_pos, plane_radius);

}


void vmxVisualizer::SetPlaneOpacity(int number, double opacity) {
  plane_actors[number]->GetProperty()->SetOpacity(opacity);
  
}

void vmxVisualizer::SetCenterlinePositions( mxImage &image, mxList<mxPosition> &centers, mxList<mxPosition> &dirs, mxImage *radius_image) {
  image_for_centerline = &image;
  centerline_centers = centers;
  centerline_directions = dirs;
  this->radius_image = radius_image;
}

void vmxVisualizer::IncrementCenterlineBasedPlaneLocation(){
  this->SetPlaneWithCenterlineIndex(active_centerline_plane, centerline_plane_index+1, centerline_plane_radius); 
  this->UpdateOnlyPlanes();
}

void vmxVisualizer::DecrementCenterlineBasedPlaneLocation(){
  this->SetPlaneWithCenterlineIndex(active_centerline_plane, centerline_plane_index-1, centerline_plane_radius); 
  this->UpdateOnlyPlanes();
}

void vmxVisualizer::SetSelectedPlaneLocation() {
  selectedPlaneCenter = centerline_centers[centerline_plane_index];
  selectedPlaneDirection = centerline_directions[centerline_plane_index];
  selected_centerline_plane_index = centerline_plane_index;

  std::cout << "Selected Plane at index :" << centerline_plane_index << std::endl;
  std::cout << "C: " << selectedPlaneCenter.S() << " " << selectedPlaneCenter.R() << " " << selectedPlaneCenter.C(); 
  std::cout << "| D: " << selectedPlaneDirection.S() << " " << selectedPlaneDirection.R() << " " << selectedPlaneDirection.C() << std::endl; 
}

int vmxVisualizer::GetSelectedPlaneVectors(mxPosition &center, mxPosition &direction) {
  center = selectedPlaneCenter;
  direction = selectedPlaneDirection;
  return selected_centerline_plane_index;
}

int vmxVisualizer::GetSelectedPlaneCenterlineIndex() {
  return selected_centerline_plane_index;
}


void vmxVisualizer::ToggleActiveDirectionAxis(char axis){
  if (active_direction_axis == axis || 
      active_direction_axis == 'x' || 
      active_direction_axis == 'y' || 
      active_direction_axis == 'z' )
    active_direction_axis = '0';
  else
    active_direction_axis = axis;
  std::cout << "Active axis set to: " << axis << std::endl;
  
}
void vmxVisualizer::IncrementDirectionOnActiveAxis(){
  if(active_direction_axis == 'x' )
    selectedPlaneDirection.C() += 0.1;
  else if(active_direction_axis == 'y' )
    selectedPlaneDirection.R() += 0.1;
  else if(active_direction_axis == 'z' )
    selectedPlaneDirection.S() += 0.1;
  this->SetPlane(active_centerline_plane, *image_for_centerline, selectedPlaneDirection, selectedPlaneCenter, centerline_plane_radius);
  this->UpdateOnlyPlanes();
}
void vmxVisualizer::DecrementDirectionOnActiveAxis(){
  if(active_direction_axis == 'x' )
    selectedPlaneDirection.C() -= 0.1;
  else if(active_direction_axis == 'y' )
    selectedPlaneDirection.R() -= 0.1;
  else if(active_direction_axis == 'z' )
    selectedPlaneDirection.S() -= 0.1;
  this->SetPlane(active_centerline_plane, *image_for_centerline, selectedPlaneDirection, selectedPlaneCenter, centerline_plane_radius);
  this->UpdateOnlyPlanes();
}




void vmxVisualizer::UpdateNoninteractive(){ 

  for( int i=0; i<10; i++) {
    if(active_meshes[i])
      meshes[i]->Update();
    if (active_planes[i]) {
      planes[i]->Update();
    }
  }

  main_widget.GetRenderer_3D()->ResetCamera();
  main_widget.GetRenderWindow()->Render();
};

void vmxVisualizer::UpdateOnlyPlanes(){ 

  for( int i=0; i<10; i++) {
    if (active_planes[i]) {
      planes[i]->Update();
    }
  }

  //main_widget.GetRenderer_3D()->ResetCamera();
  main_widget.GetRenderWindow()->Render();
};


void vmxVisualizer::KillMesh(int number) {
    if(this->getEnvBool("mipxDISABLE_VISUALIZE"))
      return;
  images[number]->Reset();
  meshes[number]->Reset();
  active_meshes[number] = 0;
};

void vmxVisualizer::Update(){ 
    if(this->getEnvBool("mipxDISABLE_VISUALIZE"))
      return;
  for( int i=0; i<10; i++) {
    if(active_meshes[i])
      meshes[i]->Update();
  }
  for( int i=0; i<10; i++) {
    if (active_planes[i]) {
      planes[i]->Update();
    }
  }
  for( int i=0; i<100; i++) {
    if (active_lines[i]) {
      lines[i]->Update();
    }
  }
  //if( plane)
    //plane->Update();
    //
  // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
  main_widget.StartInteraction();

};

void vmxVisualizer::setTitle(const char * title) {
  mxString the_title;
  the_title.Assign("vmxVisualizer - ");
  the_title.Append(title);
  main_widget.GetRenderWindow()->SetWindowName(the_title.Get_C_String());

}

void vmxVisualizer::setSizeAndPosition(int sizex, int sizey, int posx, int posy) {
  main_widget.GetRenderWindow()->SetSize(sizex, sizey);
  main_widget.GetRenderWindow()->SetPosition(posx, posy);
}

void vmxVisualizer::setLightBg() {
    main_widget.GetRenderer_3D()->SetBackground(1,1,1);
    main_widget.GetRenderer_3D()->SetBackground2(1,1,1);
    //Change list widget color WIP
    //mxList<vmxGUIObject*> objects = main_widget.m_objects;
    //for (int i = 0; i < objects.GetNumberOfElements(); ++i) {
      //std::cout << objects[i]->GetClassName().Get_C_String() << std::endl;
      
    //}

}

void vmxVisualizer::SaveScreenshot() {
  if(screenshot_save_location.GetNumberOfCharacters() == 0) {
    logg("Screenshot save location is not set!!!!");
    logg("Screenshot is not saved!!!!");
    return;
  }
  srand(time(NULL));

  saved_screenshots++;
  wimg_filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
  png = vtkSmartPointer<vtkPNGWriter>::New();

  wimg_filter->SetInput(main_widget.GetRenderWindow());
  wimg_filter->SetMagnification(2);
  wimg_filter->SetInputBufferTypeToRGBA();
  wimg_filter->Update();

  const char *basename;
  if( using_irlname) 
    basename = this->getEnv("mipxIRL_NAME");
  else
    basename= this->getEnv("mipxNAME");
  mxString ssname;
  ssname.Assign(screenshot_save_location);
  if( ssname[ssname.GetNumberOfCharacters()-1] != '/' )
    ssname.Append("/");
  ssname.Append(basename);
  ssname.Append("_");
  ssname.AppendNumber(saved_screenshots);
  ssname.Append("_");
  ssname.AppendNumber(rand());
  ssname.Append(".png");

  logg(ssname.Get_C_String());

  main_widget.GetRenderWindow()->GetInteractor()->Render();

  png->SetInputConnection(wimg_filter->GetOutputPort());
  png->SetFileName(ssname.Get_C_String());
  png->Update();
  png->Write();
}
 

void vmxVisualizer::initalizeScreenshotSaveLocation(const char * save_location, int use_irlname) {
  screenshot_save_location.Assign(save_location);
  if( use_irlname)
    using_irlname = 1;
  else
    using_irlname = 0;
}

