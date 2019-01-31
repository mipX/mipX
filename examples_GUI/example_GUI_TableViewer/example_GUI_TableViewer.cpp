/*=========================================================================
 
 Program:   mipx
 Module:    example_GUI_TableViewer.cpp
 
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
 
 This example illustrates how to create a simple app with table viewer viewing of tables.
 
 */




#include "vmxGUIMenu.h"
#include "vmxGUIMenuBar.h"
#include "vmxGUIFilesDialog.h"
#include "vmxGUILabel.h"
#include "vmxGUIListWidget.h"
#include "vmxGUIInputWidget.h"
#include "vmxGUIButtonGroup.h"
#include "vmxGUIInteractorStyle.h"
#include "vmxGUIRendererTableViewer.h"


#include <vtkGenericDataObjectReader.h>
#include <vtkMath.h>






class MainApp : public vmxGUIMainWidget
{
    
public:
    
    vmxGUIRendererTableViewer *m_table_renderer;
    
 
    MainApp()
    {
        // Initialization.
        {
            vmxGUIConnection::internal_SetMainWidget(this);
        }
        
        
        m_table_renderer = new vmxGUIRendererTableViewer(this);
        this->AddRenderer(m_table_renderer);
        
        
        const char *input = " {Table}{A}{B}{C} {1}{1a}{1b}{1c} {2}{2a}{2b}{2c}";
        const char *input2 = " {Table}{A}{B}{C} {1}{1a}{1b}{1c}";
        const char *input3 = " {Table}{A}{B} {1}{1a}{1b}";
        
        m_table_renderer->SetTitle("my multi-sheet table");
        m_table_renderer->AddInputData("my sheet", input, 3, 4);
        m_table_renderer->AddInputData("my sheet2", input2, 2, 4);
        m_table_renderer->AddInputData("my sheet3", input3, 2, 3);

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




