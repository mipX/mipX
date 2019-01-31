#include <vtkSphereSource.h>
#include <vtkMath.h>
#include <vtkDoubleArray.h>
#include <vtkDataArray.h>
#include <vtkFieldData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkXYPlotActor.h>

int main(int, char *[])
{
    vtkSmartPointer<vtkXYPlotActor> plot =
    vtkSmartPointer<vtkXYPlotActor>::New();
    plot->ExchangeAxesOff();
    plot->SetLabelFormat( "%g" );
    plot->SetXTitle( "Level" );
    plot->SetYTitle( "Frequency" );
    //plot->SetXValuesToIndex();
    plot->SetXValuesToValue();
    
//    plot->SetDataObjectXComponent(0, 0);
//    plot->SetDataObjectYComponent(0, 1);
//    plot->SetDataObjectYComponent(1, 2);
    //plot->SetDataObjectYComponent(0, 1);
    
    vtkSmartPointer<vtkDataObject> data =
    vtkSmartPointer<vtkDataObject>::New();
    
    vtkSmartPointer<vtkFieldData> field =
    vtkSmartPointer<vtkFieldData>::New();
    
    for (unsigned int i = 0 ; i < 2 ; i++)
    {
        vtkSmartPointer<vtkDoubleArray> array_s =
        vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> array_x =
        vtkSmartPointer<vtkDoubleArray>::New();
//        vtkSmartPointer<vtkFieldData> field =
//        vtkSmartPointer<vtkFieldData>::New();
        //vtkSmartPointer<vtkDataObject> data =
        //vtkSmartPointer<vtkDataObject>::New();
        
        for (int b = 0; b < 30; b++)   /// Assuming an array of 30 elements
        {
            double val = vtkMath::Random(0.0,2.0);
            //double tuple[2];
            //tuple[0] = b-10;
            //tuple[1] = val;
            array_s->InsertValue(b, val);
            //array_s->InsertTuple(b, tuple);
            double x_val = b-10;
            array_x->InsertValue(b, x_val);
        }
        field->AddArray(array_x);
        field->AddArray(array_s);
//        data->SetFieldData(field);
        //plot->AddDataObjectInput(data);
        
        //connect index and X value array.
        //plot->SetPointComponent(0, 0);
        //plot->SetPointComponent(i, 2*i);
//        plot->SetDataObjectXComponent(0, 0);
        plot->SetDataObjectXComponent(i, 2*i);

        // connect index and Y value array.
 //       plot->SetDataObjectYComponent(0, 1);
        plot->SetDataObjectYComponent(i, 2*i+1);
        
        //plot->AddDataObjectInput(data);
    }
    data->SetFieldData(field);
    
    plot->AddDataObjectInput(data);
    
    plot->SetPlotColor(1,1,1,0);
    plot->SetPlotColor(1,0,1,0);
    
    vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(plot);
    
    vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );
    renderWindow->SetSize(500,500);
    
    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow( renderWindow );
    
    // Initialize the event loop and then start it
    interactor->Initialize();
    interactor->Start();
    
    return EXIT_SUCCESS;
}



