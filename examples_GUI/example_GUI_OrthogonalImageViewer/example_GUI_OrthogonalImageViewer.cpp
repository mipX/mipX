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
    

    
//    static int Slot_vmxGUIMenu_LoadFiles2(vmxGUIConnection *connection)
//    {
//        vmxGUIFilesDialog *fd = static_cast<vmxGUIFilesDialog *>(connection->GetReceiverObject());
//
//        cout<<"MainApp::Slot_vmxGUIMenu_LoadFiles(): "<<fd->GetClassName().Get_C_String()<<endl;
//
//        fd->OpenFiles(Slot_vmxGUIFilesDialog_LoadFiles);
//
////        vmxGUIFilesDialog::OpenFiles(Slot_vmxGUIFilesDialog_LoadFiles);
//
//        return 1;
//    };
    
 
    MainApp()
    {
        // Initialization.
        {
            vmxGUIConnection::internal_SetMainWidget(this);
        }
        
        
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
//        vtkSmartPointer<vtkGenericDataObjectReader> reader = vtkSmartPointer<vtkGenericDataObjectReader>::New();
//        reader->SetFileName("/Users/danilobabin/Dropbox/-DIP_IMAGES/-AORTA/From_Laura_and_Dan/PWV_data_converted/MFS002/00_PC.vtk");
//        reader->Update();
//

        vtkSmartPointer<vtkGenericDataObjectReader> reader2 = vtkSmartPointer<vtkGenericDataObjectReader>::New();
        //reader2->SetFileName("/Users/danilobabin/Dropbox/-DIP_IMAGES/-AORTA/From_Laura_and_Dan/PWV_data_converted/MFS002/00_cine.vtk");
        reader2->SetFileName("/Users/danilobabin/Dropbox/-DIP_IMAGES/-AORTA/Wavelocity/InterScanner_UZGent/Volunteer02_M32Y/data/trueFisp.vtk");
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
        
        

        
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[0], ImagePickedEvent, m_array_of_image_renderers[1], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[0]->GetPickedIndexes());
            //c->SetPassedDataDouble(m_array_of_image_renderers[0]->GetPickedWorldPosition());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[0], ImagePickedEvent, m_array_of_image_renderers[2], m_array_of_image_renderers[1],vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[0]->GetPickedIndexes());
            //c->SetPassedDataDouble(m_array_of_image_renderers[0]->GetPickedWorldPosition());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[1], ImagePickedEvent, m_array_of_image_renderers[0], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[1]->GetPickedIndexes());
            //c->SetPassedDataDouble(m_array_of_image_renderers[1]->GetPickedWorldPosition());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[1], ImagePickedEvent, m_array_of_image_renderers[2], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[1]->GetPickedIndexes());
            //c->SetPassedDataDouble(m_array_of_image_renderers[1]->GetPickedWorldPosition());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[2], ImagePickedEvent, m_array_of_image_renderers[0], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[2]->GetPickedIndexes());
            //c->SetPassedDataDouble(m_array_of_image_renderers[2]->GetPickedWorldPosition());
        }
        {
            vmxGUIConnection *c = vmxGUIConnection::New(m_array_of_image_renderers[2], ImagePickedEvent, m_array_of_image_renderers[1], m_array_of_image_renderers[1], vmxGUIRendererImageViewer::Slot_Pick);
            c->SetPassedDataInt(m_array_of_image_renderers[2]->GetPickedIndexes());
            //c->SetPassedDataDouble(m_array_of_image_renderers[2]->GetPickedWorldPosition());
        }
        
        
        
        m_array_of_image_renderers[0]->FitImageToScreen();
        m_array_of_image_renderers[1]->FitImageToScreen();
        m_array_of_image_renderers[2]->FitImageToScreen();
        

        

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
