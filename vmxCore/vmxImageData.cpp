/*=========================================================================
 
 Program:   mipx
 Module:    vmxImageData.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxImageData.h"



template<class T>
vmxImageDataT<T>::vmxImageDataT()
{
    int type_size = sizeof(T);
    int number_of_bits = 8 * type_size;
    this->m_class_name.Assign("vmxImageData");
    this->m_class_name.AppendNumber(number_of_bits);
    this->m_class_name.Append("U");

	this->m_vtk_image_data = vtkSmartPointer<vtkImageData>::New();
}


template<class T>
vmxImageDataT<T>::~vmxImageDataT()
{
    this->vmxImageDataT<T>::Reset();
}


template<class T>
int vmxImageDataT<T>::internalAttachToPreset_mxImage()
{
    int type_size = sizeof(T);
    
    this->m_vtk_image_data_array.SetNumberOfElements(mxImageT<T>::GetDimension_T());
    int src = mxImageT<T>::GetDimension_S()*mxImageT<T>::GetDimension_R()*mxImageT<T>::GetDimension_C();
    int tsrc = src * mxImageT<T>::GetDimension_T();

    this->m_vtk_image_data->SetDimensions(mxImageT<T>::GetDimension_C(), mxImageT<T>::GetDimension_R(), mxImageT<T>::GetDimension_S()*mxImageT<T>::GetDimension_T());
    this->m_vtk_image_data->SetExtent(0, mxImageT<T>::GetDimension_C()-1, 0, mxImageT<T>::GetDimension_R()-1, 0, mxImageT<T>::GetDimension_S()*mxImageT<T>::GetDimension_T()-1);
    this->m_vtk_image_data->SetOrigin(mxImageT<T>::GetOrigin_C(),mxImageT<T>::GetOrigin_R(),mxImageT<T>::GetOrigin_S());
    this->m_vtk_image_data->SetSpacing(mxImageT<T>::GetSpacing_C(),mxImageT<T>::GetSpacing_R(),mxImageT<T>::GetSpacing_S());

    
    switch(type_size)
    {
        case 1:
        {
            uint8_t *data_address = (uint8_t*)(this->GetData());
            this->m_vtk_image_data->SetPointDataActiveScalarInfo(this->m_vtk_image_data->GetInformation(),VTK_TYPE_UINT8,1);
            vtkSmartPointer<vtkTypeUInt8Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt8Array>::New();
            vtk_data_array->SetArray(data_address, tsrc, 1);//The last argument value '1' indicates that VTK must not free array memory.
            m_vtk_image_data->GetPointData()->SetScalars(vtk_data_array);
        }
            break;
        case 2:
        {
            uint16_t *data_address = (uint16_t*)(this->GetData());
            this->m_vtk_image_data->SetPointDataActiveScalarInfo(this->m_vtk_image_data->GetInformation(),VTK_TYPE_UINT16,1);
            vtkSmartPointer<vtkTypeUInt16Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt16Array>::New();
            vtk_data_array->SetArray(data_address, tsrc, 1);//The last argument value '1' indicates that VTK must not free array memory.
            m_vtk_image_data->GetPointData()->SetScalars(vtk_data_array);
        }
            break;
        case 4:
        {
            uint32_t *data_address = (uint32_t*)(this->GetData());
            this->m_vtk_image_data->SetPointDataActiveScalarInfo(this->m_vtk_image_data->GetInformation(),VTK_TYPE_UINT32,1);
            vtkSmartPointer<vtkTypeUInt32Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt32Array>::New();
            vtk_data_array->SetArray(data_address, tsrc, 1);//The last argument value '1' indicates that VTK must not free array memory.
            m_vtk_image_data->GetPointData()->SetScalars(vtk_data_array);
        }
            break;
//        case 8:
//        {
//            uint64_t *data_address = (uint64_t*)(this->GetData()); // this line does not wotk with vtkUnsignedLongArray.
//            this->m_vtk_image_data->SetPointDataActiveScalarInfo(this->m_vtk_image_data->GetInformation(),VTK_TYPE_UINT64,1);
//            vtkSmartPointer<vtkTypeUInt64Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt64Array>::New();
//            vtk_data_array->SetArray(data_address, tsrc, 1);//The last argument value '1' indicates that VTK must not free array memory.
//            m_vtk_image_data->GetPointData()->SetScalars(vtk_data_array);
//        }
//            break;
            
            cout<<"vmxImageDataT<T>::SetDimensions() error: the type of size size(T)="<<type_size<<" is not supported!"<<endl;
            return 0;
    }
    
    
    for(unsigned int i=0; i<this->m_vtk_image_data_array.GetNumberOfElements(); i++)
    {
        this->m_vtk_image_data_array[i] = vtkSmartPointer<vtkImageData>::New();
        this->m_vtk_image_data_array[i]->SetDimensions(mxImageT<T>::GetDimension_C(), mxImageT<T>::GetDimension_R(), mxImageT<T>::GetDimension_S());
        this->m_vtk_image_data_array[i]->SetExtent(0, mxImageT<T>::GetDimension_C()-1, 0, mxImageT<T>::GetDimension_R()-1, 0, mxImageT<T>::GetDimension_S()-1);
        this->m_vtk_image_data_array[i]->SetOrigin(mxImageT<T>::GetOrigin_C(),mxImageT<T>::GetOrigin_R(),mxImageT<T>::GetOrigin_S());
        this->m_vtk_image_data_array[i]->SetSpacing(mxImageT<T>::GetSpacing_C(),mxImageT<T>::GetSpacing_R(),mxImageT<T>::GetSpacing_S());
        
        switch(type_size)
        {
            case 1:
            {
                uint8_t *data_address = (uint8_t*)(this->GetData());
                this->m_vtk_image_data_array[i]->SetPointDataActiveScalarInfo(this->m_vtk_image_data_array[i]->GetInformation(),VTK_TYPE_UINT8,1);
                vtkSmartPointer<vtkTypeUInt8Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt8Array>::New();
                vtk_data_array->SetArray(&(data_address[i*src]), src, 1);//The last argument value '1' indicates that VTK must not free array memory.
                m_vtk_image_data_array[i]->GetPointData()->SetScalars(vtk_data_array);
            }
                break;
            case 2:
            {
                uint16_t *data_address = (uint16_t*)(this->GetData());
                this->m_vtk_image_data_array[i]->SetPointDataActiveScalarInfo(this->m_vtk_image_data_array[i]->GetInformation(),VTK_TYPE_UINT16,1);
                vtkSmartPointer<vtkTypeUInt16Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt16Array>::New();
                vtk_data_array->SetArray(&(data_address[i*src]), src, 1);//The last argument value '1' indicates that VTK must not free array memory.
                m_vtk_image_data_array[i]->GetPointData()->SetScalars(vtk_data_array);
            }
                break;
            case 4:
            {
                uint32_t *data_address = (uint32_t*)(this->GetData());
                this->m_vtk_image_data_array[i]->SetPointDataActiveScalarInfo(this->m_vtk_image_data_array[i]->GetInformation(),VTK_TYPE_UINT32,1);
                vtkSmartPointer<vtkTypeUInt32Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt32Array>::New();
                vtk_data_array->SetArray(&(data_address[i*src]), src, 1);//The last argument value '1' indicates that VTK must not free array memory.
                m_vtk_image_data_array[i]->GetPointData()->SetScalars(vtk_data_array);
            }
                break;
//            case 8:
//            {
//                uint64_t *data_address = (uint64_t*)(this->GetData()); // this line does not wotk with vtkUnsignedLongArray.
//                this->m_vtk_image_data_array[i]->SetPointDataActiveScalarInfo(this->m_vtk_image_data_array[i]->GetInformation(),VTK_TYPE_UINT64,1);
//                vtkSmartPointer<vtkTypeUInt64Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt64Array>::New();
//                vtk_data_array->SetArray(&(data_address[i*src]), src, 1);//The last argument value '1' indicates that VTK must not free array memory.
//                m_vtk_image_data_array[i]->GetPointData()->SetScalars(vtk_data_array);
//            }
//                break;
                
                cout<<"vmxImageDataT<T>::SetDimensions() error: the type of size size(T)="<<type_size<<" is not supported!"<<endl;
                return 0;
        }
        
    }
    
    return 1;
}


template<class T>
int vmxImageDataT<T>::AttachData(void *data_address, int is_data_owned_by_mxImage, unsigned int dimension_c, unsigned int dimension_r, unsigned int dimension_s, unsigned int dimension_t)
{
    if(!(this->mxImageT<T>::AttachData(data_address, is_data_owned_by_mxImage, dimension_c, dimension_r, dimension_s, dimension_t))) return 0;
    
    return (vmxImageDataT<T>::internalAttachToPreset_mxImage());

    return 0;
}


template<class T>
int vmxImageDataT<T>::ConvertToDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    if(r==0 || c==0 || s==0 || t==0) return 0;
    
    if(!(mxImageT<T>::ConvertToDimensions(t,s,r,c))) return 0;
    
    return (vmxImageDataT<T>::internalAttachToPreset_mxImage());
}


template<class T>
int vmxImageDataT<T>::CopyFromDataObject(mxDataObject *data_object)
{
    std::cout<<std::endl<<" vmxImageDataT<T>::CopyFromDataObject(mxDataObject *data_object) was called!";
    if(!data_object) return 0;
    
    mxImageT<T> *image = dynamic_cast<mxImageT<T>*>(data_object);
    
    if(!image) return 0;
    
    return (this->vmxImageDataT<T>::Copy(image));
}


template<class T>
int vmxImageDataT<T>::Copy(vmxImageDataT<T> *image)
{
    std::cout<<std::endl<<" vmxImageDataT<T>::Copy(vmxImageDataT<T> *image) was called!";
    if(!this->mxImageT<T>::Copy(image)) return 0;
    this->vmxImageDataT<T>::SetPropertiesAs(image);
    this->internalAttachToPreset_mxImage();
    return 1;
}


template<class T>
int vmxImageDataT<T>::Copy(mxImageT<T> *image)
{
    std::cout<<std::endl<<" vmxImageDataT<T>::Copy(mxImageT<T> *image) was called!";
    if(!this->mxImageT<T>::Copy(image)) return 0;
    this->vmxImageDataT<T>::SetPropertiesAs(image);
    this->internalAttachToPreset_mxImage();
    return 1;
}


template<class T>
void* vmxImageDataT<T>::DetachData()
{
    void *p = this->mxImageT<T>::DetachData();
    this->vmxImageDataT<T>::Reset();
    
    return p;
}


template<class T>
vtkImageData* vmxImageDataT<T>::GetVTKImageData(unsigned int time_index)
{
    if(time_index >= this->GetDimension_T()) return NULL;
    return (this->m_vtk_image_data_array[time_index]);
}


template<class T>
int vmxImageDataT<T>::LoadPNGImages(vtkStringArray *file_names)
{
    this->vmxImageDataT<T>::Reset();
    
    vtkSmartPointer<vtkPNGReader> reader = vtkSmartPointer<vtkPNGReader>::New();
    reader->SetDataScalarTypeToUnsignedChar();
    reader->SetNumberOfScalarComponents(1);
    reader->SetFileNames(file_names);
    reader->Update();
    
    vtkSmartPointer<vtkImageFlip> image_flip = vtkSmartPointer<vtkImageFlip>::New();
    image_flip->SetInputConnection(reader->GetOutputPort());
    image_flip->SetFilteredAxis(0);
    image_flip->Update();
    
    vtkSmartPointer<vtkImageCast> image_cast = vtkSmartPointer<vtkImageCast>::New();
    image_cast->SetInputConnection(image_flip->GetOutputPort());
    int type_size = sizeof(T);
    switch(type_size)
    {
        case 1:
        {
            image_cast->SetOutputScalarTypeToUnsignedChar();
        }
            break;
        case 2:
        {
            image_cast->SetOutputScalarTypeToUnsignedShort();
        }
            break;
        case 4:
        {
            image_cast->SetOutputScalarTypeToUnsignedInt();
        }
            break;
//        case 8:
//        {
//            image_cast->SetOutputScalarTypeToUnsignedLong();
//        }
//            break;
            
            cout<<"vmxImageDataT<T>::LoadPNGImages(vtkStringArray*) error: the type of size size(T)="<<type_size<<" is not supported!"<<endl;
            return 0;
    }
    image_cast->Update();
    
    //	For future: // Set tags array.
    
    if(!this->vmxImageDataT<T>::TakeOverVTKImageData(image_cast->GetOutput(), 1, file_names->GetNumberOfElementComponents())) return 0;
    
    return 1;
}


template<class T>
int vmxImageDataT<T>::LoadPNGImages(const char *file_name_root, unsigned int number_of_digits, unsigned int number_of_slices)
{
    return (this->vmxImageDataT<T>::LoadPNGImages(file_name_root,number_of_digits,0,number_of_slices-1));
}


template<class T>
int vmxImageDataT<T>::LoadPNGImages(const char *file_name_root, unsigned int number_of_digits, unsigned int slice_start_index, unsigned int slice_end_index)
{
    if(slice_end_index<slice_start_index) return 0;
    
    this->vmxImageDataT<T>::Reset();
    
    vtkSmartPointer<vtkStringArray> file_names = vtkSmartPointer<vtkStringArray>::New();
        
    for(int s=slice_start_index; s<=(int)slice_end_index; s++)
    {
        mxString file_name;
        file_name.Assign(file_name_root);
        
        int temp = s;
        for(int d=number_of_digits-1; d>=0; d--)
        {
            //std::cout<<std::endl<<" temp="<<temp<<", d="<<d<<", 10^d="<<(pow (10, d));
            file_name.AppendNumber(temp/ ((int)(pow (10, d))));
            temp = temp % ((int)(pow (10, d)));
        }
        
        file_name.Append(".png");
        
        file_names->InsertNextValue(file_name.Get_C_String());
    }
    
    return (this->vmxImageDataT<T>::LoadPNGImages(file_names));
}


template<class T>
int vmxImageDataT<T>::LoadVTKFile(const char *file_name)
{
    this->vmxImageDataT<T>::Reset();
    
    vtkSmartPointer<vtkGenericDataObjectReader> reader = vtkSmartPointer<vtkGenericDataObjectReader>::New();
    reader->SetFileName(file_name);
    reader->Update();
    
    int t_size = 0, s_size = 0;
    mxString bds;
    if(reader->GetHeader())
    {
        bds.Assign(reader->GetHeader());
        if(!bds.IsEmpty())
        {
            mxString bds2;
            if(bds.ExtractString('T','t',bds2)) { bds2.ToNumber(t_size); }
            if(bds.ExtractString('S','s',bds2)) { bds2.ToNumber(s_size); }
            if(bds.ExtractString('O','o',bds2))
            {
                mxList<double> l;
                bds2.ExtractNumbers(l);
                if(l.GetNumberOfElements()==6)
                {
                    this->SetOrientation(l[0],l[1],l[2],l[3],l[4],l[5]);
                }
            }
        }
    }
    
    vtkSmartPointer<vtkImageShiftScale> ic = vtkSmartPointer<vtkImageShiftScale>::New();
    ic->SetInputData(reader->GetOutput());
    
    int type_size = sizeof(T);
    if(type_size==1)
    {
        ic->SetOutputScalarTypeToUnsignedChar();
    }
    if(type_size==2)
    {
        ic->SetOutputScalarTypeToUnsignedShort();
    }
    if(type_size==4)
    {
        ic->SetOutputScalarTypeToUnsignedInt();
    }
//    if(type_size==8)
//    {
//        ic->SetOutputScalarTypeToUnsignedLong();
//    }
    ic->Update();
    
    if(!this->vmxImageDataT<T>::TakeOverVTKImageData(ic->GetOutput(), t_size, s_size)) return 0;
    
    //	// For future: Set tags array.
    
    return 1;
}


template<class T>
void vmxImageDataT<T>::Reset()
{
    this->mxImageT<T>::Reset();
    
    this->m_vtk_image_data_array.Reset();
    
    if(!this->m_is_data_owned_by_mxImage) this->m_vtk_image_data->ReleaseData();
    
    this->m_vtk_image_data->SetOrigin(0,0,0);
    this->m_vtk_image_data->SetSpacing(1,1,1);
}


template<class T>
void vmxImageDataT<T>::SaveToVTIFile(const char *file_name, unsigned int scalar_size_in_bytes)
{
    vtkSmartPointer<vtkImageCast> image_cast = vtkSmartPointer<vtkImageCast>::New();
    image_cast->SetInputData(this->m_vtk_image_data);
    switch (scalar_size_in_bytes)
    {
        case 1:
            image_cast->SetOutputScalarTypeToUnsignedChar();
            break;
        case 2:
            image_cast->SetOutputScalarTypeToUnsignedShort();
            break;
        case 4:
            image_cast->SetOutputScalarTypeToUnsignedInt();
            break;
            //        case 8:
            //            image_cast->SetOutputScalarTypeToUnsignedLong();
            //            break;
    }
    image_cast->Update();
    
    
//    mxString header_string;
//    header_string.Append("T");
//    header_string.AppendNumber((int)this->GetDimension_T());
//    header_string.Append("t");
//    header_string.Append("S");
//    header_string.AppendNumber((int)this->GetDimension_S());
//    header_string.Append("s");
//    double Cx, Cy, Cz;
//    this->GetOrientation_C_Vector(Cx, Cy, Cz);
//    header_string.Append("O"); header_string.AppendNumber(Cx);
//    header_string.Append(" "); header_string.AppendNumber(Cy);
//    header_string.Append(" "); header_string.AppendNumber(Cz);
//    double Rx, Ry, Rz;
//    this->GetOrientation_R_Vector(Rx, Ry, Rz);
//    header_string.Append(" "); header_string.AppendNumber(Rx);
//    header_string.Append(" "); header_string.AppendNumber(Ry);
//    header_string.Append(" "); header_string.AppendNumber(Rz);
//    header_string.Append("o");
//
//    writer->SetHeader(header_string.Get_C_String());
    
    
    vtkSmartPointer<vtkXMLImageDataWriter> writer = vtkSmartPointer<vtkXMLImageDataWriter>::New();
    
    
    mxString file_string;
    file_string.Assign(file_name);
    if( (file_string[file_string.GetNumberOfCharacters()-1]!='i' && file_string[file_string.GetNumberOfCharacters()-1]!='I') || (file_string[file_string.GetNumberOfCharacters()-2]!='t' && file_string[file_string.GetNumberOfCharacters()-2]!='T') || (file_string[file_string.GetNumberOfCharacters()-3]!='v' && file_string[file_string.GetNumberOfCharacters()-3]!='V') || file_string[file_string.GetNumberOfCharacters()-4]!='.' )
    {
        file_string.Append(".vti");
    }
    
    writer->SetInputConnection(image_cast->GetOutputPort());
    writer->SetFileName(file_string.Get_C_String());
    //writer->SetFileTypeToBinary();
    writer->Update();
    writer->Write();
}


template<class T>
void vmxImageDataT<T>::SaveToVTKFile(const char *file_name, unsigned int scalar_size_in_bytes)
{
    vtkSmartPointer<vtkImageCast> image_cast = vtkSmartPointer<vtkImageCast>::New();
    image_cast->SetInputData(this->m_vtk_image_data);
    switch (scalar_size_in_bytes)
    {
        case 1:
            image_cast->SetOutputScalarTypeToUnsignedChar();
            break;
        case 2:
            image_cast->SetOutputScalarTypeToUnsignedShort();
            break;
        case 4:
            image_cast->SetOutputScalarTypeToUnsignedInt();
            break;
//        case 8:
//            image_cast->SetOutputScalarTypeToUnsignedLong();
//            break;
    }
    image_cast->Update();
    
    vtkSmartPointer<vtkStructuredPointsWriter> writer = vtkSmartPointer<vtkStructuredPointsWriter>::New();
    
    mxString header_string;
    header_string.Append("T");
    header_string.AppendNumber((int)this->GetDimension_T());
    header_string.Append("t");
    header_string.Append("S");
    header_string.AppendNumber((int)this->GetDimension_S());
    header_string.Append("s");
    double Cx, Cy, Cz;
    this->GetOrientation_C_Vector(Cx, Cy, Cz);
    header_string.Append("O"); header_string.AppendNumber(Cx);
    header_string.Append(" "); header_string.AppendNumber(Cy);
    header_string.Append(" "); header_string.AppendNumber(Cz);
    double Rx, Ry, Rz;
    this->GetOrientation_R_Vector(Rx, Ry, Rz);
    header_string.Append(" "); header_string.AppendNumber(Rx);
    header_string.Append(" "); header_string.AppendNumber(Ry);
    header_string.Append(" "); header_string.AppendNumber(Rz);
    header_string.Append("o");
    
    writer->SetHeader(header_string.Get_C_String());
    
    
    
    
    mxString file_string;
    file_string.Assign(file_name);
    if( (file_string[file_string.GetNumberOfCharacters()-1]!='k' && file_string[file_string.GetNumberOfCharacters()-1]!='K') || (file_string[file_string.GetNumberOfCharacters()-2]!='t' && file_string[file_string.GetNumberOfCharacters()-2]!='T') || (file_string[file_string.GetNumberOfCharacters()-3]!='v' && file_string[file_string.GetNumberOfCharacters()-3]!='V') || file_string[file_string.GetNumberOfCharacters()-4]!='.' )
    {
        file_string.Append(".vtk");
    }
    
    writer->SetInputConnection(image_cast->GetOutputPort());
    writer->SetFileName(file_string.Get_C_String());
    writer->SetFileTypeToBinary();
    writer->Update();
    writer->Write();
}


template<class T>
void vmxImageDataT<T>::SaveToVTK8UFile(const char *file_name)
{
    this->SaveToVTKFile(file_name,1);
}


template<class T>
void vmxImageDataT<T>::SaveToVTK16UFile(const char *file_name)
{
    this->SaveToVTKFile(file_name,2);
}


template<class T>
void vmxImageDataT<T>::SaveToVTK32UFile(const char *file_name)
{
    this->SaveToVTKFile(file_name,4);
}


template<class T>
void vmxImageDataT<T>::SaveToPNGImages(const char *file_name_root, unsigned int scalar_size_in_bytes)
{
    vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
    flip->SetInputData(this->m_vtk_image_data);
    flip->SetFilteredAxis(0);
    flip->Update();
    
    double *scalar_range = this->m_vtk_image_data->GetScalarRange();
    double difference = scalar_range[1] - scalar_range[0];
    
    vtkSmartPointer<vtkImageShiftScale> shift_scale = vtkSmartPointer<vtkImageShiftScale>::New();
    
    switch (scalar_size_in_bytes)
    {
        case 1:
            shift_scale->SetScale(255.0/difference);
            shift_scale->SetOutputScalarTypeToUnsignedChar();
            break;
        case 2:
            shift_scale->SetScale(65535.0/difference);
            shift_scale->SetOutputScalarTypeToUnsignedShort();
            break;
        case 4:
            shift_scale->SetScale(4294967295.0/difference);
            shift_scale->SetOutputScalarTypeToUnsignedInt();
            break;
//        case 8:
//            shift_scale->SetScale((2^64-1)/difference);
//            shift_scale->SetOutputScalarTypeToUnsignedLong();
//            break;
    }
    
    shift_scale->SetInputConnection(flip->GetOutputPort());
    shift_scale->Update();
    
    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetInputConnection(shift_scale->GetOutputPort());
    writer->SetFilePrefix(file_name_root);
    do // run through this only once
    {
        if(this->GetNumberOfDataElements()<10)
        {
            writer->SetFilePattern("%s%01d.png");
            break;
        }
        if(this->GetNumberOfDataElements()<100)
        {
            writer->SetFilePattern("%s%02d.png");
            break;
        }
        if(this->GetNumberOfDataElements()<1000)
        {
            writer->SetFilePattern("%s%03d.png");
            break;
        }
        if(this->GetNumberOfDataElements()<10000)
        {
            writer->SetFilePattern("%s%04d.png");
            break;
        }
        if(this->GetNumberOfDataElements()<100000)
        {
            writer->SetFilePattern("%s%05d.png");
            break;
        }
    }while(0);
    
    writer->Update();
    writer->Write();
}


template<class T>
void vmxImageDataT<T>::SaveToPNG8UImages(const char *file_name_root)
{
    vmxImageDataT<T>::SaveToPNGImages(file_name_root,1);
}


template<class T>
void vmxImageDataT<T>::SaveToPNG16UImages(const char *file_name_root)
{
    vmxImageDataT<T>::SaveToPNGImages(file_name_root,2);
}


template<class T>
void vmxImageDataT<T>::SetOrigin(double t, double s, double r, double c)
{
	this->mxImageT<T>::SetOrigin(t,s,r,c);
	this->m_vtk_image_data->SetOrigin(c,r,s);
    for(unsigned int i=0; i<this->m_vtk_image_data_array.GetNumberOfElements(); i++)
    {
        this->m_vtk_image_data_array[i]->SetOrigin(mxImageT<T>::GetOrigin_C(),mxImageT<T>::GetOrigin_R(),mxImageT<T>::GetOrigin_S());
    }
}


template<class T>
void vmxImageDataT<T>::SetSpacing(double t, double s, double r, double c)
{
	this->mxImageT<T>::SetSpacing(t,s,r,c);
	this->m_vtk_image_data->SetSpacing(c,r,s);
    for(unsigned int i=0; i<this->m_vtk_image_data_array.GetNumberOfElements(); i++)
    {
        this->m_vtk_image_data_array[i]->SetSpacing(mxImageT<T>::GetSpacing_C(),mxImageT<T>::GetSpacing_R(),mxImageT<T>::GetSpacing_S());
    }
}


template<class T>
int vmxImageDataT<T>::SetDimensions(unsigned int time, unsigned int slices, unsigned int rows, unsigned int columns)
{
	if(!(this->mxImageT<T>::SetDimensions(time,slices,rows,columns))) return 0;
    
    return (this->vmxImageDataT<T>::internalAttachToPreset_mxImage());
}


template<class T>
void vmxImageDataT<T>::SetDimensionsAs(mxImageT<T> *image)
{
    this->vmxImageDataT<T>::SetDimensions(image->GetDimension_T(),image->GetDimension_S(),image->GetDimension_R(),image->GetDimension_C());
}


template<class T>
void vmxImageDataT<T>::SetDimensionsAndPropertiesAs(mxImageT<T> *image)
{
    this->vmxImageDataT<T>::SetDimensionsAs(image);
    this->vmxImageDataT<T>::SetPropertiesAs(image);
}


template<class T>
void vmxImageDataT<T>::SetPropertiesAs(mxImageT<T> *img)
{
    this->vmxImageDataT<T>::SetOrigin(img->GetOrigin_T(),img->GetOrigin_S(),img->GetOrigin_R(),img->GetOrigin_C());
    this->vmxImageDataT<T>::SetSpacing(img->GetSpacing_T(),img->GetSpacing_S(),img->GetSpacing_R(),img->GetSpacing_C());
}


template<class T>
int vmxImageDataT<T>::TakeOverVTKImageData(vtkImageData *vtk_image, unsigned int number_of_time_series, unsigned int number_of_slices)
{
    if(!vtk_image) return 0;
    if(this->m_vtk_image_data == vtk_image) return 1;
    
    this->vmxImageDataT<T>::Reset();
    
    this->mxImageT<T>::SetSpacing(1,vtk_image->GetSpacing()[2],vtk_image->GetSpacing()[1],vtk_image->GetSpacing()[0]);
    this->mxImageT<T>::SetOrigin(0,vtk_image->GetOrigin()[2],vtk_image->GetOrigin()[1],vtk_image->GetOrigin()[0]);
    
    
    this->m_vtk_image_data->ShallowCopy(vtk_image);// m_vtk_image_data becomes the owner of the data.
    
    int type_size = sizeof(T);
    if(type_size==1)// UINT8
    {
        this->m_vtk_image_data->SetPointDataActiveScalarInfo(this->m_vtk_image_data->GetInformation(),VTK_TYPE_UINT8,1);
    }
    if(type_size==2)// UINT16
    {
        this->m_vtk_image_data->SetPointDataActiveScalarInfo(this->m_vtk_image_data->GetInformation(),VTK_TYPE_UINT16,1);
    }
    if(type_size==4)// UINT32
    {
        this->m_vtk_image_data->SetPointDataActiveScalarInfo(this->m_vtk_image_data->GetInformation(),VTK_TYPE_UINT32,1);
    }
//    if(type_size==8)// UINT64
//    {
//        this->m_vtk_image_data->SetPointDataActiveScalarInfo(this->m_vtk_image_data->GetInformation(),VTK_TYPE_UINT64,1);
//    }
    
    
    if(number_of_time_series==0 || number_of_slices==0 || (number_of_time_series*number_of_slices != vtk_image->GetDimensions()[2]) )
    {
        number_of_time_series = 1;
        number_of_slices = this->m_vtk_image_data->GetDimensions()[2];
    }
    
    
    
    this->mxImageT<T>::AttachData(m_vtk_image_data->GetScalarPointer(), 0, (unsigned int)(this->m_vtk_image_data->GetDimensions()[0]), (unsigned int)(this->m_vtk_image_data->GetDimensions()[1]), number_of_slices, number_of_time_series);
    
    
    
   
    
    this->m_vtk_image_data_array.SetNumberOfElements(number_of_time_series);
    int src = number_of_slices * this->m_vtk_image_data->GetDimensions()[1] * this->m_vtk_image_data->GetDimensions()[0];
    for(unsigned int i=0; i<this->m_vtk_image_data_array.GetNumberOfElements(); i++)
    {
        this->m_vtk_image_data_array[i] = vtkSmartPointer<vtkImageData>::New();
        this->m_vtk_image_data_array[i]->SetDimensions(this->m_vtk_image_data->GetDimensions()[0], this->m_vtk_image_data->GetDimensions()[1], number_of_slices);
        this->m_vtk_image_data_array[i]->SetExtent(0, this->m_vtk_image_data->GetDimensions()[0]-1, 0, this->m_vtk_image_data->GetDimensions()[1]-1, 0, number_of_slices-1);
        this->m_vtk_image_data_array[i]->SetOrigin(this->m_vtk_image_data->GetOrigin());
        this->m_vtk_image_data_array[i]->SetSpacing(this->m_vtk_image_data->GetSpacing());
        
        switch(type_size)
        {
            case 1:
            {
                uint8_t *data_address = (uint8_t*)(this->GetData());
                this->m_vtk_image_data_array[i]->SetPointDataActiveScalarInfo(this->m_vtk_image_data_array[i]->GetInformation(),VTK_TYPE_UINT8,1);
                vtkSmartPointer<vtkTypeUInt8Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt8Array>::New();
                vtk_data_array->SetArray(&(data_address[i*src]), src, 1);//The last argument value '1' indicates that VTK must not free array memory.
                m_vtk_image_data_array[i]->GetPointData()->SetScalars(vtk_data_array);
            }
                break;
            case 2:
            {
                uint16_t *data_address = (uint16_t*)(this->GetData());
                this->m_vtk_image_data_array[i]->SetPointDataActiveScalarInfo(this->m_vtk_image_data_array[i]->GetInformation(),VTK_TYPE_UINT16,1);
                vtkSmartPointer<vtkTypeUInt16Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt16Array>::New();
                vtk_data_array->SetArray(&(data_address[i*src]), src, 1);//The last argument value '1' indicates that VTK must not free array memory.
                m_vtk_image_data_array[i]->GetPointData()->SetScalars(vtk_data_array);
            }
                break;
            case 4:
            {
                uint32_t *data_address = (uint32_t*)(this->GetData());
                this->m_vtk_image_data_array[i]->SetPointDataActiveScalarInfo(this->m_vtk_image_data_array[i]->GetInformation(),VTK_TYPE_UINT32,1);
                vtkSmartPointer<vtkTypeUInt32Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt32Array>::New();
                vtk_data_array->SetArray(&(data_address[i*src]), src, 1);//The last argument value '1' indicates that VTK must not free array memory.
                m_vtk_image_data_array[i]->GetPointData()->SetScalars(vtk_data_array);
            }
                break;
//            case 8:
//            {
//                uint64_t *data_address = (uint64_t*)(this->GetData()); // this line does not wotk with vtkUnsignedLongArray.
//                this->m_vtk_image_data_array[i]->SetPointDataActiveScalarInfo(this->m_vtk_image_data_array[i]->GetInformation(),VTK_TYPE_UINT64,1);
//                vtkSmartPointer<vtkTypeUInt64Array> vtk_data_array = vtkSmartPointer<vtkTypeUInt64Array>::New();
//                vtk_data_array->SetArray(&(data_address[i*src]), src, 1);//The last argument value '1' indicates that VTK must not free array memory.
//                m_vtk_image_data_array[i]->GetPointData()->SetScalars(vtk_data_array);
//            }
//                break;
                
                cout<<"vmxImageDataT<T>::TakeOverVTKImageData() error: the type of size size(T)="<<type_size<<" is not supported!"<<endl;
                return 0;
        }
    }
    
    return 1;
}






template class vmxImageData_API vmxImageDataT<uint8_t>;
template class vmxImageData_API vmxImageDataT<uint16_t>;
template class vmxImageData_API vmxImageDataT<uint32_t>;
//template class vmxImageData_API vmxImageDataT<uint64_t>;



