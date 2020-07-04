/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIRendererTableViewer.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#if defined(vmxGUIRendererTableViewer_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIRendererTableViewer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIRendererTableViewer_EXPORTS
            #define vmxGUIRendererTableViewer_API __declspec(dllexport)
        #else
            #define vmxGUIRendererTableViewer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIRendererTableViewer_EXPORTS
            #define vmxGUIRendererTableViewer_API __attribute__((visibility("default")))
        #else
            #define vmxGUIRendererTableViewer_API
        #endif
    #endif
#endif



#ifndef vmxGUIRendererTableViewer_H
    #define vmxGUIRendererTableViewer_H

#include "mxMatrix.h"
#include "mxList.h"
//#include "mxString.h"
#include "mxString.h"
#include "vmxGUIWidget.h"
//#include "vmxGUIRenderer.h"
//#include "vmxGUIInteractorStyleImageViewer.h"


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif


#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
//#include <vtkImagePlaneWidget.h>
//#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleImage.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageReslice.h>


#include <vtkCellPicker.h>

#include <vtkSmartPointer.h>
#include <vtkImageReslice.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkMatrix4x4.h>
//#include <vtkImageActor.h>
//#include <vtkImageMapper3D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkTransform.h>

#include <vtkPolyDataMapper.h>
#include <vtkCursor3D.h>
#include <vtkCamera.h>


#include <vtkContext2D.h>
#include <vtkContextItem.h>
#include <vtkContextActor.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkBrush.h>
#include <vtkTextProperty.h>
#include <vtkOpenGLContextDevice2D.h>



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




// pre-declaration.
class vmxGUIRendererTableViewer;


/// Sheet of a table.

class vmxGUIRendererTableViewerSheet : public vtkContextItem
{
public:
    
    mxString m_title;
    mxString m_data;
    
    int m_number_of_rows;
    int m_number_of_columns;
    
    vmxGUIRendererTableViewer *m_renderer;
    
    
    vmxGUIRendererTableViewerSheet()
    {
        m_title.Assign("sheet");
        m_data.Assign("{}");
        m_number_of_rows = 1;
        m_number_of_columns = 1;
        m_renderer = NULL;
    };
    
    ~vmxGUIRendererTableViewerSheet() { };
    
    static vmxGUIRendererTableViewerSheet *New();
    vtkTypeMacro(vmxGUIRendererTableViewerSheet, vtkContextItem);
    
    // Redefinition of the Paint event (called when the sheet is being drawn).
    virtual bool Paint(vtkContext2D *painter);

};




/// Table as a list of multiple sheets.

class vmxGUIRendererTableViewer_API vmxGUIRendererTableViewer : public vmxGUIRenderer
{

public:
    
    /// List of all sheets of the (multi)table.
    mxList< vtkSmartPointer< vmxGUIRendererTableViewerSheet > > m_sheets;
    
    /// Index of the currently shown sheet.
    int m_current_sheet_index;
    
    /// Font size.
    int m_font_size;
    
    /// Title of the multisheet table.
    mxString m_title;
    
    /// Context actor.
    vtkSmartPointer<vtkContextActor> m_context_actor;
    
    
    /// Constructor.
    vmxGUIRendererTableViewer(vmxGUIMainWidget *main_widget);
    
    /// Destructor.
    virtual ~vmxGUIRendererTableViewer();
    
    /// Set the input data.
    void AddInputData(const char *sheet_title, const char *sheet_data, int number_of_rows, int number_of_columns);
    
    /// Get VTK interactor style for this renderer (if none specified, leave to return value NULL).
    virtual vtkInteractorStyle* GetVTKInteractorStyle() { return NULL;} ;
        
    /// Executed on mouse move event.
    virtual void OnMouseMove() {};
    
    /// Executed on mouse move event.
    virtual void OnMouseWheelBackward();
    
    /// Executed on mouse move event.
    virtual void OnMouseWheelForward();
    
    /// Executed when left button is released.
    virtual void OnLeftButtonUp() {};
    
    /// Executed when left button is released. Re-implemented to detect double-click.
    virtual void OnLeftButtonDown() {};
    
    /// Executed when key is pressed.
    virtual void OnKeyPress() {};
    
    /// Remove table from the viewer.
    void RemoveData();
    
    /// Set the sheet index - it is the index of hsheet that is displayed.
    void SetSheetIndex(int sheet_index);
    
    /// Set the title of this multisheet table.
    void SetTitle(const char *table_title) { m_title.Assign(table_title); };
};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif




#endif
