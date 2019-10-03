/*=========================================================================
 
 Program:   mipx
 Module:    example_GUI_OrthogonalImageViewer.cpp
 
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
 
 This example illustrates how to create a simple app with 3 image viewes for orthogonal (simultaneous) viewing of images.
 Picking on one image will (using the system of connections) scroll through the image in the other viewers.
 
 */




#include "vmxGUIMenu.h"
#include "vmxGUIMenuBar.h"
#include "vmxGUIFilesDialog.h"
#include "vmxGUILabel.h"
#include "vmxGUIListWidget.h"
#include "vmxGUIInputWidget.h"
#include "vmxGUIButtonGroup.h"
#include "vmxGUIInteractorStyle.h"
#include "vmxGUIRendererImageViewer.h"


#include <vtkGenericDataObjectReader.h>


void CreateTestImage02(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C);




class MainApp : public vmxGUIMainWidget
{
    
public:
    
    
    mxArray< vmxGUIRendererImageViewer* > m_array_of_image_renderers;
    
    
 
    MainApp()
    {
        // Initialization.
        {
            vmxGUIConnection::internal_SetMainWidget(this);
        }
        
        
        vtkSmartPointer<vtkGenericDataObjectReader> reader2 = vtkSmartPointer<vtkGenericDataObjectReader>::New();
        mxString file_name;
        file_name.Assign(MIPX_FILES_DATA_PATH);
        file_name.Append("aorta_trufisp_sag.vtk");
        reader2->SetFileName(file_name.Get_C_String());
        reader2->Update();
        

        
        vtkSmartPointer< vtkImageData > image1 = vtkSmartPointer< vtkImageData >::New();
        
        
        image1->DeepCopy(reader2->GetOutput());
        //CreateTestImage02(image1, 60, 50, 40);
        
        
        
        m_array_of_image_renderers.SetNumberOfElements(3);
        for(unsigned int i=0; i<m_array_of_image_renderers.GetNumberOfElements(); i++)
        {
            m_array_of_image_renderers[i] = new vmxGUIRendererImageViewer(this);
            this->AddRenderer(m_array_of_image_renderers[i]);
        }
        m_array_of_image_renderers[0]->SetImageData(image1);
        m_array_of_image_renderers[1]->SetImageData(image1);
        m_array_of_image_renderers[2]->SetImageData(image1);
        
        m_array_of_image_renderers[2]->SetDirectionToTransversal();
        m_array_of_image_renderers[1]->SetDirectionToCoronal();
        m_array_of_image_renderers[0]->SetDirectionToSagittal();
        
        m_array_of_image_renderers[0]->SetMappingToGrayScale(); //m_array_of_image_renderers[0]->SetMappingToColor();
        m_array_of_image_renderers[1]->SetMappingToGrayScale(); //m_array_of_image_renderers[1]->SetMappingToColor();
        m_array_of_image_renderers[2]->SetMappingToGrayScale(); //m_array_of_image_renderers[2]->SetMappingToColor();
        
        

        // Create connections for orthogonal viewing
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[0], ImagePickedEvent, m_array_of_image_renderers[1], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[0]->GetPickedIndexes());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[0], ImagePickedEvent, m_array_of_image_renderers[2], m_array_of_image_renderers[1],vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[0]->GetPickedIndexes());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[1], ImagePickedEvent, m_array_of_image_renderers[0], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[1]->GetPickedIndexes());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[1], ImagePickedEvent, m_array_of_image_renderers[2], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[1]->GetPickedIndexes());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[2], ImagePickedEvent, m_array_of_image_renderers[0], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[2]->GetPickedIndexes());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[2], ImagePickedEvent, m_array_of_image_renderers[1], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[2]->GetPickedIndexes());
        }
        
        
        
        m_array_of_image_renderers[0]->FitImageToScreen();
        m_array_of_image_renderers[1]->FitImageToScreen();
        m_array_of_image_renderers[2]->FitImageToScreen();
        

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





// Creates an image of with a sphere and a cube.
void CreateTestImage02(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C)
{
    image->SetDimensions(dimension_C, dimension_R, dimension_S);
    image->AllocateScalars(VTK_UNSIGNED_CHAR,1);
    
    int center_sphere_s = dimension_S/3;
    int center_sphere_r = dimension_R/3;
    int center_sphere_c = dimension_C/3;
    
    int center_cube_s = (dimension_S*2)/3;
    int center_cube_r = (dimension_R*2)/3;
    int center_cube_c = (dimension_C*2)/3;
    
    int half_size = (dimension_S-1)/3;
    if((dimension_R-1)/3 < half_size) half_size = (dimension_R-1)/3;
    if((dimension_C-1)/3 < half_size) half_size = (dimension_C-1)/3;
    
    int radius = dimension_S/3;
    if(dimension_R/3 < radius) radius = dimension_R/3;
    if(dimension_C/3 < radius) radius = dimension_C/3;
    
    
    for(unsigned int s = 0; s < dimension_S; s++)
    {
        for(unsigned int r = 0; r < dimension_R; r++)
        {
            for(unsigned int c = 0; c < dimension_C; c++)
            {
                unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(c,r,s));
                pixel[0] = 0;
                
                int d = (int) (sqrt( (s-center_sphere_s)*(s-center_sphere_s) + (r-center_sphere_r)*(r-center_sphere_r) + (c-center_sphere_c)*(c-center_sphere_c) ));
                if(d<radius) pixel[0] = d;
                else
                {
                    int l_max = s-center_cube_s;
                    if(l_max<0) l_max = -l_max;
                    
                    int m = r-center_cube_r;
                    if(m<0) m = -m;
                    if(l_max<m) l_max = m;
                    
                    m = c-center_cube_c;
                    if(m<0) m = -m;
                    if(l_max<m) l_max = m;
                    
                    //unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(c,r,s));
                    if(l_max<half_size) pixel[0] = l_max;
                }
            }
        }
    }
}