//#include "vtkRenderer.h"
//#include "vtkRenderWindow.h"
//#include "vtkRenderWindowInteractor.h"
//#include "vtkSmartPointer.h"
//#include "vtkObjectFactory.h"
//#include "vtkContext2D.h"
//#include "vtkContextItem.h"
//#include "vtkContextActor.h"
//#include "vtkContextScene.h"
//#include "vtkPen.h"
//#include "vtkBrush.h"
//#include "vtkTextProperty.h"
//#include "vtkOpenGLContextDevice2D.h"
//#include "vtkStdString.h"
//
//#include "mxString.h"
//#include "mxMatrix.h"
//
//#define VTK_CREATE(type, name) \
//vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
//
//
////----------------------------------------------------------------------------
//class APIDiagram : public vtkContextItem
//{
//public:
//
//    //char *m_input;
//
//    static APIDiagram *New();
//    vtkTypeMacro(APIDiagram, vtkContextItem);
//    // Paint event for the chart, called whenever the chart needs to be drawn
//    virtual bool Paint(vtkContext2D *painter);
//
//    //void SetInput(const char *input) {m_input = input;};
//};
//
//
////----------------------------------------------------------------------------
//int main( int, char *[] )
//{
//    // Set up a 2D chart actor, APIDiagram object andn add them to the renderer
//    VTK_CREATE(vtkContextActor, actor);
//    VTK_CREATE(APIDiagram, diagram);
//    actor->GetScene()->AddItem(diagram);
//    VTK_CREATE(vtkRenderer, renderer);
//    renderer->SetBackground(.2, .3, .4);
//    VTK_CREATE(vtkRenderWindow, renderWindow);
//    renderWindow->SetSize(800, 600);
//    renderWindow->AddRenderer(renderer);
//    renderer->AddActor(actor);
//
//    VTK_CREATE(vtkRenderWindowInteractor, interactor);
//    interactor->SetRenderWindow(renderWindow);
//    //renderWindow->SetMultiSamples(0);
//    renderWindow->Render();
//
//    interactor->Start();
//
//    return 1;
//}
//
//// Make our new derived class to draw a diagram
//vtkStandardNewMacro(APIDiagram);
//
////// This function draws our API diagram
////bool APIDiagram::Paint(vtkContext2D *painter)
////{
////    // Drawing a hard wired diagram 800x600 as a demonstration of the 2D API
////    painter->GetTextProp()->SetVerticalJustificationToCentered();
////    painter->GetTextProp()->SetJustificationToCentered();
////    painter->GetTextProp()->SetColor(0.0, 0.0, 0.0);
////    painter->GetTextProp()->SetFontSize(24);
////    painter->GetPen()->SetColor(0, 0, 0);
////
////    painter->GetBrush()->SetColor(100, 255, 100,25);
////    painter->DrawRect(100, 50, 200, 100);
////    painter->DrawString(200, 100, "OpenGL");
////
////    painter->GetBrush()->SetColor(255, 100, 0,255);
////    painter->DrawRect(300, 50, 200, 100);
////    painter->DrawString(400, 100, "Others?");
////
////    painter->GetBrush()->SetColor(100, 0, 255);
////    painter->DrawRect(500, 50, 200, 100);
////    painter->DrawString(600, 100, "Others?");
////
////    painter->GetBrush()->SetColor(180, 180, 255);
////    painter->DrawRect(100, 150, 600, 100);
////    painter->DrawString(400, 200, "2D API");
////
////    painter->GetBrush()->SetColor(255, 255, 180);
////    painter->DrawRect(100, 250, 600, 200);
////    painter->DrawString(400, 400, "Canvas API");
////
////    painter->GetBrush()->SetColor(180, 255, 180,75);
////    painter->DrawRect(100, 250, 300, 100);
////    painter->DrawString(250, 300, "Point Mark");
////
////    painter->GetBrush()->SetColor(255, 255, 255,255);
////    painter->DrawRect(100, 450, 600, 100);
////    painter->DrawString(400, 500, "Canvas View");
////
////    return true;
////}
//
//
//// This function draws our API diagram
//bool APIDiagram::Paint(vtkContext2D *painter)
//{
//    // Drawing a hard wired diagram 800x600 as a demonstration of the 2D API
//    painter->GetTextProp()->SetVerticalJustificationToCentered();
//    painter->GetTextProp()->SetJustificationToCentered();
//    painter->GetTextProp()->SetColor(0.0, 0.0, 0.0);
//    painter->GetTextProp()->SetFontSize(24);
//    painter->GetPen()->SetColor(0, 0, 0);
//
//
//    const char *input = " {Table}{A}{B}{C} {1}{1a}{1b}{1c} {2}{2a}{2b}{2c}";
//    mxString s;
//    s.Assign(input);
//
//    int size_C = 4;
//    int size_R = 3;
//
//    int window_size[2];
//    window_size[0] = 800;
//    window_size[1] = 600;
//
//    int field_size[2];
//    field_size[0] = window_size[0]/size_C;
//    field_size[1] = window_size[1]/size_R;
//
//
//    mxList< mxString > list;
//
//    s.ExtractStrings('{', '}', list);
//
//    painter->GetBrush()->SetColor(100, 100, 100, 255);
//
//    painter->GetPen()->SetColor(255,255,0,255);
//    painter->GetTextProp()->SetColor(1,1,0);// notice: this method takes doubles (range 0 to 1)!
//
//    mxMatrix< mxString > table;
//    table.SetDimensions(size_R, size_C);
//
//    mxListIterator<mxString> it;
//    it.SetToBegin(list);
//    for(int r=table.GetNumberOfRows()-1; r>=0 && it.IsValid(); r--)
//    {
//        for(int c=0; c<size_C && it.IsValid(); c++, it.MoveToNext())
//        {
//            table(r,c).Assign(it.GetElement());
//        }
//    }
//
//    for(int r=0; r<table.GetNumberOfRows(); r++)
//    {
//        for(int c=0; c<table.GetNumberOfColumns(); c++)
//        {
//            painter->DrawRect(c*field_size[0], r*field_size[1], (c+1)*field_size[0], (r+1)*field_size[1]);
//            painter->DrawString( ((2*c+1)*field_size[0])/2, ((2*r+1)*field_size[1])/2, table(r,c).Get_C_String());
//        }
//    }
//
//    return true;
//}
//




