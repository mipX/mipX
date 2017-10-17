/*=========================================================================
 
 Program:   mipx
 Module:    vmxImagePlanes.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxImagePlanes.h"



template<class T>
vmxImagePlanesT<T>::vmxImagePlanesT()
{
    this->m_image = NULL;
    
    /// Indexes of planes currently being visualized in order c,r,s,t.
    this->m_index[0] = this->m_index[1] = this->m_index[2] = this->m_index[3] = 0;
    
    this->m_plane_S = vtkSmartPointer<vmxImagePlaneWidget>::New();//this->m_plane_S = vtkSmartPointer<vtkImagePlaneWidget>::New();
    this->m_plane_R = vtkSmartPointer<vmxImagePlaneWidget>::New();
    this->m_plane_C = vtkSmartPointer<vmxImagePlaneWidget>::New();
    
    this->m_lookup_table = vtkSmartPointer<vtkLookupTable>::New();
    
    double total_gray_value_range = 2^(sizeof(T) * 8) - 1;
    
    this->BuildLookupTable(0,total_gray_value_range, 0,1, 1,1, 0,0, 0,0.66);
    
    this->m_interactor = NULL;
}


template<class T>
vmxImagePlanesT<T>::~vmxImagePlanesT()
{
    //cout<<endl<<"vmxImagePlanesT<T>::~vmxImagePlanesT()"<<endl;
    if(this->m_interactor)
    {
        this->m_plane_S->EnabledOff();
        this->m_plane_S->SetInteractor(NULL);
        this->m_plane_R->EnabledOff();
        this->m_plane_R->SetInteractor(NULL);
        this->m_plane_C->EnabledOff();
        this->m_plane_C->SetInteractor(NULL);

        this->m_interactor = NULL;
    }
}


template<class T>
void vmxImagePlanesT<T>::BuildLookupTable(double range0, double range1, double brightness0, double brightness1, double aplha0, double aplha1, double saturation0, double saturation1, double hue0, double hue1)
{
    this->m_lookup_table->SetRange(range0,range1);
    this->m_lookup_table->SetValueRange(brightness0,brightness1);
    this->m_lookup_table->SetAlphaRange(aplha0,aplha1);
    this->m_lookup_table->SetSaturationRange(saturation0,saturation1);
    this->m_lookup_table->SetHueRange(hue0,1);
    this->m_lookup_table->SetRampToLinear();
    this->m_lookup_table->Build();
}


template<class T>
unsigned int vmxImagePlanesT<T>::GetIndexColumns()
{
    return (this->m_index[0]);
}


template<class T>
unsigned int vmxImagePlanesT<T>::GetIndexRows()
{
    return (this->m_index[1]);
}


template<class T>
unsigned int vmxImagePlanesT<T>::GetIndexSlices()
{
    return (this->m_index[2]);
}


template<class T>
unsigned int vmxImagePlanesT<T>::GetIndexTime()
{
    return (this->m_index[3]);
}


template<class T>
vtkLookupTable* vmxImagePlanesT<T>::GetLookupTable()
{
    return (this->m_lookup_table);
}


template<class T>
vmxImagePlaneWidget* vmxImagePlanesT<T>::GetPlaneWidget_S()
{
    return (this->m_plane_S);
}


template<class T>
vmxImagePlaneWidget* vmxImagePlanesT<T>::GetPlaneWidget_R()
{
    return (this->m_plane_R);
}


template<class T>
vmxImagePlaneWidget* vmxImagePlanesT<T>::GetPlaneWidget_C()
{
    return (this->m_plane_C);
}


template<class T>
int vmxImagePlanesT<T>::SetImage(vmxImageDataT<T> *input_image)
{
    if(!input_image) return 0;
    if(input_image->IsEmpty()) return 0;
    
    m_image = input_image;
    
    this->m_index[0] = input_image->GetDimension_C() / 2;
    this->m_index[1] = input_image->GetDimension_R() / 2;
    this->m_index[2] = input_image->GetDimension_S() / 2;
    this->m_index[3] = 0;
    
    
//    //We will make a grayscale opaque table as a default table
//    unsigned int min, max;
//    this->m_image->GetVoxelValueFullRange(&min, &max);
//    //this->m_image->GetVoxelMinimumAndMaximumValue(&min, &max);
//    m_lookup_table->SetRange(min, max);
//    //m_lookup_table->SetAlphaRange(1,1);//set the transparency to opaque
//    //m_lookup_table->SetValueRange(0,1); // from black to white
//    //m_lookup_table->SetSaturationRange(0.0, 0.0); // no color saturation
//    //m_lookup_table->SetHueRange(0, 0.66666);
//    //m_lookup_table->SetRampToLinear();
//    m_lookup_table->Build();
    
    
    m_plane_S->SetInputData(input_image->GetVTKImageData(this->GetIndexTime()));
    m_plane_S->DisplayTextOn();
    m_plane_S->SetPlaneOrientationToZAxes();
    m_plane_S->SetSliceIndex(this->GetIndexSlices());
    //m_plane_S->TextureInterpolateOff();
    m_plane_S->SetTextureInterpolate(0);
    m_plane_S->SetResliceInterpolateToNearestNeighbour();
    m_plane_S->SetLookupTable(this->m_lookup_table);
    //m_plane_S->SetInteractor(this->m_interactor);
    //m_plane_S->EnabledOn();

    m_plane_R->SetInputData(input_image->GetVTKImageData(this->GetIndexTime()));
    m_plane_R->DisplayTextOn();
    m_plane_R->SetPlaneOrientationToYAxes();
    m_plane_R->SetSliceIndex(this->GetIndexRows());
    //m_plane_R->TextureInterpolateOff();
    m_plane_R->SetTextureInterpolate(0);
    m_plane_R->SetResliceInterpolateToNearestNeighbour();
    m_plane_R->SetLookupTable(this->m_lookup_table);
    //m_plane_R->SetInteractor(this->m_interactor);
    //m_plane_R->EnabledOn();

    m_plane_C->SetInputData(input_image->GetVTKImageData(this->GetIndexTime()));
    m_plane_C->DisplayTextOn();
    m_plane_C->SetPlaneOrientationToXAxes();
    m_plane_C->SetSliceIndex(this->GetIndexColumns());
    //m_plane_C->TextureInterpolateOff();
    m_plane_C->SetTextureInterpolate(0);
    m_plane_C->SetResliceInterpolateToNearestNeighbour();
    m_plane_C->SetLookupTable(this->m_lookup_table);
    //m_plane_C->SetInteractor(this->m_interactor);
    //m_plane_C->EnabledOn();


    return 1;
}



template<class T>
int vmxImagePlanesT<T>::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return 0;
    
    this->m_interactor = interactor;
    this->m_plane_S->SetInteractor(this->m_interactor);
    this->m_plane_R->SetInteractor(this->m_interactor);
    this->m_plane_C->SetInteractor(this->m_interactor);
    
    return 1;
}


template<class T>
int vmxImagePlanesT<T>::SetIndex_C(unsigned int index)
{
    if(index>=this->m_image->GetDimension_C()) return 0;
    this->m_index[0] = index;
    this->m_plane_C->SetSliceIndex(index);
    return 1;
}



template<class T>
int vmxImagePlanesT<T>::SetIndex_R(unsigned int index)
{
    if(index>=this->m_image->GetDimension_R()) return 0;
    this->m_index[1] = index;
    this->m_plane_R->SetSliceIndex(index);
    return 1;
}



template<class T>
int vmxImagePlanesT<T>::SetIndex_S(unsigned int index)
{
    if(index>=this->m_image->GetDimension_S()) return 0;
    this->m_index[2] = index;
    this->m_plane_S->SetSliceIndex(index);
    return 1;
}


template<class T>
int vmxImagePlanesT<T>::SetIndex_T(unsigned int index)
{
    if(index>=this->m_image->GetDimension_T()) return 0;
    this->m_index[3] = index;
    
    this->m_plane_S->SetUserControlledLookupTable(1);
    this->m_plane_S->SetInputData(this->m_image->GetVTKImageData(index));
    this->m_plane_S->SetUserControlledLookupTable(0);
    this->m_plane_S->SetSliceIndex(this->GetIndexSlices());
    
    this->m_plane_R->SetUserControlledLookupTable(1);
    this->m_plane_R->SetInputData(this->m_image->GetVTKImageData(index));
    this->m_plane_R->SetUserControlledLookupTable(0);
    this->m_plane_R->SetSliceIndex(this->GetIndexRows());
    
    this->m_plane_C->SetUserControlledLookupTable(1);
    this->m_plane_C->SetInputData(this->m_image->GetVTKImageData(index));
    this->m_plane_C->SetUserControlledLookupTable(0);
    this->m_plane_C->SetSliceIndex(this->GetIndexColumns());
    
    return 1;
}


template<class T>
void vmxImagePlanesT<T>::SetMappingToOpaqueColor()
{
    uint64_t min, max;
    min = 0;
    max = 2^(sizeof(T)) - 1;
    if(m_image)
    {
        if(!m_image->IsEmpty())
        {
            this->m_image->GetVoxelMinAndMaxValues(min, max);
        }
    }
    this->BuildLookupTable(min,max, 1,1, 1,1, 1,1, 0,0.66);
    //this->m_plane_S->SetLookupTable(this->m_lookup_table);
    if(this->m_interactor) this->m_interactor->Render();
//    this->m_mapping = OPAQUE_COLOR;
}


template<class T>
void vmxImagePlanesT<T>::SetMappingToTransparentColor()
{
    uint64_t min, max;
    min = 0;
    max = 2^(sizeof(T)) - 1;
    if(m_image)
    {
        if(!m_image->IsEmpty())
        {
            this->m_image->GetVoxelMinAndMaxValues(min, max);
        }
    }

    this->BuildLookupTable(min,max, 1,1, 0,1, 1,1, 0,0.66);
    if(this->m_interactor) this->m_interactor->Render();
//    this->m_mapping = TRANSPARENT_COLOR;
}


template<class T>
void vmxImagePlanesT<T>::SetMappingToOpaqueGrayScale()
{
    uint64_t min, max;
    min = 0;
    max = 2^(sizeof(T)) - 1;
    if(m_image)
    {
        if(!m_image->IsEmpty())
        {
            this->m_image->GetVoxelMinAndMaxValues(min, max);
        }
    }
    this->BuildLookupTable(min,max, 0,1, 1,1, 0,0, 0,0.66);
    if(this->m_interactor) this->m_interactor->Render();
//    this->m_mapping = OPAQUE_GRAY;
}


template<class T>
void vmxImagePlanesT<T>::SetMappingToTransparentGrayScale()
{
    uint64_t min, max;
    min = 0;
    max = 2^(sizeof(T)) - 1;
    if(m_image)
    {
        if(!m_image->IsEmpty())
        {
            this->m_image->GetVoxelMinAndMaxValues(min, max);
        }
    }

    this->BuildLookupTable(min,max, 0,1, 0,1, 0,0, 0,0.66);
    if(this->m_interactor) this->m_interactor->Render();
//    this->m_mapping = TRANSPARENT_GRAY;
}


template<class T>
void vmxImagePlanesT<T>::SetMappingToTransparentBinary()
{
    this->BuildLookupTable(0,1, 1,1, 0,1, 1,1, 0,0.66);
    this->m_plane_S->SetLookupTable(this->m_lookup_table);
    if(this->m_interactor) this->m_interactor->Render();
//    this->m_mapping = TRANSPARENT_BINARY;
}


template<class T>
void vmxImagePlanesT<T>::SetVisibilityOf_S_Plane(int is_visible)
{
    if(is_visible)
    {
        if(this->m_interactor) this->m_plane_S->EnabledOn();
        return;
    }
    else
    {
        if(this->m_interactor) this->m_plane_S->EnabledOff();
    }
}


template<class T>
void vmxImagePlanesT<T>::SetVisibilityOf_R_Plane(int is_visible)
{
    if(is_visible)
    {
        if(this->m_interactor) this->m_plane_R->EnabledOn();
        return;
    }
    else
    {
        if(this->m_interactor) this->m_plane_R->EnabledOff();
    }
}


template<class T>
void vmxImagePlanesT<T>::SetVisibilityOf_C_Plane(int is_visible)
{
    if(is_visible)
    {
        if(this->m_interactor) this->m_plane_C->EnabledOn();
        return;
    }
    else
    {
        if(this->m_interactor) this->m_plane_C->EnabledOff();
    }
}



template class vmxImagePlanes_API vmxImagePlanesT<uint8_t>;
template class vmxImagePlanes_API vmxImagePlanesT<uint16_t>;
template class vmxImagePlanes_API vmxImagePlanesT<uint32_t>;
//template class vmxImagePlanes_API vmxImagePlanesT<uint64_t>;




