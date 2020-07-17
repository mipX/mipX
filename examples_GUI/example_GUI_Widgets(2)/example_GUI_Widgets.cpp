/*=========================================================================
 
 Program:   mipx
 Module:    example_GUI_Widgets.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
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


#include "vmxGUIMenu.h"
#include "vmxGUIMenuBar.h"
#include "vmxGUIFilesDialog.h"
#include "vmxGUILabel.h"
#include "vmxGUIListWidget.h"
#include "vmxGUIInputWidget.h"
#include "vmxGUIButtonGroup.h"
#include "vmxGUIInteractorStyle.h"

#include "vmxGUIRendererImageViewer.h"


#include <vtkJPEGReader.h>
#include <vtkImageMapper3D.h>
#include <vtkImageActor.h> // Note: this is a 3D actor (c.f. vtkImageMapper which is 2D)
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkGenericDataObjectReader.h>




int Slot_vmxGUIFilesDialog_LoadFiles(vmxGUIConnection *connection)
{
    vmxGUIFilesDialog *files_dialog = static_cast<vmxGUIFilesDialog *>(connection->GetSlotCallerObject());
    
    cout<<"vmxGUIFilesDialog_LoadFiles(): "<<files_dialog->GetClassName().Get_C_String()<<endl;
    
    mxArray<mxString> array_of_selected_items_names;
    files_dialog->m_list_widget.GetSelectedItemsNames(array_of_selected_items_names);
    
    if(array_of_selected_items_names.IsEmpty()) return 0;
    
    
    mxString file_name, f_temp, extension;
    
    files_dialog->GetSelectedFilePath(file_name);
    
    file_name.ExtractFileNameAndFileExtension(f_temp, extension);
    if(extension!="jpg" && extension!="JPG") return 0;
    
    //----- Add a new renderer to the render window -----
    //vmxGUIRendererImage *image_renderer = new vmxGUIRendererImage();//vmxGUIRendererImage image_renderer;
    vmxGUIRendererImageViewer *image_renderer = new vmxGUIRendererImageViewer();//vmxGUIRendererImage image_renderer;
    files_dialog->GetMainWidget()->AddRenderer(image_renderer);
    //----------
    
    
    cout<<endl<<"vmxGUIFilesDialog_LoadFiles(): loading: "<<file_name.Get_C_String()<<endl;
    
    //----- Create an image actor -----
    vtkSmartPointer<vtkJPEGReader> jpg_reader = vtkSmartPointer<vtkJPEGReader>::New();
    jpg_reader->SetFileName(file_name.Get_C_String());// "/Users/danilobabin/Downloads/Petra u vrticu/IMG_2071.jpg");
    jpg_reader->Update();
    vtkSmartPointer<vtkImageActor> image_actor = vtkSmartPointer<vtkImageActor>::New();
    image_actor->GetMapper()->SetInputConnection(jpg_reader->GetOutputPort());
    image_renderer->GetVTKRenderer()->AddActor(image_actor);
    //----------

    files_dialog->GetMainWidget()->RedoPlacement();
    
    return 1;
};



//
//
//
//int Slot_vmxGUIMenu_LoadFiles(vmxGUIConnection *connection)
//{
//    vmxGUIFilesDialog *files_dialog = static_cast<vmxGUIFilesDialog *>(connection->GetReceiverObject());
//    
//    cout<<"Slot_vmxGUIMenu_LoadFiles(): "<<files_dialog->GetClassName().Get_C_String()<<endl;
//    
//    files_dialog->OpenFiles(Slot_vmxGUIFilesDialog_LoadFiles);
//    return 1;
//};
//






class MainApp : public vmxGUIMainWidget
{
    
public:
    
    vmxGUIMenuBar m_menu_bar;
    
    vmxGUIFilesDialog *m_files_dialog;
    
    //mxArray<vmxGUIRenderer3DTrackBallCamera> m_array_of_3D_renderers;
    
    mxArray< vmxGUIRendererImageViewer* > m_array_of_image_renderers;
    
    //vmxGUIRendererImage m_image_renderer;
    vmxGUIRendererImageViewer* m_image_renderer;

    
    static int Slot_vmxGUIMenu_LoadFiles2(vmxGUIConnection *connection)
    {
        vmxGUIFilesDialog *fd = static_cast<vmxGUIFilesDialog *>(connection->GetSlotCallerObject());
        
        cout<<"MainApp::Slot_vmxGUIMenu_LoadFiles(): "<<fd->GetClassName().Get_C_String()<<endl;
        
        //fd->m_connection_open_files = (*connection);
        //fd->OpenFiles(Slot_vmxGUIFilesDialog_LoadFiles);
        
        fd->OpenFiles(Slot_vmxGUIFilesDialog_LoadFiles, connection);
        
//        vmxGUIFilesDialog::OpenFiles(Slot_vmxGUIFilesDialog_LoadFiles);

        return 1;
    };
    
 
    MainApp()
    {
        // Initialization.
        {
            vmxGUIConnection::internal_SetMainWidget(this);
        }
        
        //----- File Dialog -----
        m_files_dialog = new vmxGUIFilesDialog(this);
        m_files_dialog->SetColor(1,0,0);
        m_files_dialog->SetFontSize(20);
        //m_files_dialog.SetPlacementOrderToBelow();
        //----------

        
        
        //----- Menu Bar -----
        m_menu_bar.SetMainWidget(this);
        vmxGUIMenuBarItem *file_menu_item = m_menu_bar.AddItem("File");
        vmxGUIMenuItem *file_load_item = file_menu_item->GetMenu()->AddSubMenu("Load Data"); //AddItem("Load Data");
        vmxGUIMenuItem *file_load_jpg_item = file_load_item->GetSubMenu()->AddItem("JPG");
        //file_load_jpg_item->AttachSlot(new FileMenu_LoadJPG(&files_dialog));
        file_load_item->GetSubMenu()->AddSubMenu("PNG");
        file_menu_item->GetMenu()->AddItem("Close");
        file_menu_item->GetMenu()->BuildMenu();
        m_menu_bar.AddItem("Edit");
        m_menu_bar.AddItem("Help");
        m_menu_bar.SetColor(1,1,0);
        m_menu_bar.SetFontSize(20);
        m_menu_bar.SetInteractor(this->GetInteractor());
        m_menu_bar.SetPlacementToUpperLeft();
        m_menu_bar.SetPlacementOrderToAbove();
        m_menu_bar.SetVisibility(1);
        //----------
        
        //vmxGUIConnection::New(file_load_item->GetSubMenu(), file_load_jpg_item, LeftButtonUpEvent, m_files_dialog, MainApp::Slot_vmxGUIMenu_LoadFiles2);
        
        vmxGUIConnection::New(file_load_item->GetSubMenu(), LeftButtonUpEvent, m_files_dialog, m_files_dialog, MainApp::Slot_vmxGUIMenu_LoadFiles2);
        
//        //----- Create a sphere -----
//        vtkSmartPointer<vtkSphereSource> sphere_source = vtkSmartPointer<vtkSphereSource>::New();
//        sphere_source->SetCenter(0.0, 0.0, 0.0);
//        sphere_source->SetRadius(5);
//        sphere_source->Update();
//        vtkSmartPointer<vtkPolyDataMapper> sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//        sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());
//        vtkSmartPointer<vtkActor> sphere_actor = vtkSmartPointer<vtkActor>::New();
//        sphere_actor->SetMapper(sphere_mapper);
////        this->GetRenderer_3D()->AddActor(sphere_actor);
//        //----------
//

//        //----- Add a new renderer to the render window -----
//        //vmxGUIRendererImage image_renderer;
//        this->AddRenderer(&m_image_renderer);
//        //----------
//
//
//        //----- Create an image actor -----
//        vtkSmartPointer<vtkJPEGReader> jpg_reader = vtkSmartPointer<vtkJPEGReader>::New();
//        jpg_reader->SetFileName("/Users/danilobabin/Downloads/Petra u vrticu/IMG_2071.jpg");
//        jpg_reader->Update();
//
//        // /Users/danilobabin/Dropbox/-DIP_IMAGES/-AORTA/From_Laura_and_Dan/PWV_data_converted/MFS002/00_cine.vtk
//
//        //vtkSmartPointer<vtkImageActor> image_actor = vtkSmartPointer<vtkImageActor>::New();
//        //image_actor->GetMapper()->SetInputConnection(jpg_reader->GetOutputPort());
//        //m_image_renderer.GetVTKRenderer()->AddActor(image_actor);
//
        vtkSmartPointer<vtkGenericDataObjectReader> sp_reader = vtkSmartPointer<vtkGenericDataObjectReader>::New();
        sp_reader->SetFileName("/Users/danilobabin/Dropbox/-DIP_IMAGES/Aorta/From_Laura_and_Dan/PWV_data_converted/MFS002/anatomical_3D.vtk");
        sp_reader->Update();

        vtkSmartPointer< vtkImageData > image_data = vtkSmartPointer< vtkImageData >::New();
        image_data->DeepCopy(sp_reader->GetOutput());

////        //image_data->DeepCopy(jpg_reader->GetOutput());
////        m_image_renderer.SetImageData(image_data);
//
//        m_image_renderer.SetImageData(image_data); //m_image_renderer.SetImageData((vtkImageData*)sp_reader->GetOutput());
//        //m_image_renderer.SetDirectionToCoronal();
//        m_image_renderer.SetDirectionToSagittal();
//        //----------
//
//

        //mxArray< vmxGUIRendererImageWidget > m_array_of_image_renderers;
        m_array_of_image_renderers.SetNumberOfElements(3);
        for(unsigned int i=0; i<m_array_of_image_renderers.GetNumberOfElements(); i++)
        {
            m_array_of_image_renderers[i] = new vmxGUIRendererImageViewer(this);
            this->AddRenderer(m_array_of_image_renderers[i]);
            m_array_of_image_renderers[i]->SetImageData(image_data);
        }
        m_array_of_image_renderers[0]->SetDirectionToTransversal();
        m_array_of_image_renderers[1]->SetDirectionToCoronal();
        m_array_of_image_renderers[2]->SetDirectionToSagittal();
        
        
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[0], KeyPressEvent, m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(m_array_of_image_renderers[0]->m_index_slice)));
//        }
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[0], KeyPressEvent, m_array_of_image_renderers[2], vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(m_array_of_image_renderers[0]->m_index_slice)));
//        }
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[1], KeyPressEvent, m_array_of_image_renderers[0], vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(m_array_of_image_renderers[1]->m_index_slice)));
//        }
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[1], KeyPressEvent, m_array_of_image_renderers[2], vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(m_array_of_image_renderers[1]->m_index_slice)));
//        }
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[2], KeyPressEvent, m_array_of_image_renderers[0], vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(m_array_of_image_renderers[2]->m_index_slice)));
//        }
//        {
//            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[2], KeyPressEvent, m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_SetIndexSlice);
//            c->SetPassedDataInt((int*)(&(m_array_of_image_renderers[2]->m_index_slice)));
//        }
        

        

//
//
//
//
//
//        //----- Add a number of new 3D renderers to the render window -----
//        int number_of_renderers_to_add = 2;
//        //mxArray<vmxGUIRenderer3DTrackBallCamera> array_of_3D_renderers;
//        m_array_of_3D_renderers.SetNumberOfElements(number_of_renderers_to_add);
//
//        mxArray< vtkSmartPointer<vtkActor> > array_of_sphere_actors;
//        array_of_sphere_actors.SetNumberOfElements(number_of_renderers_to_add);
//        for(unsigned int i=0; i<m_array_of_3D_renderers.GetNumberOfElements(); i++)
//        {
//            array_of_sphere_actors[i] = vtkSmartPointer<vtkActor>::New();
//            this->AddRenderer(&(m_array_of_3D_renderers[i]));
//            m_array_of_3D_renderers[i].GetVTKRenderer()->AddActor(sphere_actor);
//        }
//        //----------
 
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


//    
////    //----- Label -----
////    vmxGUILabel label;
////    label.SetMainWidget(&main_widget);
////    label.SetText("Ovo je samo proba!");
////    label.SetColor(1,0,0);
////    label.SetFontSize(20);
////    label.SetInteractor(main_widget.GetInteractor());
////    label.SetPlacementToUpperLeft();
////    label.SetVisibility(1);
//
//    
//    //----- File Dialog -----
//    vmxGUIFilesDialog files_dialog(&main_widget);
//    files_dialog.SetMainWidget(&main_widget);
//    files_dialog.SetColor(1,0,0);
//    files_dialog.SetFontSize(20);
//    //files_dialog.SetInteractor(main_widget.GetInteractor());
//    //files_dialog.OpenFiles();//"/Users/danilobabin/-DIP_IMAGES/");
//    //files_dialog.SetPlacementOrderToBelow();
//    //----------
//
//
//    
//    vmxGUIMenuBar menu_bar;
//    menu_bar.SetMainWidget(&main_widget);
//    vmxGUIMenuBarItem *file_menu_item = menu_bar.AddItem("File");
//    vmxGUIMenuItem *file_load_item = file_menu_item->GetMenu()->AddSubMenu("Load Data"); //AddItem("Load Data");
//    vmxGUIMenuItem *file_load_jpg_item = file_load_item->GetSubMenu()->AddItem("JPG");
//    file_load_jpg_item->AttachSlot(new FileMenu_LoadJPG(&files_dialog));
//    file_load_item->GetSubMenu()->AddSubMenu("PNG");
//    file_menu_item->GetMenu()->AddItem("Close");
//    file_menu_item->GetMenu()->BuildMenu();
//    menu_bar.AddItem("Edit");
//    menu_bar.AddItem("Help");
//    menu_bar.SetColor(1,1,0);
//    menu_bar.SetFontSize(20);
//    menu_bar.SetInteractor(main_widget.GetInteractor());
//    menu_bar.SetPlacementToUpperLeft();
//    menu_bar.SetPlacementOrderToAbove();
//    menu_bar.SetVisibility(1);
//
//    
//    
//
//    
//    
//
//    
//    
////    vmxGUIInputWidget int_input;
////    int_input.SetMainWidget(&main_widget);
////    int_input.SetColor(1,1,1);
////    int_input.SetFontSize(20);
////    int_input.SetInputTypeToIntegerNumber(837);
////    int_input.SetDescriptionText("Integer Number: ");
////    int_input.SetPlacementToLowerLeft();
////    int_input.SetVisibility(1);
////    
////    
////    vmxGUIInputWidget double_input;
////    double_input.SetMainWidget(&main_widget);
////    double_input.SetColor(1,1,1);
////    double_input.SetFontSize(20);
////    double_input.SetInputTypeToDoubleNumber(873.23);
////    double_input.SetDescriptionText("Double Number: ");
////    double_input.SetPlacementToLowerRight();
////    //double_input.SetStretchingOver_X_Axis(1);
////    double_input.SetVisibility(1);
////    
////    
////    
////    vmxGUIInputWidget item_input;
////    item_input.SetMainWidget(&main_widget);
////    item_input.SetColor(1,1,1);
////    item_input.SetFontSize(20);
////    item_input.SetInputTypeToItem();
////    item_input.SetDescriptionText("Item input: ");
////    item_input.SetPlacementToLowerLeft();
////    item_input.SetVisibility(1);
////    
////
////    
////    vmxGUIButtonGroup check_boxes;
////    check_boxes.SetButtonTypeToCheckBox();
////    check_boxes.SetDescriptionText("Check boxes: ");
////    check_boxes.AddItem("first");
////    check_boxes.AddItem("second",1);
////    check_boxes.AddItem("third");
////    check_boxes.SetMainWidget(&main_widget);
////    check_boxes.SetColor(1,1,1);
////    check_boxes.SetFontSize(20);
////    check_boxes.SetPlacementToLowerLeft();
////    check_boxes.SetVisibility(1);
////   
////    
////    vmxGUIButtonGroup radio_buttons;
////    radio_buttons.SetButtonTypeToRadioButton();
////    radio_buttons.SetDescriptionText("Radio buttons: ");
////    radio_buttons.AddItem("alfa",1);
////    radio_buttons.AddItem("beta");
////    radio_buttons.AddItem("gamma");
////    radio_buttons.SetMainWidget(&main_widget);
////    radio_buttons.SetColor(1,1,1);
////    radio_buttons.SetFontSize(20);
////    radio_buttons.SetPlacementToLowerLeft();
////    radio_buttons.SetVisibility(1);
////
////    
////    
////    vmxGUIButtonGroup push_buttons;
////    push_buttons.SetButtonTypeToPushButton();
////    push_buttons.SetDescriptionText(" ");
////    push_buttons.AddItem("OK");
////    push_buttons.AddItem("Cancel");
////    push_buttons.SetMainWidget(&main_widget);
////    push_buttons.SetColor(1,1,1);
////    push_buttons.SetFontSize(20);
////    push_buttons.SetPlacementToLowerLeft();
////    push_buttons.SetPlacementOrderToBelow();
////    push_buttons.SetVisibility(1);
////
////
////    
////    
////    //----- List Widget -----
////    vmxGUIListWidget list_widget;
////    list_widget.SetMainWidget(&main_widget);
////    list_widget.SetColor(1,1,0);
////    list_widget.SetFontSize(16);
////    for(int i=0; i<100; i++)
////    {
////        mxString name;
////        name.Append("item_");
////        name.AppendNumber(i);
////        list_widget.AddItem(name.Get_C_String(),i%4);
////        cout<<" "<<name.Get_C_String()<<" ";
////    }
////    vmxGUIListWidgetItem *item = list_widget.AddItem("item_100");
////    list_widget.AddItem("Something new",item);
////    list_widget.BuildListWidget();
////    list_widget.SetPlacementToUpperRight();
////    list_widget.SetVisibility(1);
////    //----------
////
//    
//    
//    //----- Create a sphere -----
//    vtkSmartPointer<vtkSphereSource> sphere_source = vtkSmartPointer<vtkSphereSource>::New();
//    sphere_source->SetCenter(0.0, 0.0, 0.0);
//    sphere_source->SetRadius(5);
//    sphere_source->Update();
//    vtkSmartPointer<vtkPolyDataMapper> sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//    sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());
//    vtkSmartPointer<vtkActor> sphere_actor = vtkSmartPointer<vtkActor>::New();
//    sphere_actor->SetMapper(sphere_mapper);
//    //sphere_actor->GetProperty()->SetOpacity(0.5);
//    main_widget.GetRenderer_3D()->AddActor(sphere_actor);
//    //----------
//    
//    
//    //----- Add a new renderer to the render window -----
//    vmxGUIRendererImage image_renderer;
//    main_widget.AddRenderer(&image_renderer);
//    //----------
//    
//    
//    //----- Create an image actor -----
//    vtkSmartPointer<vtkJPEGReader> jpg_reader = vtkSmartPointer<vtkJPEGReader>::New();
//    jpg_reader->SetFileName("/Users/danilobabin/Downloads/Petra u vrticu/IMG_2071.jpg");
//    jpg_reader->Update();
//    vtkSmartPointer<vtkImageActor> image_actor = vtkSmartPointer<vtkImageActor>::New();
//    image_actor->GetMapper()->SetInputConnection(jpg_reader->GetOutputPort());
//    image_renderer.GetVTKRenderer()->AddActor(image_actor);
//    //----------
//
//    
//    //----- Add a number of new 3D renderers to the render window -----
//    int number_of_renderers_to_add = 2;
//    mxArray<vmxGUIRenderer3DTrackBallCamera> array_of_3D_renderers;
//    array_of_3D_renderers.SetNumberOfElements(number_of_renderers_to_add);
//    mxArray< vtkSmartPointer<vtkActor> > array_of_sphere_actors;
//    array_of_sphere_actors.SetNumberOfElements(number_of_renderers_to_add);
//    for(unsigned int i=0; i<array_of_3D_renderers.GetNumberOfElements(); i++)
//    {
//        array_of_sphere_actors[i] = vtkSmartPointer<vtkActor>::New();
//        main_widget.AddRenderer(&(array_of_3D_renderers[i]));
//        array_of_3D_renderers[i].GetVTKRenderer()->AddActor(sphere_actor);
//    }
//    //----------
//
//    
//    
//    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
//    main_widget.StartInteraction();
//
//    
//    return 1;
//    
//    
//    
//    
//    
//    
//    
////    
////    //    //----- Menu -----
////    //    vmxMenu menu;
////    //    menu.SetMainWidget(&main_widget);
////    //    menu.SetClipBoard(&cb);
////    //    //menu.SetDataObjectTree(&tree);
////    //    //menu.SetOrigin(0,30);
////    //    menu.SetColor(1,0,0);
////    //    menu.SetFontSize(20);
////    //    vmxMenuItem *item = menu.AddItem("Hello world to call slot",3);
////    //    item->AttachSlot(new TryMenuSlot);
////    //    menu.AddItem("Something new");
////    //    vmxMenuItem *item2 =  menu.AddSubMenu("Has submenu");
////    //    item2->GetSubMenu()->AddItem("item 1");
////    //    item2->GetSubMenu()->AddItem("item 2");
////    //    menu.AddItem("Yet another one",1);
////    //    menu.AddItem("Hello world",2);
////    //    menu.AddItem("Something new");
////    //    menu.AddItem("Another thing");
////    //    menu.AddItem("Yet another one");
////    //    menu.AddItem("Hello world}");
////    //    menu.AddItem("Something new>");
////    //    menu.AddItem("Another thing>");
////    //    menu.AddItem("Yet another one");// Hello world Hello world");
////    //    menu.AddItem("Hello world",3);
////    //    menu.AddItem("Something new");
////    //    menu.AddItem("Another thing");
////    //    menu.AddItem("Yet another one",3);
////    //    menu.AddItem("Hello world",1);
////    //    menu.AddItem("Something new");
////    //    menu.AddItem("Another thing",2);
////    //    menu.AddItem("Yet another one");
////    //    vmxMenuItem *item3 =  menu.AddSubMenu("Has submenu");
////    //    item3->GetSubMenu()->AddItem("item 3");
////    //    item3->GetSubMenu()->AddItem("item 4");
////    //    item3->GetSubMenu()->AddItem("item 5");
////    //    item3->GetSubMenu()->AddItem("item 6");
////    //    menu.BuildMenu();
////    //    menu.SetInteractor(iren);
////    //    //menu.SetPlacementToLowerRight();
////    //    //menu.SetPlacementToUpperRight();
////    //    menu.SetPlacementToUpperLeft();
////    //    //menu.SetPlacementToLowerLeft();
////    //    //menu.SetPlacementToLowerCenter();
////    //    //menu.SetPlacementToUpperCenter();
////    //    //menu.SetPlacementToCenterLeft();
////    //    //menu.SetPlacementToCenterRight();
////    //    //menu.SetPlacementToRelative(100, 50);
////    //    //------------
////    
////    
////    
////    
//    
////    
////    
////
////    vmxGUIWidgetCollection collection;
////    collection.SetMainWidget(&main_widget);
////    collection.AddWidget(&text_input4);
////    collection.AddWidget(&check_boxes);
////    
////    collection.SetPlacementOrderToBelow();
////    collection.SetPlacementToUpperRight();
////    
////    //check_boxes.SetInteractor(iren);
////    
////    collection.SetVisibility(1);
////
////    
//  
//
//}