////#include <iostream>
////#include <sstream>
////#include <chrono>
////#include <csignal>
////#include <unistd.h>
////
////
////using namespace std;
////
////void signalHandler( int signum ) {
////    cout << "Interrupt signal (" << signum << ") received.\n";
////    
////    // cleanup and close up stuff here
////    // terminate program
////    
////    exit(signum);
////    
////}
////
////int main () {
////    int i = 0;
////    // register signal SIGINT and signal handler
////    signal(SIGUSR1, signalHandler);
////    
////    while(++i){
////        cout << "Going to sleep...." << i<< endl;
////        //cout<< SIGUSR1<<endl;
////        if( i == 3 ){
////            raise( SIGUSR1 );
////        }
////        
////        usleep(1);
////    }
////    
////    return 0;
////}
//
//
////#include <csignal>
////#include <iostream>
////#include <unistd.h>
////
////int signaled = 0;
////
////void signal_handler(int signal)
////{
////    signaled = 1;
////    std::cout << "Received signal " << signal << '\n';
////}
////
////int main()
////{
////    int sig_value = 4;
////    
////    // Install a signal handler
////    std::signal(sig_value, signal_handler);
////    
////    std::cout << "Sending signal " << sig_value << '\n';
////    
////    //try
////    //{
////        std::raise(sig_value);//raise(SIGINT);
////    //}
////    //catch (int &z)
////    //{
////    //    std::cout << "Caught exception: " << z << std::endl;
////    //}
////    
//////    int ppid;
//////    ppid=getppid();
//////    kill(ppid, SIGUSR1);
//////    
////    //usleep( 5000 );
////    
////    std::cout<<" kraj signaled="<<signaled<<std::endl;
////    //std::raise(SIGTERM);
////}
//
//
/////* signal example */
////#include <stdio.h>      /* printf */
////#include <signal.h>     /* signal, raise, sig_atomic_t */
////
////sig_atomic_t signaled = 0;
////
////void my_handler (int param)
////{
////    signaled = 1;
////}
////
////int main ()
////{
////    void (*prev_handler)(int);
////    
////    prev_handler = signal (2, my_handler);
////    
////    /* ... */
////    raise(2);
////    /* ... */
////    
////    printf ("signaled is %d.\n",signaled);
////    
////    
////    return 0;
////}
//
//
//
////#include <signal.h>
////#include <stdio.h>
////
/////* or you might use a semaphore to notify a waiting thread */
////static volatile sig_atomic_t sig_caught = 0;
////
////void handle_sighup(int signum)
////{
////    /* in case we registered this handler for multiple signals */
////    if (signum == SIGINT) {//if (signum == SIGHUP) {
////        sig_caught = 1;
////    }
////}
////
////int main(int argc, char* argv[])
////{
////    /* you may also prefer sigaction() instead of signal() */
////    signal(SIGINT, handle_sighup);//signal(SIGHUP, handle_sighup);
////    
////    while(1) {
////        if (sig_caught) {
////            
////            sig_caught = 0;
////            printf("caught a SIGHUP.  I should re-read settings.\n");
////        }
////    }
////    
////    return 0;
////}
//
///*=========================================================================
// 
// Program:   mipx
// Module:    test_VTK_over_libs.cpp
// 
// Authors: Danilo Babin, Hrvoje Leventic.
// Copyright (c) Danilo Babin, Hrvoje Leventic.
// All rights reserved.
// See Copyright.txt
//
// Licensed under the BSD License 2.0.
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notice for more information.
//
// =========================================================================*/
//
//
//
//
//#include <vtkSmartPointer.h>
//#include <vtkAxes.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkActor.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRendererCollection.h>
//#include <vtkInteractorStyleTrackballCamera.h>
//#include <vtkInteractorStyleImage.h>
//#include <vtkPropPicker.h>
//#include <vtkPicker.h>
//#include <vtkCellPicker.h>
//#include <vtkImagePlaneWidget.h>
//#include <vtkLookupTable.h>
//#include <vtkSphereSource.h>
//#include <vtkImageData.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkCommand.h>
//#include <vtkProperty.h>
//#include <vtkTextActor.h>
//#include <vtkTextProperty.h>
//#include <vtkCallbackCommand.h>
//#include <vtkCamera.h>
//
//
//#include <iostream>
//#include <sstream>
//#include <chrono>
//
//#include "mxArray.h"
//#include "mxDataObjectTree.h"
//#include "mxList.h"
//#include "mxRegularGrid.h"
//#include "mxGeometry.h"
//#include "mxImage.h"
//#include "mxGIP.h"
//
//#include "vmxImage.h"
//
//#include "vmxGUIMenu.h"
//#include "vmxGUIMenuBar.h"
//#include "vmxGUIFilesDialog.h"
//#include "vmxGUIListWidget.h"
//#include "vmxGUITextInput.h"
//#include "vmxGUIInteractorStyleImage.h"
//#include "vmxGUIInteractorStyleTrackballCamera.h"
//#include "vmxGUIButtonGroup.h"
////#include "vmxGUIRenderer.h"
//
//
//
//
//class DoubleClickCallback : public vtkCommand
//{
//    
//public:
//    
//    /// Constructor.
//    DoubleClickCallback(){};
//    
//    /// Destructor.
//    ~DoubleClickCallback(){};
//    
//    /// Create new object instance.
//    static DoubleClickCallback* New() { return new DoubleClickCallback; };
//    
//    /// Method that executes when the callback is called.
//    virtual void Execute(vtkObject *caller, unsigned long, void *)
//    {
//        cout<<" double click deteted! ";
//    };
//};
//
//
//
//class TryMenuSlot : public vmxGUIMenuSlot
//{
//public:
//    
//    virtual ~TryMenuSlot(){};
//    
//    int Execute(vmxGUIMenu *menu) { cout<<((void*)menu)<<"->Execute() "; return 1; };
//};
//
//
//
//
//
//int main()
//{
//	//===== THIS IS GENERAL VTK PART - NEEDED FOR ALL EXAMPLES =====
//	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
//	renWin->AddRenderer(renderer);
//	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    iren->SetRenderWindow(renWin);
////	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//    vtkSmartPointer<vmxGUIInteractorStyleTrackballCamera> style = vtkSmartPointer<vmxGUIInteractorStyleTrackballCamera>::New();
////  vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
////    vtkSmartPointer<vmxGUIInteractorStyleImage> style = vtkSmartPointer<vmxGUIInteractorStyleImage>::New();
//
//	iren->SetInteractorStyle(style);
//	renWin->SetSize(1920, 1080);
//	//iren->Initialize();//Initialize the interactor here in order for the SplineWighet to work properly
//	//==============================================================
//    
//    
////    double origin[2];
////    origin[0] = 0; origin[1] = 0;
////    double font_size = 20;
////    
////    // Setup the text and add it to the renderer
////    vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
////    textActor->SetInput ( "[ ] Hello world\nSomething new\nAnother thing\nYet another one" );
////    textActor->SetPosition ( origin[0], origin[1] );
////    textActor->GetTextProperty()->SetFontSize ( font_size );
////    textActor->GetTextProperty()->SetColor ( 1.0, 0.0, 0.0 );
////    renderer->AddActor2D ( textActor );
////    
////    vtkSmartPointer<vtkTextActor> textActor2 = vtkSmartPointer<vtkTextActor>::New();
////    textActor2->SetInput ( "_T_T_T_T_T_" );
////    textActor2->SetPosition ( origin[0], origin[1] + 4*(font_size+1) );
////    textActor2->GetTextProperty()->SetFontSize ( font_size );
////    textActor2->GetTextProperty()->SetColor ( 1.0, 0.0, 0.0 );
////    renderer->AddActor2D ( textActor2 );
//
//    
//    vtkSmartPointer<vtkRenderer> renderer_ul = vtkSmartPointer<vtkRenderer>::New();
//    //vtkSmartPointer<vmxRenderer> renderer_ul = vtkSmartPointer<vmxRenderer>::New();
//    //vmxRenderer renderer_ul_obj;
//    //vmxRenderer *renderer_ul = &renderer_ul_obj;
//    renWin->AddRenderer(renderer_ul);
//    renderer_ul->SetViewport(0,0,0.5,1);
//    
//    vtkSmartPointer<vtkRenderer> renderer_ur = vtkSmartPointer<vtkRenderer>::New();
//    renWin->AddRenderer(renderer_ur);
//    renderer_ur->SetViewport(0.5,0,1,1);
//    
//
//    
//    style->m_registered_renderer_2D = renderer_ur;
////    style->m_registered_renderer_3D = renderer_ul;
//    
//    //vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
//    //camera->SetPosition(20, 20, 20);
//    //camera->SetFocalPoint(0, 0, 0);
//    vtkCamera *cam = renderer_ur->GetActiveCamera();
//    //cam->SetPosition(0, 0, 200);
//    //cam->SetViewAngle(90);
//    //cam->SetFocalPoint(0, 0, 0);
//    //cam->SetViewUp (0, 1, 0);
////    cam->SetPosition (0, -1, 0);
////    cam->ComputeViewPlaneNormal();
//    //cam->Azimuth(90.0);
//    cam->Elevation(90);
//    //renderer_ur->SetActiveCamera(camera);
//    
////    //----- Sphere -----
////    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
////    sphereSource->SetCenter(0.0, 0.0, 0.0);
////    sphereSource->SetRadius(5);
////    sphereSource->Update();
////    
//////    vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
//////    mapper2->SetInputConnection(sphereSource->GetOutputPort());
//////    vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
//////    actor2->SetMapper(mapper2);
//////    //actor->GetProperty()->SetOpacity(0.5);
//////    renderer_ur->AddActor(actor2);
////    
////    // Create a mapper and actor
////    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
////    mapper->SetInputConnection(sphereSource->GetOutputPort());
////    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
////    actor->SetMapper(mapper);
////    //actor->GetProperty()->SetOpacity(0.5);
////    renderer_ul->AddActor(actor);//renderer_ul->AddActor(actor);
////    //----------
//
//    renderer_ul->ResetCamera();
//    renderer_ul->SetBackground(0,0,0);
//    
//    cout<<"number_of_layers="<<renWin->GetNumberOfLayers() ;
//    renWin->SetNumberOfLayers(2);
//    
//    renderer->SetViewport(0,0,1,1);
////    renderer->GetViewport();
//    renderer->SetLayer(1);
//    renderer_ul->SetLayer(0);
//    
//    //renderer_ul->
//
////    vtkSmartPointer<vtkRenderWindowInteractor> iren2 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
////    vtkSmartPointer<vtkInteractorStyleImage> style2 = vtkSmartPointer<vtkInteractorStyleImage>::New();
////    iren2->SetRenderWindow(renWin);
////    iren2->SetInteractorStyle(style2);
////    style2->SetCurrentRenderer(renderer_ur);
//
////    // Define viewport ranges
////    double xmins[4] = {0.4,   0.5, 0.4,   0.5};
////    double xmaxs[4] = {0.5,   1, 0.5,   1};
////    double ymins[4] = {0.4,     0.4, 0.5, 0.5};
////    double ymaxs[4] = {0.5, 0.5,   1,   1};
////    for(unsigned i = 0; i < 4; i++)
////    {
////        vtkSmartPointer<vtkRenderer> renderer =
////        vtkSmartPointer<vtkRenderer>::New();
////        
////        renWin->AddRenderer(renderer);
////        renderer->SetViewport(xmins[i],ymins[i],xmaxs[i],ymaxs[i]);
////        
////        // Create a sphere
////        vtkSmartPointer<vtkSphereSource> sphereSource =
////        vtkSmartPointer<vtkSphereSource>::New();
////        sphereSource->SetCenter(0.0, 0.0, 0.0);
////        sphereSource->SetRadius(5);
////        sphereSource->Update();
////        
////        // Create a mapper and actor
////        vtkSmartPointer<vtkPolyDataMapper> mapper =
////        vtkSmartPointer<vtkPolyDataMapper>::New();
////        mapper->SetInputConnection(sphereSource->GetOutputPort());
////        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
////        actor->SetMapper(mapper);
////        renderer->AddActor(actor);
////        renderer->ResetCamera();
////        renderer->SetBackground(255,255,255);
////        renderer->SetLayer(1);
////        
////        renWin->Render();
////        renWin->SetWindowName("Multiple ViewPorts");
////    }
//    
//    
//    
//    mxDataObjectTree tree;
//    vmxImageFactory *image_factory = new vmxImageFactory;
//    tree.AddFactory(image_factory);
//    
//   
//    
//    vmxGUIClipBoard cb;
//    
//    vmxGUIMainWidget main_widget;
//    main_widget.SetRenderWindow(renWin);
//   
//    
//    
////    vmxTextInput text_input0;
////    text_input0.SetClipBoard(&cb);
////    text_input0.SetMainWidget(&main_widget);
////    text_input0.SetColor(1,1,1);
////    text_input0.SetFontSize(20);
////    text_input0.SetInteractor(iren);
////    text_input0.SetDescriptionText("Upper input: ");
////    text_input0.SetPlacementToUpperRight();
////    text_input0.SetStretchingOver_X_Axis(1);
////    text_input0.SetVisibility(1);
//    
//    
//    
//    vmxGUIMenuBar menu_bar;
//    menu_bar.SetMainWidget(&main_widget);
//    //menu_bar.SetClipBoard(&cb);
//    vmxGUIMenuBarItem *file_menu_item = menu_bar.AddItem("File");
//    file_menu_item->GetMenu()->AddItem("Load Data");
//    file_menu_item->GetMenu()->AddItem("Close");
//    file_menu_item->GetMenu()->BuildMenu();
//    menu_bar.AddItem("Edit");
//    menu_bar.AddItem("Help");
//    menu_bar.SetColor(1,0,0);
//    menu_bar.SetFontSize(20);
//    menu_bar.SetInteractor(iren);
//    //menu_bar.ShowInputText();
//    menu_bar.SetPlacementToUpperLeft();
//    menu_bar.SetVisibility(1);
//
//    mxListIterator< vmxGUIMenuBarItem > it;
//    for(it.SetToBegin(menu_bar.m_items); it.IsValid(); it.MoveToNext())
//    {
//
//        double *pos = it.GetElement().m_text_actor->GetPosition();
//        cout<<endl<<"pos=("<<pos[0]<<","<<pos[1]<<")";
//    }
//    
//    
////    //----- Menu -----
////    vmxMenu menu;
////    menu.SetMainWidget(&main_widget);
////    menu.SetClipBoard(&cb);
////    //menu.SetDataObjectTree(&tree);
////    //menu.SetOrigin(0,30);
////    menu.SetColor(1,0,0);
////    menu.SetFontSize(20);
////    vmxMenuItem *item = menu.AddItem("Hello world to call slot",3);
////    item->AttachSlot(new TryMenuSlot);
////    menu.AddItem("Something new");
////    vmxMenuItem *item2 =  menu.AddSubMenu("Has submenu");
////    item2->GetSubMenu()->AddItem("item 1");
////    item2->GetSubMenu()->AddItem("item 2");
////    menu.AddItem("Yet another one",1);
////    menu.AddItem("Hello world",2);
////    menu.AddItem("Something new");
////    menu.AddItem("Another thing");
////    menu.AddItem("Yet another one");
////    menu.AddItem("Hello world}");
////    menu.AddItem("Something new>");
////    menu.AddItem("Another thing>");
////    menu.AddItem("Yet another one");// Hello world Hello world");
////    menu.AddItem("Hello world",3);
////    menu.AddItem("Something new");
////    menu.AddItem("Another thing");
////    menu.AddItem("Yet another one",3);
////    menu.AddItem("Hello world",1);
////    menu.AddItem("Something new");
////    menu.AddItem("Another thing",2);
////    menu.AddItem("Yet another one");
////    vmxMenuItem *item3 =  menu.AddSubMenu("Has submenu");
////    item3->GetSubMenu()->AddItem("item 3");
////    item3->GetSubMenu()->AddItem("item 4");
////    item3->GetSubMenu()->AddItem("item 5");
////    item3->GetSubMenu()->AddItem("item 6");
////    menu.BuildMenu();
////    menu.SetInteractor(iren);
////    //menu.SetPlacementToLowerRight();
////    //menu.SetPlacementToUpperRight();
////    menu.SetPlacementToUpperLeft();
////    //menu.SetPlacementToLowerLeft();
////    //menu.SetPlacementToLowerCenter();
////    //menu.SetPlacementToUpperCenter();
////    //menu.SetPlacementToCenterLeft();
////    //menu.SetPlacementToCenterRight();
////    //menu.SetPlacementToRelative(100, 50);
////    //------------
//    
//    
//    
//    
////    //----- File Dialog -----
////    vmxFilesDialog od;
////    od.SetClipBoard(&cb);
////    od.SetOrigin(0,30);
////    od.SetColor(1,0,0);
////    od.SetFontSize(20);
////    od.SetInteractor(iren);
////    od.SetPlacementToUpperLeft();
////    od.OpenFiles();//"/Users/danilobabin/-DIP_IMAGES/");
////    //----------
//    
//
//    
//    
//    
//    
//    
//    
//    
//    vmxGUITextInput text_input;
//    text_input.SetClipBoard(&cb);
//    text_input.SetMainWidget(&main_widget);
//    //text_input.SetDataObjectTree(&tree);
//    //text_input.SetOrigin(0,30);
//    text_input.SetColor(1,1,1);
//    text_input.SetFontSize(20);
//    text_input.SetInteractor(iren);
//    text_input.SetInputTypeToIntegerNumber(313);
//    text_input.SetDescriptionText("Int Number: ");
//    text_input.SetPlacementToLowerLeft();
//    //text_input.ShowInputText();
//    text_input.SetVisibility(1);
//    
//    vmxGUITextInput text_input3;
//    text_input3.SetClipBoard(&cb);
//    text_input3.SetMainWidget(&main_widget);
//    //text_input3.SetDataObjectTree(&tree);
//    //text_input3.SetOrigin(0,30);
//    text_input3.SetColor(1,1,1);
//    text_input3.SetFontSize(20);
//    text_input3.SetInteractor(iren);
//    text_input3.SetInputTypeToDoubleNumber(313.13);
//    text_input3.SetDescriptionText("Right Double Number: ");
//    text_input3.SetPlacementToLowerRight();
//    text_input3.SetStretchingOver_X_Axis(1);
//    text_input3.SetVisibility(1);
//
//
//    
//    vmxGUITextInput text_input2;
//    text_input2.SetClipBoard(&cb);
//    text_input2.SetMainWidget(&main_widget);
//    //text_input2.SetDataObjectTree(&tree);
//    //text_input2.SetOrigin(0,30);
//    text_input2.SetColor(1,1,1);
//    text_input2.SetFontSize(20);
//    text_input2.SetInteractor(iren);
//    text_input2.SetInputTypeToItem();
//    text_input2.SetDescriptionText("Item input: ");
//    text_input2.SetPlacementToLowerLeft();
//    //text_input2.ShowInputText();
//    text_input2.SetVisibility(1);
//    
//    vmxGUITextInput text_input4;
//    text_input4.SetClipBoard(&cb);
//    text_input4.SetMainWidget(&main_widget);
//    //text_input4.SetDataObjectTree(&tree);
//    //text_input4.SetOrigin(0,30);
//    text_input4.SetColor(1,1,1);
//    text_input4.SetFontSize(20);
//    text_input4.SetInteractor(iren);
//    text_input4.SetInputTypeToText("some text");
//    text_input4.SetDescriptionText("Left text: ");
//    text_input4.SetPlacementToLowerLeft();
//    //text_input4.ShowInputText();
//    text_input4.SetVisibility(1);
//
//    vmxGUITextInput text_input5;
//    text_input5.SetClipBoard(&cb);
//    text_input5.SetMainWidget(&main_widget);
//    //text_input5.SetDataObjectTree(&tree);
//    //text_input5.SetOrigin(0,30);
//    text_input5.SetColor(1,1,1);
//    text_input5.SetFontSize(20);
//    text_input5.SetInteractor(iren);
//    text_input5.SetDescriptionText("Right text: ");
//    text_input5.SetPlacementToLowerRight();
//    //text_input5.SetStretchingOver_X_Axis(1);
//    text_input5.SetVisibility(1);
//    
//    
//    
//    vmxButtonGroup check_boxes;
//    check_boxes.SetButtonTypeToCheckBox();
//    check_boxes.SetDescriptionText("Check boxes: ");
//    check_boxes.AddItem("jedan");
//    check_boxes.AddItem("dva",1);
//    check_boxes.AddItem("tri");
//    check_boxes.SetMainWidget(&main_widget);
//    check_boxes.SetColor(1,1,1);
//    check_boxes.SetFontSize(20);
//    check_boxes.SetInteractor(iren);
//    check_boxes.SetPlacementToLowerLeft();
//    check_boxes.SetVisibility(1);
//    
//    
//    vmxButtonGroup radio_buttons;
//    radio_buttons.SetButtonTypeToRadioButton();
//    radio_buttons.SetDescriptionText("Radio buttons: ");
//    radio_buttons.AddItem("jedan");
//    radio_buttons.AddItem("dva",1);
//    radio_buttons.AddItem("tri");
//    radio_buttons.SetMainWidget(&main_widget);
//    radio_buttons.SetColor(1,1,1);
//    radio_buttons.SetFontSize(20);
//    radio_buttons.SetInteractor(iren);
//    radio_buttons.SetPlacementToLowerLeft();
//    radio_buttons.SetVisibility(1);
//   
//    vmxButtonGroup push_buttons;
//    push_buttons.SetButtonTypeToPushButton();
//    push_buttons.SetDescriptionText(" ");
//    push_buttons.AddItem("OK");
//    push_buttons.AddItem("Cancel");
//    push_buttons.SetMainWidget(&main_widget);
//    push_buttons.SetColor(1,1,1);
//    push_buttons.SetFontSize(20);
//    push_buttons.SetInteractor(iren);
//    push_buttons.SetPlacementToLowerLeft();
//    push_buttons.SetVisibility(1);
//    
//    
//    
//    
//    //----- List Widget -----
//    vmxGUIListWidget list_widget;
//    list_widget.SetClipBoard(&cb);
//    list_widget.SetMainWidget(&main_widget);
//    //list_widget.SetDataObjectTree(&tree);
//    //list_widget.SetOrigin(0,30);
//    list_widget.SetColor(1,1,0);
//    list_widget.SetFontSize(16);
//    for(int i=0; i<100; i++)
//    {
//        mxString name;
//        name.Append("item_");
//        name.AppendNumber(i);
//        list_widget.AddItem(name.Get_C_String(),i%4);
//    }
//    //vmxListWidgetItem *item = list_widget.AddItem("item_100");
//    //list_widget.AddItem("Something new",item);
//    list_widget.SetInteractor(iren);
//    list_widget.BuildListWidget();
//    list_widget.SetPlacementToLowerRight();
//    //list_widget.SetPlacementToUpperRight();
//    //list_widget.SetPlacementToUpperLeft();
//    //list_widget.SetPlacementToLowerLeft();
//    //list_widget.SetPlacementToLowerCenter();
//    //list_widget.SetPlacementToUpperCenter();
//    //list_widget.SetPlacementToCenterLeft();
//    //list_widget.SetPlacementToCenterRight();
//    //list_widget.SetPlacementToRelative(100, 50);
//    //list_widget.SetOrigin(1752,41);
//    list_widget.SetVisibility(1);
//    //----------
//
//
//    
//
//    
//    
//    //style->SetDataObjectTree(&tree);
//    //style->SetImage(vimg);
//    //style->SetMenu(&menu);
////    iren->SetObjects(&tree);
//    
//    mxArray<int> a;
//    a.SetNumberOfElements(4);
//    a.FillInWith(0);
//    a[0] = 4;
//    a[3] = 2;
//    
//    mxList<int>  l;
//    l.AddToEnd(a[0]);
//    l.AddToEnd(a[1]);
//    l.AddToEnd(a[2]);
//    l.AddToEnd(a[3]);
//    mxListFunctions::SortAscending(l);
//    
//    
//    mxRegularGrid<unsigned short> rg;
//    rg.SetDimensions(1, 10, 20, 30);
//    rg.FillInWith(4);
//    
//    mxGeometry g;
//    g.SetDimensions(100,100,100);
//    int sn,rn,cn;
//    for(g.ForSphere(10,10,10,0); g.GetSphere(3, sn,rn,cn); )
//    {
//        cout<<"("<<sn<<","<<rn<<","<<cn<<")";
//    }
//    cout<<endl<<endl;
//    
//    for(g.ForCircle(10,10,0); g.GetCircle(4, rn,cn); )
//    {
//        cout<<"("<<rn<<","<<cn<<")";
//    }
//    cout<<endl<<endl;
//    
//    for(g.ForLine(5, 5, 5, 5, 7, 9); g.GetLine(sn,rn,cn); )
//    {
//        cout<<"("<<sn<<","<<rn<<","<<cn<<")";
//    }
//    cout<<endl<<endl;
//
//    for(g.ForCube(10, 10, 10, 1); g.GetCube(1,sn,rn,cn); )
//    {
//        cout<<"("<<sn<<","<<rn<<","<<cn<<")";
//    }
//    cout<<endl<<endl;
//
//    mxImage img;
//    img.SetDimensions(2,100,100,100);
//    img.FillInWith(0);
//    img.Set(1,2,3,4,65535);
//    cout<<img.Get(1,2,3,4)<<","<<img.Get(1,2,3,5);
//    
//    cout<<endl<<endl;
//    
//    
////    //{
////    
//////        vmxImage vi;
//////        vi.SetInteractor(iren);
//////        vi.SetDimensions(2,100,100,100);
//////        vi.SetVisibility(1);
//////        vi.FillInWith(0);
//////        for(g.ForSphere(10,10,10,0); g.GetSphere(16, sn,rn,cn); )
//////        {
//////            //cout<<"("<<sn<<","<<rn<<","<<cn<<")";
//////            vi(0,sn,rn,cn) = 255;
//////        }
//////        for(g.ForLine(5, 5, 5, 6, 7, 9); g.GetLine(sn,rn,cn); )
//////        {
//////            vi(1,sn,rn,cn) = 255;
//////        }
////    
////        //vi.SetMappingToTransparentBinary();
////
//    
//    vmxImage *vimg = static_cast<vmxImage*> (tree.Create("vmxImage16U","image"));
//
//    //vmxImage *vimg = static_cast<vmxImage*>(image_factory->Create());
//    
//    vimg->SetInteractor(iren);
//    vimg->GetImagePlanes()->GetPlaneWidget_S()->SetCurrentRenderer(renderer_ul);
//    vimg->SetMappingToOpaqueGrayScale(); //vimg->SetMappingToOpaqueColor();
//    vimg->LoadVTKFile("/Users/danilobabin/-DIP_IMAGES/-HEART/LAA/2016_12_23/dicom_set_grayscale.vtk");
//    //vimg->SetVisibility(1);
//    //vimg->GetImagePlanes()->GetPlaneWidgetForSlices()->SetPlaneOrientationToYAxes();
//    
//    
////    mxImageScalar minimum_value, maximum_value;
////    vimg->GetVoxelMinAndMaxValues(minimum_value, maximum_value);
////    int value_range = maximum_value - minimum_value;
////    
////    for(unsigned int i=0; i<vimg->GetNumberOfDataElements(); i++)
////    {
////        double coeff = 1.0 - ((double)(vimg->Get(i)-minimum_value)) / ((double)(value_range));
////        vimg->Set(i, ((mxImageScalar)(coeff * ((double)maximum_value))) );
////    }
//
//   
//    
//    vmxImage *vimg2 = static_cast<vmxImage*>(image_factory->Create());
//    
//    mxGIP ipg;
//    //ipg.InvertValues(*vimg, *vimg2);
//    ipg.ThresholdInRange(*vimg, 100,150, *vimg2);
//    
//    vimg2->SetInteractor(iren);
//    vimg2->GetImagePlanes()->GetPlaneWidget_S()->SetCurrentRenderer(renderer_ul);
//    vimg2->SetMappingToOpaqueGrayScale();
//    //vimg2->GetImagePlanes()->GetPlaneWidgetForSlices()->SetPlaneOrientationToYAxes();
//    vimg2->SetVisibility(1);
//
//    
//    //vimg->SetDimensions(2,100,100,100);
//    //vimg->FillInWith(0);
////    for(g.ForSphere(10,10,10,0); g.GetSphere(16, sn,rn,cn); )
////    {
////        //cout<<"("<<sn<<","<<rn<<","<<cn<<")";
////        vimg->operator()(0,sn,rn,cn) = 255;
////    }
////    for(g.ForLine(5, 5, 5, 6, 7, 9); g.GetLine(sn,rn,cn); )
////    {
////        vimg->operator()(1,sn,rn,cn) = 255;
////    }
////        //vimg->LoadVTKFile("/Users/danilobabin/-DIP_IMAGES/-HEART/LAA/2016_12_23/dicom_set_grayscale.vtk");
//////
////    vi.SetInteractor(iren);
////    vi.LoadVTKFile("/Users/danilobabin/-DIP_IMAGES/-AORTA/Wavelocity/InterScanner_UZGent/Volunteer01_F43Y/v01_UZG_sag_PC.vtk");
////
////    vi.SetMappingToOpaqueColor();
////    //vi.SetMappingToTransparentColor();
////
////    vi.SetVisibility(1);
//////    //}
//////
//////    vimg->SetVisibility(1);
//    
//    
////    vmxImageData vid;
////    vid.SetDimensions(2,100,100,100);
////    vid.FillInWith(0);
////    vid.Set(1,2,3,4,65535);
////    cout<<vid.Get(1,2,3,4)<<","<<vid.Get(1,2,3,5);
////    
////    vmxImagePlaneWidget pw;
////    pw.SetImage(&vid);
////    pw.SetInteractor(iren);
////    //pw.SetMappingToOpaqueColor();
////    pw.SetVisibilityOfSlicePlane(1);
//    
//    int size = sizeof(uint32_t);
//    cout<<size;
//    
//    
////    
////
////
////	vbdBasicImage img;
////	img.Load_VTK_FileStructuredPoints("/Users/danilobabin/-DIP_IMAGES/-AORTA/Aorta inplane PhC for Danilo/L_Juna_PhC_min_difference.vtk");
////
//////    // Create a black/white lookup table.
//////    vtkSmartPointer<vtkLookupTable> bwLut = vtkSmartPointer<vtkLookupTable>::New();
//////    bwLut->SetTableRange( 0, 255);
//////    bwLut->SetSaturationRange( 0, 0);
//////    bwLut->SetHueRange( 0, 0);
//////    bwLut->SetValueRange( 0, 1);
////    
////    // Create transversal image plane
////    vtkSmartPointer<vtkImagePlaneWidget> transversal_plane = vtkSmartPointer<vtkImagePlaneWidget>::New();
////    transversal_plane->SetInputData(img.m_vtk_visualization_image_data);
////    transversal_plane->DisplayTextOn();
////    transversal_plane->SetPlaneOrientationToZAxes();
////    transversal_plane->SetSliceIndex(0);
////    //transversal_plane->SetLookupTable(bwLut);
////    transversal_plane->SetInteractor(iren);
////    transversal_plane->EnabledOn();
////
////    
////    
////	//vbdImagePlanes pl;
////	//pl.SetInputImage(&img);
////	//pl.SetInteractor(iren);
////	//pl.SetMappingToColorTransparent();
////	//pl.SetVisibilityOfTransversalPlane(1);
////	//pl.SetVisibilityOfSagittalPlane(1);
////	//pl.SetVisibilityOfCoronalPlane(1);
////
//
//    
//////    // Initialize a keypress callback function
////    vtkSmartPointer<KeyPressCallBack> key_press_call_back = vtkSmartPointer<KeyPressCallBack>::New();
////    key_press_call_back->SetObjects(vimg, iren);//key_press_call_back->SetObjects(vimg, iren);
////////key_press_call_back->SetObjects(&vi, iren);
//////    //key_press_call_back->SetVTKObjects(transversal_plane, iren, sphereActor1, sphereActor2);
////    iren->AddObserver(vtkCommand::CharEvent, key_press_call_back);
//
//
//    
//    
//    vtkSmartPointer<DoubleClickCallback> double_click_call_back = vtkSmartPointer<DoubleClickCallback>::New();
//    //style->AddObserver(style->EventDoubleClick, double_click_call_back);
//    style->AddObserver(vmxGUIInteractorStyleTrackballCamera::EventDoubleClick, double_click_call_back);
//
//    
//    
//    
//
//
//	//===== START THE RENDERING - NEEDED FOR ALL EXAMPLES =====
//	renWin->Render();
//    //iren->Initialize();
//	iren->Start();
//	//=====
//    
//    return 1;
//}
//
//
//
//
//
//
