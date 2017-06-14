/*=========================================================================
 
 Program:   mipx
 Module:    test_vmxGUI.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include <vtkSmartPointer.h>
#include <vtkAxes.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
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
#include <vtkCamera.h>


#include <iostream>
#include <sstream>
#include <chrono>

#include "mxArray.h"
#include "mxList.h"
#include "mxRegularGrid.h"
#include "mxGeometry.h"
#include "mxImage.h"

//#include "vmxImage.h"

#include "vmxMenu.h"
#include "vmxMenuBar.h"
#include "vmxFilesDialog.h"
#include "vmxListWidget.h"
#include "vmxTextInput.h"
//#include "vmxInteractorStyleImage.h"
#include "vmxInteractorStyleTrackballCamera.h"
#include "vmxButtonGroup.h"




class DoubleClickCallback : public vtkCommand
{
    
public:
    
    /// Constructor.
    DoubleClickCallback(){};
    
    /// Destructor.
    ~DoubleClickCallback(){};
    
    /// Create new object instance.
    static DoubleClickCallback* New() { return new DoubleClickCallback; };
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *)
    {
        cout<<" double click deteted! ";
    };
};



class TryMenuSlot : public vmxMenuSlot
{
public:
    
    virtual ~TryMenuSlot(){};
    
    int Execute(vmxMenu *menu) { cout<<((void*)menu)<<"->Execute() "; return 1; };
};





int main()
{
    //===== THIS IS GENERAL VTK PART - NEEDED FOR ALL EXAMPLES =====
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);
    //vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    vtkSmartPointer<vmxInteractorStyleTrackballCamera> style = vtkSmartPointer<vmxInteractorStyleTrackballCamera>::New();
    //  vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    //    vtkSmartPointer<vmxInteractorStyleImage> style = vtkSmartPointer<vmxInteractorStyleImage>::New();
    
    iren->SetInteractorStyle(style);
    renWin->SetSize(1920, 1080);
    //iren->Initialize();//Initialize the interactor here in order for the SplineWighet to work properly
    //==============================================================
    
    
    
    vtkSmartPointer<vtkRenderer> renderer_ul = vtkSmartPointer<vtkRenderer>::New();
    //vtkSmartPointer<vmxRenderer> renderer_ul = vtkSmartPointer<vmxRenderer>::New();
    renWin->AddRenderer(renderer_ul);
    renderer_ul->SetViewport(0,0,0.5,1);
    
    vtkSmartPointer<vtkRenderer> renderer_ur = vtkSmartPointer<vtkRenderer>::New();
    renWin->AddRenderer(renderer_ur);
    renderer_ur->SetViewport(0.5,0,1,1);
    
    
    //----- register a renderer as 2D -----
    style->m_registered_renderer_2D = renderer_ur;
    //    style->m_registered_renderer_3D = renderer_ul;
    //-----
    
    
    //vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
    //camera->SetPosition(20, 20, 20);
    //camera->SetFocalPoint(0, 0, 0);
    vtkCamera *cam = renderer_ur->GetActiveCamera();
    //cam->SetPosition(0, 0, 200);
    //cam->SetViewAngle(90);
    //cam->SetFocalPoint(0, 0, 0);
    //cam->SetViewUp (0, 1, 0);
    //    cam->SetPosition (0, -1, 0);
    //    cam->ComputeViewPlaneNormal();
    //cam->Azimuth(90.0);
    cam->Elevation(90);
    //renderer_ur->SetActiveCamera(camera);
    
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(0.0, 0.0, 0.0);
    sphereSource->SetRadius(5);
    sphereSource->Update();
    
    // Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    //actor->GetProperty()->SetOpacity(0.5);
    renderer_ul->AddActor(actor);//renderer_ul->AddActor(actor);
    
    renderer_ul->ResetCamera();
    renderer_ul->SetBackground(0,0,0);
    
    cout<<"number_of_layers="<<renWin->GetNumberOfLayers() ;
    renWin->SetNumberOfLayers(2);
    
    renderer->SetViewport(0,0,1,1);
    renderer->SetLayer(1);
    renderer_ul->SetLayer(0);

    
    
    
//    mxDataObjectTree tree;
//    vmxImageFactory *image_factory = new vmxImageFactory;
//    tree.AddFactory(image_factory);
//    
    
    
    vmxClipBoard cb;
    
    vmxGUIMainWidget main_widget;
    main_widget.SetRenderWindow(renWin);
    
    
    
    //    vmxTextInput text_input0;
    //    text_input0.SetClipBoard(&cb);
    //    text_input0.SetMainWidget(&main_widget);
    //    text_input0.SetColor(1,1,1);
    //    text_input0.SetFontSize(20);
    //    text_input0.SetInteractor(iren);
    //    text_input0.SetDescriptionText("Upper input: ");
    //    text_input0.SetPlacementToUpperRight();
    //    text_input0.SetStretchingOver_X_Axis(1);
    //    text_input0.SetVisibility(1);
    
    
    
    vmxMenuBar menu_bar;
    menu_bar.SetMainWidget(&main_widget);
    //menu_bar.SetClipBoard(&cb);
    vmxMenuBarItem *file_menu_item = menu_bar.AddItem("File");
    file_menu_item->GetMenu()->AddItem("Load Data");
    file_menu_item->GetMenu()->AddItem("Close");
    file_menu_item->GetMenu()->BuildMenu();
    menu_bar.AddItem("Edit");
    menu_bar.AddItem("Help");
    menu_bar.SetColor(1,0,0);
    menu_bar.SetFontSize(20);
    menu_bar.SetInteractor(iren);
    //menu_bar.ShowInputText();
    menu_bar.SetPlacementToUpperLeft();
    menu_bar.SetVisibility(1);
    
    mxListIterator< vmxMenuBarItem > it;
    for(it.SetToBegin(menu_bar.m_items); it.IsValid(); it.MoveToNext())
    {
        
        double *pos = it.GetElement().m_text_actor->GetPosition();
        cout<<endl<<"pos=("<<pos[0]<<","<<pos[1]<<")";
    }
    
    
    //    //----- Menu -----
    //    vmxMenu menu;
    //    menu.SetMainWidget(&main_widget);
    //    menu.SetClipBoard(&cb);
    //    //menu.SetDataObjectTree(&tree);
    //    //menu.SetOrigin(0,30);
    //    menu.SetColor(1,0,0);
    //    menu.SetFontSize(20);
    //    vmxMenuItem *item = menu.AddItem("Hello world to call slot",3);
    //    item->AttachSlot(new TryMenuSlot);
    //    menu.AddItem("Something new");
    //    vmxMenuItem *item2 =  menu.AddSubMenu("Has submenu");
    //    item2->GetSubMenu()->AddItem("item 1");
    //    item2->GetSubMenu()->AddItem("item 2");
    //    menu.AddItem("Yet another one",1);
    //    menu.AddItem("Hello world",2);
    //    menu.AddItem("Something new");
    //    menu.AddItem("Another thing");
    //    menu.AddItem("Yet another one");
    //    menu.AddItem("Hello world}");
    //    menu.AddItem("Something new>");
    //    menu.AddItem("Another thing>");
    //    menu.AddItem("Yet another one");// Hello world Hello world");
    //    menu.AddItem("Hello world",3);
    //    menu.AddItem("Something new");
    //    menu.AddItem("Another thing");
    //    menu.AddItem("Yet another one",3);
    //    menu.AddItem("Hello world",1);
    //    menu.AddItem("Something new");
    //    menu.AddItem("Another thing",2);
    //    menu.AddItem("Yet another one");
    //    vmxMenuItem *item3 =  menu.AddSubMenu("Has submenu");
    //    item3->GetSubMenu()->AddItem("item 3");
    //    item3->GetSubMenu()->AddItem("item 4");
    //    item3->GetSubMenu()->AddItem("item 5");
    //    item3->GetSubMenu()->AddItem("item 6");
    //    menu.BuildMenu();
    //    menu.SetInteractor(iren);
    //    //menu.SetPlacementToLowerRight();
    //    //menu.SetPlacementToUpperRight();
    //    menu.SetPlacementToUpperLeft();
    //    //menu.SetPlacementToLowerLeft();
    //    //menu.SetPlacementToLowerCenter();
    //    //menu.SetPlacementToUpperCenter();
    //    //menu.SetPlacementToCenterLeft();
    //    //menu.SetPlacementToCenterRight();
    //    //menu.SetPlacementToRelative(100, 50);
    //    //------------
    
    
    
    
    //    //----- File Dialog -----
    //    vmxFilesDialog od;
    //    od.SetClipBoard(&cb);
    //    od.SetOrigin(0,30);
    //    od.SetColor(1,0,0);
    //    od.SetFontSize(20);
    //    od.SetInteractor(iren);
    //    od.SetPlacementToUpperLeft();
    //    od.OpenFiles();//"/Users/danilobabin/-DIP_IMAGES/");
    //    //----------
    
    
    
    
    
    
    
    
    
    vmxTextInput text_input;
    text_input.SetClipBoard(&cb);
    text_input.SetMainWidget(&main_widget);
    //text_input.SetDataObjectTree(&tree);
    //text_input.SetOrigin(0,30);
    text_input.SetColor(1,1,1);
    text_input.SetFontSize(20);
    text_input.SetInteractor(iren);
    text_input.SetInputTypeToIntegerNumber(313);
    text_input.SetDescriptionText("Int Number: ");
    text_input.SetPlacementToLowerLeft();
    //text_input.ShowInputText();
    text_input.SetVisibility(1);
    
    vmxTextInput text_input3;
    text_input3.SetClipBoard(&cb);
    text_input3.SetMainWidget(&main_widget);
    //text_input3.SetDataObjectTree(&tree);
    //text_input3.SetOrigin(0,30);
    text_input3.SetColor(1,1,1);
    text_input3.SetFontSize(20);
    text_input3.SetInteractor(iren);
    text_input3.SetInputTypeToDoubleNumber(313.13);
    text_input3.SetDescriptionText("Right Double Number: ");
    text_input3.SetPlacementToLowerRight();
    text_input3.SetStretchingOver_X_Axis(1);
    text_input3.SetVisibility(1);
    
    
    
    vmxTextInput text_input2;
    text_input2.SetClipBoard(&cb);
    text_input2.SetMainWidget(&main_widget);
    //text_input2.SetDataObjectTree(&tree);
    //text_input2.SetOrigin(0,30);
    text_input2.SetColor(1,1,1);
    text_input2.SetFontSize(20);
    text_input2.SetInteractor(iren);
    text_input2.SetInputTypeToItem();
    text_input2.SetDescriptionText("Item input: ");
    text_input2.SetPlacementToLowerLeft();
    //text_input2.ShowInputText();
    text_input2.SetVisibility(1);
    
    vmxTextInput text_input4;
    text_input4.SetClipBoard(&cb);
    text_input4.SetMainWidget(&main_widget);
    //text_input4.SetDataObjectTree(&tree);
    //text_input4.SetOrigin(0,30);
    text_input4.SetColor(1,1,1);
    text_input4.SetFontSize(20);
    text_input4.SetInteractor(iren);
    text_input4.SetInputTypeToText("some text");
    text_input4.SetDescriptionText("Left text: ");
    text_input4.SetPlacementToLowerLeft();
    //text_input4.ShowInputText();
    text_input4.SetVisibility(1);
    
    vmxTextInput text_input5;
    text_input5.SetClipBoard(&cb);
    text_input5.SetMainWidget(&main_widget);
    //text_input5.SetDataObjectTree(&tree);
    //text_input5.SetOrigin(0,30);
    text_input5.SetColor(1,1,1);
    text_input5.SetFontSize(20);
    text_input5.SetInteractor(iren);
    text_input5.SetDescriptionText("Right text: ");
    text_input5.SetPlacementToLowerRight();
    //text_input5.SetStretchingOver_X_Axis(1);
    text_input5.SetVisibility(1);
    
    
    
    vmxButtonGroup check_boxes;
    check_boxes.SetButtonTypeToCheckBox();
    check_boxes.SetDescriptionText("Check boxes: ");
    check_boxes.AddItem("jedan");
    check_boxes.AddItem("dva",1);
    check_boxes.AddItem("tri");
    check_boxes.SetMainWidget(&main_widget);
    check_boxes.SetColor(1,1,1);
    check_boxes.SetFontSize(20);
    check_boxes.SetInteractor(iren);
    check_boxes.SetPlacementToLowerLeft();
    check_boxes.SetVisibility(1);
    
    
    vmxButtonGroup radio_buttons;
    radio_buttons.SetButtonTypeToRadioButton();
    radio_buttons.SetDescriptionText("Radio buttons: ");
    radio_buttons.AddItem("jedan");
    radio_buttons.AddItem("dva",1);
    radio_buttons.AddItem("tri");
    radio_buttons.SetMainWidget(&main_widget);
    radio_buttons.SetColor(1,1,1);
    radio_buttons.SetFontSize(20);
    radio_buttons.SetInteractor(iren);
    radio_buttons.SetPlacementToLowerLeft();
    radio_buttons.SetVisibility(1);
    
    vmxButtonGroup push_buttons;
    push_buttons.SetButtonTypeToPushButton();
    push_buttons.SetDescriptionText(" ");
    push_buttons.AddItem("OK");
    push_buttons.AddItem("Cancel");
    push_buttons.SetMainWidget(&main_widget);
    push_buttons.SetColor(1,1,1);
    push_buttons.SetFontSize(20);
    push_buttons.SetInteractor(iren);
    push_buttons.SetPlacementToLowerLeft();
    push_buttons.SetVisibility(1);
    
    
    
    
    //----- List Widget -----
    vmxListWidget list_widget;
    list_widget.SetClipBoard(&cb);
    list_widget.SetMainWidget(&main_widget);
    //list_widget.SetDataObjectTree(&tree);
    //list_widget.SetOrigin(0,30);
    list_widget.SetColor(1,1,0);
    list_widget.SetFontSize(16);
    for(int i=0; i<100; i++)
    {
        mxString name;
        name.Append("item_");
        name.AppendNumber(i);
        list_widget.AddItem(name.Get_C_String(),i%4);
    }
    //vmxListWidgetItem *item = list_widget.AddItem("item_100");
    //list_widget.AddItem("Something new",item);
    list_widget.SetInteractor(iren);
    list_widget.BuildListWidget();
    list_widget.SetPlacementToLowerRight();
    //list_widget.SetPlacementToUpperRight();
    //list_widget.SetPlacementToUpperLeft();
    //list_widget.SetPlacementToLowerLeft();
    //list_widget.SetPlacementToLowerCenter();
    //list_widget.SetPlacementToUpperCenter();
    //list_widget.SetPlacementToCenterLeft();
    //list_widget.SetPlacementToCenterRight();
    //list_widget.SetPlacementToRelative(100, 50);
    //list_widget.SetOrigin(1752,41);
    list_widget.SetVisibility(1);
    //----------
    
    
    
    
    
    
    //style->SetDataObjectTree(&tree);
    //style->SetImage(vimg);
    //style->SetMenu(&menu);
    //    iren->SetObjects(&tree);
    
    mxArray<int> a;
    a.SetNumberOfElements(4);
    a.FillInWith(0);
    a[0] = 4;
    a[3] = 2;
    
    mxList<int>  l;
    l.AddToEnd(a[0]);
    l.AddToEnd(a[1]);
    l.AddToEnd(a[2]);
    l.AddToEnd(a[3]);
    mxListFunctions::SortAscending(l);
    
    
    mxRegularGrid<unsigned short> rg;
    rg.SetDimensions(1, 10, 20, 30);
    rg.FillInWith(4);
    
    mxGeometry g;
    g.SetDimensions(100,100,100);
    int sn,rn,cn;
    for(g.ForSphere(10,10,10,0); g.GetSphere(3, sn,rn,cn); )
    {
        cout<<"("<<sn<<","<<rn<<","<<cn<<")";
    }
    cout<<endl<<endl;
    
    for(g.ForCircle(10,10,0); g.GetCircle(4, rn,cn); )
    {
        cout<<"("<<rn<<","<<cn<<")";
    }
    cout<<endl<<endl;
    
    for(g.ForLine(5, 5, 5, 5, 7, 9); g.GetLine(sn,rn,cn); )
    {
        cout<<"("<<sn<<","<<rn<<","<<cn<<")";
    }
    cout<<endl<<endl;
    
    for(g.ForCube(10, 10, 10, 1); g.GetCube(1,sn,rn,cn); )
    {
        cout<<"("<<sn<<","<<rn<<","<<cn<<")";
    }
    cout<<endl<<endl;
    
    mxImage img;
    img.SetDimensions(2,100,100,100);
    img.FillInWith(0);
    img.Set(1,2,3,4,65535);
    cout<<img.Get(1,2,3,4)<<","<<img.Get(1,2,3,5);
    
    cout<<endl<<endl;
    
    

    
//    vmxImage *vimg = static_cast<vmxImage*>(image_factory->Create());
//    
//    vimg->SetInteractor(iren);
//    vimg->GetImagePlanes()->GetPlaneWidgetForSlices()->SetCurrentRenderer(renderer_ur);
//    //vimg->SetMappingToOpaqueColor();
//    vimg->SetMappingToOpaqueGrayScale();
//    vimg->LoadVTKFile("/Users/danilobabin/-DIP_IMAGES/-HEART/LAA/2016_12_23/dicom_set_grayscale.vtk");
//    vimg->SetVisibility(1);
//    vimg->GetImagePlanes()->GetPlaneWidgetForSlices()->SetPlaneOrientationToYAxes();
    
    

    int size = sizeof(uint32_t);
    cout<<size;
    
    
    
    
    
    vtkSmartPointer<DoubleClickCallback> double_click_call_back = vtkSmartPointer<DoubleClickCallback>::New();
    ////style->AddObserver(style->EventDoubleClick, double_click_call_back);
    style->AddObserver(vmxInteractorStyleTrackballCamera::EventDoubleClick, double_click_call_back);
    
    
    
    
    
    
    //===== START THE RENDERING - NEEDED FOR ALL EXAMPLES =====
    renWin->Render();
    //iren->Initialize();
    iren->Start();
    //=====
    
    return 1;
}





//#include <vtkCamera.h>
//#include <vtkJPEGReader.h>
//#include <vtkImageData.h>
//#include <vtkImageMapper.h> // Note: this is a 2D mapper (cf. vtkImageActor which is 3D)
//#include <vtkActor2D.h>
//#include <vtkProperty2D.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkSmartPointer.h>
//
//int main(int argc, char **argv)
//{
//    vtkSmartPointer<vtkJPEGReader> reader =
//    vtkSmartPointer<vtkJPEGReader>::New();
//    reader->SetFileName("/Users/danilobabin/-DIP_IMAGES/Alis Michalis Vencanje 2015_10/DSC_7001.jpg");
//    reader->Update(); // why is this necessary? shouldn't the VTK pipeline take care of this automatically?
//
//    vtkSmartPointer<vtkImageMapper> mapper =
//    vtkSmartPointer<vtkImageMapper>::New();
//    mapper->SetInputData(reader->GetOutput());
//    mapper->SetColorWindow(255); // width of the color range to map to
//    mapper->SetColorLevel(127.5); // center of the color range to map to
////    int extent[2];
////    extent[0] = extent[1] = 200;
////    mapper->SetCustomDisplayExtents(extent);
//
//    vtkSmartPointer<vtkActor2D> image =
//    vtkSmartPointer<vtkActor2D>::New();
//    image->SetMapper(mapper);
//
//    vtkSmartPointer<vtkRenderer> renderer =
//    vtkSmartPointer<vtkRenderer>::New();
//    renderer->AddActor(image);
//
//    vtkSmartPointer<vtkRenderWindow> window =
//    vtkSmartPointer<vtkRenderWindow>::New();
//    window->AddRenderer(renderer);
//
//    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
//    vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    interactor->SetRenderWindow(window);
//    
//    // Set window to the image size. Note: why is this so cumbersome?
//    //int imageSize[3];
//    //reader->GetOutput()->GetDimensions(imageSize);
//    //window->SetSize(imageSize[0], imageSize[1]);
//    //reader->GetOutput()->GetDimensions(imageSize);
//    int *window_size = window->GetSize();
//    //reader->GetOutput()->SetDimensions(window_size);
//    //image->SetPosition(0,0);
//    //image->SetPosition2(window_size[0],window_size[1]);
//    image->SetWidth(1.0);//window_size[0]);
//    image->SetHeight(1.0);
//    
//    //renderer->GetActiveCamera()->Zoom(0.05);
//    
//    window->Render();
//    renderer->GetActiveCamera()->Zoom(500);
//    window->Render();
//    // Here we'd normally call window->Render() to run the pipeline assembled above,
//    // But instead we'll start an event loop using an interactor,
//    // which prevents the program from returning as soon as it opens the image
//    interactor->Start();
//    
//    return EXIT_SUCCESS;
//}
//

