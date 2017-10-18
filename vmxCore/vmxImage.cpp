/*=========================================================================
 
 Program:   mipx
 Module:    vmxImage.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
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
    if(!this->vmxImageDataT<T>::Copy(image)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
int vmxImageT<T>::Copy(vmxImageDataT<T> *image)
{
    if(!this->vmxImageDataT<T>::Copy(image)) return 0;
    return (this->m_image_planes.SetImage(this));
}


template<class T>
int vmxImageT<T>::Copy(mxImageT<T> *image)
{
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
int vmxImageT<T>::SetIndex_S(unsigned int index)
{
    return (this->m_image_planes.SetIndex_S(index));
}


template<class T>
int vmxImageT<T>::SetIndex_T(unsigned int index)
{
    return (this->m_image_planes.SetIndex_T(index));
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
void vmxImageT<T>::SetDimensionsAndPropertiesAs(mxBasicImage *image)
{
    this->vmxImageDataT<T>::SetDimensionsAndPropertiesAs(image);
    this->m_image_planes.SetImage(this);
}


template<class T>
void vmxImageT<T>::SetDimensionsAs(mxBasicImage *image)
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
void vmxImageT<T>::SetPropertiesAs(mxBasicImage *img)
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



