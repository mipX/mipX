/*=========================================================================
 
 Program:   mipx
 Module:    vmxImage.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxImage.h"





template<class T>
vmxImageT<T>::vmxImageT()
{
    m_renderer = NULL;
    
    int type_size = sizeof(T);
    int number_of_bits = 8 * type_size;
    this->m_class_name.Assign("vmxImage");
    this->m_class_name.AppendNumber(number_of_bits);
    this->m_class_name.Append("U");
    
    this->SetNumberOfVisualizationComponents(3);
}


template<class T>
vmxImageT<T>::~vmxImageT()
{
    this->vmxImageT<T>::Reset();
}


template<class T>
int vmxImageT<T>::AttachData(void *data_address, int is_data_owned_by_mxImage, unsigned int dimension_c, unsigned int dimension_r, unsigned int dimension_s, unsigned int dimension_t)
{
    if(!(this->vmxImageDataT<T>::AttachData(data_address, is_data_owned_by_mxImage, dimension_c, dimension_r, dimension_s, dimension_t))) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
int vmxImageT<T>::ConvertToDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    if(!this->vmxImageDataT<T>::ConvertToDimensions(t,s,r,c)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
int vmxImageT<T>::CopyFromDataObject(mxDataObject *data_object)
{
    if(!data_object) return 0;
    
    mxImageT<T> *image = dynamic_cast<mxImageT<T>*>(data_object);
    
    if(!image) return 0;
    
    return (this->vmxImageT<T>::Copy(image));
}


template<class T>
int vmxImageT<T>::Copy(vmxImageT<T> *image)
{
    //std::cout<<std::endl<<" vmxImageT<T>::Copy(vmxImageT<T> *image) was called!";
    
    if(!image) return 0;
    
//    this->SetDimensionsAndPropertiesAs(image);
//    for(unsigned int i=0; i<image->GetNumberOfDataElements(); i++)
//    {
//        this->m_grid[i] = image->m_grid[i];
//    }
    
    //this->m_image_planes.SetImage(this);
    
    if(!this->vmxImageDataT<T>::Copy(image)) return 0;
    return (this->m_image_planes.SetImage(this));
//    return 1;
}


template<class T>
int vmxImageT<T>::Copy(vmxImageDataT<T> *image)
{
    //std::cout<<std::endl<<" vmxImageT<T>::Copy(vmxImageDataT<T> *image) was called!";
    if(!this->vmxImageDataT<T>::Copy(image)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
int vmxImageT<T>::Copy(mxImageT<T> *image)
{
    //std::cout<<std::endl<<" vmxImageT<T>::Copy(mxImageT<T> *image) was called!";
    if(!this->vmxImageDataT<T>::Copy(image)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
void* vmxImageT<T>::DetachData()
{
    void *p = this->vmxImageDataT<T>::DetachData();
    this->vmxImageT<T>::Reset();
    return p;
}


template<class T>
unsigned int vmxImageT<T>::GetIndex_C()
{
    return (this->m_image_planes.GetIndexColumns());
}


template<class T>
unsigned int vmxImageT<T>::GetIndex_R()
{
    return (this->m_image_planes.GetIndexRows());
}


template<class T>
unsigned int vmxImageT<T>::GetIndex_S()
{
    return (this->m_image_planes.GetIndexSlices());
}


template<class T>
unsigned int vmxImageT<T>::GetIndex_T()
{
    return (this->m_image_planes.GetIndexTime());
}


template<class T>
int vmxImageT<T>::GetVisibilityOfComponent(int component_index)
{
    if(component_index==0)
    {
        return this->GetImagePlanes()->GetVisibilityOf_S_Plane();
    }
    if(component_index==1)
    {
        return this->GetImagePlanes()->GetVisibilityOf_R_Plane();
    }
    if(component_index==2)
    {
        return this->GetImagePlanes()->GetVisibilityOf_C_Plane();
    }
    return 0;
}


template<class T>
int vmxImageT<T>::LoadColorPNGImage(const char *file_name)
{
    // Read the image
    vtkSmartPointer<vtkPNGReader> reader = vtkSmartPointer<vtkPNGReader>::New();
    reader->SetFileName(file_name);
    reader->Update();
    
    // Extract R, G, B components
    vtkSmartPointer<vtkImageExtractComponents> extract_red_filter = vtkSmartPointer<vtkImageExtractComponents>::New();
    extract_red_filter->SetInputConnection(reader->GetOutputPort());
    extract_red_filter->SetComponents(0);
    extract_red_filter->Update();
    
    vtkSmartPointer<vtkImageExtractComponents> extract_green_filter = vtkSmartPointer<vtkImageExtractComponents>::New();
    extract_green_filter->SetInputConnection(reader->GetOutputPort());
    extract_green_filter->SetComponents(1);
    extract_green_filter->Update();
    
    vtkSmartPointer<vtkImageExtractComponents> extract_blue_filter = vtkSmartPointer<vtkImageExtractComponents>::New();
    extract_blue_filter->SetInputConnection(reader->GetOutputPort());
    extract_blue_filter->SetComponents(2);
    extract_blue_filter->Update();
    
    vtkSmartPointer<vtkImageData> red_image = vtkSmartPointer<vtkImageData>::New();
    red_image->DeepCopy(extract_red_filter->GetOutput());
    
    vtkSmartPointer<vtkImageData> green_image = vtkSmartPointer<vtkImageData>::New();
    green_image->DeepCopy(extract_green_filter->GetOutput());
    
    vtkSmartPointer<vtkImageData> blue_image = vtkSmartPointer<vtkImageData>::New();
    blue_image->DeepCopy(extract_blue_filter->GetOutput());
    
    // Set image dimensions.
    this->SetDimensions(3, 1, red_image->GetDimensions()[1], red_image->GetDimensions()[0]);
    
    // Depending on the scalar size we will have to address the pixels in a different way (unsigned char or unsigned short)
    // vtkImageData::GetScalarSize returns the number of Bytes of the scalar.
    
    // If image has unsigned char elements...
    if(red_image->GetScalarSize()==1)
    {
        /// ...iterate over all pixels and copy to output image.
        for(unsigned int r=0; r<this->GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<this->GetDimension_C(); c++)
            {
                unsigned char *pixel_r = static_cast<unsigned char*>(red_image->GetScalarPointer(c,r,0));
                unsigned char *pixel_g = static_cast<unsigned char*>(green_image->GetScalarPointer(c,r,0));
                unsigned char *pixel_b = static_cast<unsigned char*>(blue_image->GetScalarPointer(c,r,0));
                
                this->operator()(0,0,r,c) = pixel_r[0];
                this->operator()(1,0,r,c) = pixel_g[0];
                this->operator()(2,0,r,c) = pixel_b[0];
            }
        }
    }
    
    // If image has unsigned short elements...
    if(red_image->GetScalarSize()==2)
    {
        /// ...iterate over all pixels and copy to output image.
        for(unsigned int r=0; r<this->GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<this->GetDimension_C(); c++)
            {
                unsigned short *pixel_r = static_cast<unsigned short*>(red_image->GetScalarPointer(c,r,0));
                unsigned short *pixel_g = static_cast<unsigned short*>(green_image->GetScalarPointer(c,r,0));
                unsigned short *pixel_b = static_cast<unsigned short*>(blue_image->GetScalarPointer(c,r,0));
                
                this->operator()(0,0,r,c) = pixel_r[0];
                this->operator()(1,0,r,c) = pixel_g[0];
                this->operator()(2,0,r,c) = pixel_b[0];
            }
        }
    }
    
    return 1;
}



template<class T>
int vmxImageT<T>::LoadPNGImages(vtkStringArray *file_names)
{
    if(!vmxImageDataT<T>::LoadPNGImages(file_names)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
int vmxImageT<T>::LoadPNGImages(const char *file_name_root, unsigned int number_of_digits, unsigned int number_of_slices)
{
    if(!vmxImageDataT<T>::LoadPNGImages(file_name_root,number_of_digits,number_of_slices)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
int vmxImageT<T>::LoadPNGImages(const char *file_name_root, unsigned int number_of_digits, unsigned int slice_start_index, unsigned int slice_end_index)
{
    if(!vmxImageDataT<T>::LoadPNGImages(file_name_root,number_of_digits,slice_start_index,slice_end_index)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
int vmxImageT<T>::LoadVTKFile(const char *file_name)
{
    if(!vmxImageDataT<T>::LoadVTKFile(file_name)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
void vmxImageT<T>::Reset()
{
    this->vmxImageDataT<T>::Reset();
    this->vmxImageT<T>::SetVisibility(0);
    
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! RESET IN IMAGE PLANES NOT DONE YET< BUT IT SHOULD BE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//    this->m_image_planes.Reset();
}


template<class T>
void vmxImageT<T>::SetIndex_S(unsigned int index)
{
    //return (this->m_image_planes.SetIndex_S(index));
    this->m_image_planes.SetIndex_S(index);
}


template<class T>
void vmxImageT<T>::SetIndex_T(unsigned int index)
{
    //return (this->m_image_planes.SetIndex_T(index));
    this->m_image_planes.SetIndex_T(index);
}


template<class T>
void vmxImageT<T>::SetInteractor(vtkRenderWindowInteractor *rwi)
{
    this->m_image_planes.SetInteractor(rwi);
}


template<class T>
int vmxImageT<T>::SetDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    if(!vmxImageDataT<T>::SetDimensions(t,s,r,c)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
void vmxImageT<T>::SetDimensionsAndPropertiesAs(mxImageT<T> *image)
{
    this->vmxImageDataT<T>::SetDimensionsAndPropertiesAs(image);
    this->m_image_planes.SetImage(this);
}


template<class T>
void vmxImageT<T>::SetDimensionsAs(mxImageT<T> *image)
{
    this->vmxImageT<T>::SetDimensions(image->GetDimension_T(),image->GetDimension_S(),image->GetDimension_R(),image->GetDimension_C());
}


template<class T>
void vmxImageT<T>::SetMappingToOpaqueColor()
{
    this->m_image_planes.SetMappingToOpaqueColor();
}


template<class T>
void vmxImageT<T>::SetMappingToOpaqueGrayScale()
{
    this->m_image_planes.SetMappingToOpaqueGrayScale();
}


template<class T>
void vmxImageT<T>::SetMappingToTransparentBinary()
{
    this->m_image_planes.SetMappingToTransparentBinary();
}


template<class T>
void vmxImageT<T>::SetMappingToTransparentColor()
{
    this->m_image_planes.SetMappingToTransparentColor();
}


template<class T>
void vmxImageT<T>::SetMappingToTransparentGrayScale()
{
    this->m_image_planes.SetMappingToTransparentGrayScale();
}


template<class T>
void vmxImageT<T>::SetOrigin(double t, double s, double r, double c)
{
    this->vmxImageDataT<T>::SetOrigin(t,s,r,c);
    this->m_image_planes.SetImage(this);
}

template<class T>
void vmxImageT<T>::SetPropertiesAs(mxImageT<T> *img)
{
    this->vmxImageDataT<T>::SetOrigin(img->GetOrigin_T(),img->GetOrigin_S(),img->GetOrigin_R(),img->GetOrigin_C());
    this->vmxImageDataT<T>::SetSpacing(img->GetSpacing_T(),img->GetSpacing_S(),img->GetSpacing_R(),img->GetSpacing_C());
    this->m_image_planes.SetImage(this);
}


template<class T>
void vmxImageT<T>::SetRenderer(vtkRenderer *renderer)
{
    if(!renderer) return;
    
    m_renderer = renderer;
    
    this->GetImagePlanes()->GetPlaneWidget_S()->SetCurrentRenderer(m_renderer);
    this->GetImagePlanes()->GetPlaneWidget_R()->SetCurrentRenderer(m_renderer);
    this->GetImagePlanes()->GetPlaneWidget_C()->SetCurrentRenderer(m_renderer);
}


template<class T>
void vmxImageT<T>::SetSpacing(double t, double s, double r, double c)
{
    this->vmxImageDataT<T>::SetSpacing(t,s,r,c);
    this->m_image_planes.SetImage(this);
}


template<class T>
void vmxImageT<T>::SetVisibility(int is_visible)
{
    this->m_image_planes.SetVisibilityOf_S_Plane(is_visible);
    if(m_renderer)
    {
        this->GetImagePlanes()->GetPlaneWidget_S()->SetCurrentRenderer(m_renderer);
    }
}


template<class T>
void vmxImageT<T>::SetVisibilityOfComponent(int component_index, int is_visible)
{
    if(component_index==0)
    {
        this->m_image_planes.SetVisibilityOf_S_Plane(is_visible);
        if(m_renderer) this->GetImagePlanes()->GetPlaneWidget_S()->SetCurrentRenderer(m_renderer);
        return;
    }
    if(component_index==1)
    {
        this->m_image_planes.SetVisibilityOf_R_Plane(is_visible);
        if(m_renderer) this->GetImagePlanes()->GetPlaneWidget_R()->SetCurrentRenderer(m_renderer);
        return;
    }
    if(component_index==2)
    {
        this->m_image_planes.SetVisibilityOf_C_Plane(is_visible);
        if(m_renderer) this->GetImagePlanes()->GetPlaneWidget_C()->SetCurrentRenderer(m_renderer);
        return;
    }
}


template<class T>
int vmxImageT<T>::TakeOverVTKImageData(vtkImageData *vtk_image, unsigned int number_of_time_series, unsigned int number_of_slices)
{
    if(!this->vmxImageDataT<T>::TakeOverVTKImageData(vtk_image,number_of_time_series,number_of_slices)) return 0;
    return (this->m_image_planes.SetImage(this));
}



template class vmxImage_API vmxImageT<uint8_t>;
template class vmxImage_API vmxImageT<uint16_t>;
template class vmxImage_API vmxImageT<uint32_t>;
//template class vmxImage_API vmxImageT<uint64_t>;


template class vmxImage_API mxDataObjectFactoryT< vmxImageT<uint16_t> >;



