/*=========================================================================
 
 Program:   mipx
 Module:    example_App_Histogram.cpp
 
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
 
 This example shows display of tables.
 
 This example demonstrates the basic use of data list widget to construct data objects. The advantage is that the user can
 visualy manipulate the data using the data list widget.
 
 */


#include "mxBIP.h"
#include "mxGIP.h"

#include "vmxTable.h"

#include "vmxAppWidget.h"
//#include "vmxAppImageViewer.h"
#include "vmxAppTableViewer.h"
//#include "vmxImage.h"

#include <vtkMath.h>




class MainApp : public vmxAppMainWidget
{
    vmxAppTableViewer m_table_viewer;
    
public:
    
    
    static int Slot_RecalculateImage(vmxGUIConnection *connection);
    
    
    MainApp()
    {
        // Set up the main widget to use its default structures.
        this->SetDataListWidgetToDefault();
        //this->SetRenderer3DToDefaultTrackballCamera();
        
        // Register viewers.
        this->RegisterViewer(&m_table_viewer);

        // Register the factory with the data list widget.
        // The widget will later use this factory to construct objects of the given type.
        this->GetDataListWidget()->AddFactory(new vmxTableFactory);
        
        
        // Create an object using the data list widget.
        vmxTable *table1 = static_cast<vmxTable*> (this->GetDataListWidget()->Create("vmxTable","patient01"));
        
        table1->SetNumberOfSheets(2);
        table1->GetSheet(0)->SetTitle("examination 01");
        table1->GetSheet(0)->SetDimensions(2, 4);
        table1->GetSheet(0)->SetLabelOfRow(0, "blood pressure");
        table1->GetSheet(0)->SetLabelOfRow(1, "pulse");
        table1->GetSheet(0)->SetLabelOfColumn(0, "8h");
        table1->GetSheet(0)->SetLabelOfColumn(1, "12h");
        table1->GetSheet(0)->SetLabelOfColumn(2, "16h");
        table1->GetSheet(0)->SetLabelOfColumn(3, "20h");
        
        table1->GetSheet(0)->SetString(0,0, "110/70");
        table1->GetSheet(0)->SetString(0,1, "120/70");
        table1->GetSheet(0)->SetString(0,2, "130/80");
        table1->GetSheet(0)->SetString(0,3, "120/80");
        table1->GetSheet(0)->SetString(1,0, "50");
        table1->GetSheet(0)->SetString(1,1, "55");
        table1->GetSheet(0)->SetString(1,2, "60");
        table1->GetSheet(0)->SetString(1,3, "60");
        
        table1->GetSheet(1)->SetTitle("examination 02");
        table1->GetSheet(1)->SetDimensions(2, 3);
        table1->GetSheet(1)->SetLabelOfRow(0, "blood pressure");
        table1->GetSheet(1)->SetLabelOfRow(1, "pulse");
        table1->GetSheet(1)->SetLabelOfColumn(0, "7h");
        table1->GetSheet(1)->SetLabelOfColumn(1, "13h");
        table1->GetSheet(1)->SetLabelOfColumn(2, "19h");
        
        table1->GetSheet(1)->SetString(0,0, "100/60");
        table1->GetSheet(1)->SetString(0,1, "110/70");
        table1->GetSheet(1)->SetString(0,2, "100/50");
        table1->GetSheet(1)->SetString(1,0, "55");
        table1->GetSheet(1)->SetString(1,1, "60");
        table1->GetSheet(1)->SetString(1,2, "60");


        cout<<endl<<table1->GetSheet(0)->GetSheetAsString().Get_C_String()<<endl;
        
        
        // Create an object using the data list widget.
        vmxTable *table2 = static_cast<vmxTable*> (this->GetDataListWidget()->Create("vmxTable","patient02"));
        
        table2->SetNumberOfSheets(1);
        table2->GetSheet(0)->SetTitle("patient data");
        table2->GetSheet(0)->SetDimensions(3, 1);
        table2->GetSheet(0)->SetLabelOfRow(0, "date of \nbirth");
        table2->GetSheet(0)->SetLabelOfRow(1, "hight");
        table2->GetSheet(0)->SetLabelOfRow(2, "weight");
        table2->GetSheet(0)->SetLabelOfColumn(0, "info");
        
        table2->GetSheet(0)->SetString(0,0, "25/04/1982");
        table2->GetSheet(0)->SetString(0,1, "185 cm");
        table2->GetSheet(0)->SetString(0,2, "88 kg");

        cout<<endl<<table2->GetSheet(0)->GetSheetAsString().Get_C_String()<<endl;

        
       
//        m_table_viewer.AddInputTable(table);
        //image_viewer.GetRenderer(0)->SetMappingToGrayScale();
        //image_viewer.SetInputImage(image);
        //image_viewer.GetRenderer()->FitImageToScreen();
        
//        vmxGUIConnection *connection = vmxGUIConnection::New(image_viewer3.GetRenderer(), KeyPressEvent,
//                                                             this, MainApp::Slot_RecalculateImage);
//        connection->SetPassedData(this);
        
    };
    
    
    ~MainApp(){};
    
};



int main()
{
    
    // Create the main widget. This is the first step that should be done in each application. The widget contains in itself
    // a render window, a renderer for GUI objects, a renderer for 3D data view, and the interactor.
    MainApp main_widget;
    
    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
    main_widget.StartInteraction();
    
    return 1;
}

