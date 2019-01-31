/*=========================================================================
 
 Program:   mipx
 Module:    example_GUI_ParallelImageViewer.cpp
 
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
 
 This example illustrates how to create a simple app with 2 image viewes for parallel (simultaneous) vieweing of images.
 Scrolling through one image will (using the system of connections) scroll through the image in the second viewer.
 
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



void CreateTestImage(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C, unsigned char initial_voxel_value);
void CreateTestImage1(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C);
void CreateTestImage2(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C);

void CreateTestImage01(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C);
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
        

        
        vtkSmartPointer< vtkImageData > image1 = vtkSmartPointer< vtkImageData >::New();
        vtkSmartPointer< vtkImageData > image2 = vtkSmartPointer< vtkImageData >::New();
        
        CreateTestImage01(image1, 40, 50, 50);
        CreateTestImage02(image2, 40, 50, 50);
        

        m_array_of_image_renderers.SetNumberOfElements(2);
        for(unsigned int i=0; i<m_array_of_image_renderers.GetNumberOfElements(); i++)
        {
            m_array_of_image_renderers[i] = new vmxGUIRendererImageViewer(this);
            this->AddRenderer(m_array_of_image_renderers[i]);
        }
        m_array_of_image_renderers[0]->SetImageData(image1);
        m_array_of_image_renderers[1]->SetImageData(image2);
        
        m_array_of_image_renderers[0]->SetDirectionToTransversal();
        m_array_of_image_renderers[1]->SetDirectionToTransversal();
        
        m_array_of_image_renderers[0]->SetMappingToColor();
        m_array_of_image_renderers[1]->SetMappingToColor();
        
        
        
        
        
        
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[0], ImageSliceChangeEvent, m_array_of_image_renderers[1], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_SetIndexSlice);
            c->SetPassedDataInt((int*)(&(m_array_of_image_renderers[0]->m_index_slice)));
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[1], ImageSliceChangeEvent, m_array_of_image_renderers[0], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_SetIndexSlice);
            c->SetPassedDataInt((int*)(&(m_array_of_image_renderers[1]->m_index_slice)));
        }
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





void CreateTestImage(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C, unsigned char initial_voxel_value)
{
    image->SetDimensions(dimension_C, dimension_R, dimension_S);
    image->AllocateScalars(VTK_UNSIGNED_CHAR,1);

    unsigned int counter = initial_voxel_value;
    for(unsigned int s = 0; s < dimension_S; s++)
    {
        for(unsigned int r = 0; r < dimension_R; r++)
        {
            for(unsigned int c = 0; c < dimension_C; c++)
            {
                counter = (counter+1)%256;
                unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(c,r,s));
                pixel[0] = counter;
            }
        }
    }
}



// Creates an image of Euclidean distances from the center of the image.
void CreateTestImage1(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C)
{
    image->SetDimensions(dimension_C, dimension_R, dimension_S);
    image->AllocateScalars(VTK_UNSIGNED_CHAR,1);
    
    int center_s = dimension_S/2;
    int center_r = dimension_R/2;
    int center_c = dimension_C/2;
    
    for(unsigned int s = 0; s < dimension_S; s++)
    {
        for(unsigned int r = 0; r < dimension_R; r++)
        {
            for(unsigned int c = 0; c < dimension_C; c++)
            {
                int d = (int) (sqrt( (s-center_s)*(s-center_s) + (r-center_r)*(r-center_r) + (c-center_c)*(c-center_c) ));
                unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(c,r,s));
                pixel[0] = d;
            }
        }
    }
}


// Creates an image of l_max distances from the center of the image.
void CreateTestImage2(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C)
{
    image->SetDimensions(dimension_C, dimension_R, dimension_S);
    image->AllocateScalars(VTK_UNSIGNED_CHAR,1);
    
    int center_s = dimension_S/2;
    int center_r = dimension_R/2;
    int center_c = dimension_C/2;
    
    for(unsigned int s = 0; s < dimension_S; s++)
    {
        for(unsigned int r = 0; r < dimension_R; r++)
        {
            for(unsigned int c = 0; c < dimension_C; c++)
            {
                int l_max = s-center_s;
                if(l_max<0) l_max = -l_max;
                
                int m = r-center_r;
                if(m<0) m = -m;
                if(l_max<m) l_max = m;
                
                m = c-center_c;
                if(m<0) m = -m;
                if(l_max<m) l_max = m;
                
                unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(c,r,s));
                pixel[0] = l_max;
            }
        }
    }
}


// Creates an image of a sphere of dimater the smallest dimension size.
void CreateTestImage01(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C)
{
    image->SetDimensions(dimension_C, dimension_R, dimension_S);
    image->AllocateScalars(VTK_UNSIGNED_CHAR,1);
    
    int center_s = dimension_S/2;
    int center_r = dimension_R/2;
    int center_c = dimension_C/2;
    
    
    int radius = dimension_S/2;
    if(dimension_R/2 < radius) radius = dimension_R/2;
    if(dimension_C/2 < radius) radius = dimension_C/2;
    
    
    
    
    for(unsigned int s = 0; s < dimension_S; s++)
    {
        for(unsigned int r = 0; r < dimension_R; r++)
        {
            for(unsigned int c = 0; c < dimension_C; c++)
            {
                int d = (int) (sqrt( (s-center_s)*(s-center_s) + (r-center_r)*(r-center_r) + (c-center_c)*(c-center_c) ));
                unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(c,r,s));
                
                if(d<radius) pixel[0] = d;
                else pixel[0] = 0;
            }
        }
    }
}


// Creates an image of l_max distances from the center of the image.
void CreateTestImage02(vtkImageData* image, int dimension_S, int dimension_R, int dimension_C)
{
    image->SetDimensions(dimension_C, dimension_R, dimension_S);
    image->AllocateScalars(VTK_UNSIGNED_CHAR,1);
    
    int center_s = dimension_S/2;
    int center_r = dimension_R/2;
    int center_c = dimension_C/2;
    
    int half_size = (dimension_S-1)/2;
    if((dimension_R-1)/2 < half_size) half_size = (dimension_R-1)/2;
    if((dimension_C-1)/2 < half_size) half_size = (dimension_C-1)/2;

    
    for(unsigned int s = 0; s < dimension_S; s++)
    {
        for(unsigned int r = 0; r < dimension_R; r++)
        {
            for(unsigned int c = 0; c < dimension_C; c++)
            {
                int l_max = s-center_s;
                if(l_max<0) l_max = -l_max;
                
                int m = r-center_r;
                if(m<0) m = -m;
                if(l_max<m) l_max = m;
                
                m = c-center_c;
                if(m<0) m = -m;
                if(l_max<m) l_max = m;
                
                unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(c,r,s));
                
                if(l_max<half_size) pixel[0] = l_max;
                else pixel[0] = 0;
            }
        }
    }
}
