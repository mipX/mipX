/*=========================================================================
 
 Program:   mipx
 Module:    mxImage.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxImage.h"



template<class T>
mxImageT<T>::mxImageT()
{
    int type_size = sizeof(T);
    int number_of_bits = 8 * type_size;
    this->m_class_name.Assign("mxImage");
    this->m_class_name.AppendNumber(number_of_bits);
    this->m_class_name.Append("U");
}


template<class T>
mxImageT<T>::~mxImageT()
{
	this->mxImageT<T>::Reset();
}


template<class T>
int mxImageT<T>::AttachData(void *data_address, int is_data_owned_by_mxImage,  unsigned int dimension_c, unsigned int dimension_r, unsigned int dimension_s, unsigned int dimension_t)
{
    if(!this->m_grid.AttachData(((T*)data_address), dimension_c, dimension_r, dimension_s, dimension_t)) return 0;
    this->m_is_data_owned_by_mxImage = is_data_owned_by_mxImage;
    this->m_tags.SetDimensions(dimension_t,dimension_s);
    return 1;
}


template<class T>
int mxImageT<T>::ConvertToDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    if(!this->m_tags.m_tags.ConvertToDimensions(1, 1, t, s)) return 0;
    return this->m_grid.ConvertToDimensions(t,s,r,c);
}


template<class T>
int mxImageT<T>::CopyFromDataObject(mxDataObject *data_object)
{
    if(!data_object) return 0;
    if(data_object->GetClassName()!=this->GetClassName()) return 0;
    
    mxImageT<T> *image = dynamic_cast<mxImageT<T>*>(data_object);
    
    if(!image) return 0;
    
    return (this->mxImageT<T>::Copy(image));
}


template<class T>
int mxImageT<T>::Copy(mxImageT<T> *image)
{
    //std::cout<<std::endl<<" mxImageT<T>::Copy(mxImageT<T> *image) was called!";
    if(!image) return 0;

    if(this==image) return 1;
    if(image->IsEmpty()) return 0;
    
    this->mxImageT<T>::SetDimensionsAndPropertiesAs(image);
    
    for(unsigned int i=0; i<image->GetNumberOfDataElements(); i++)
    {
        this->m_grid[i] = image->m_grid[i];
    }
    
    m_tags = image->m_tags;
    
    //if(!this->mxDataObject::CopyFromDataObject(image)) return 0;
    
    return 1;
}


template<class T>
void* mxImageT<T>::DetachData()
{
    this->m_is_data_owned_by_mxImage = 0;
    return ((void*)this->m_grid.DetachData());
}


template<class T>
void mxImageT<T>::FillInWith(mxImageScalar value)
{
    for(unsigned int i=0; i<this->GetNumberOfDataElements(); i++)
    {
        (*this)[i] = (T)value;
    }
}


template<class T>
void mxImageT<T>::FillIn_3D_ImageWith(unsigned int t, mxImageScalar value)
{
    for(unsigned int s=0; s<this->GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<this->GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<this->GetDimension_C(); c++)
            {
                (*this)(t,s,r,c) = (T)value;
            }
        }
    }
}


template<class T>
void mxImageT<T>::FillInSliceWith(unsigned int t, unsigned int s, mxImageScalar value)
{
    for(unsigned int r=0; r<this->GetDimension_R(); r++)
    {
        for(unsigned int c=0; c<this->GetDimension_C(); c++)
        {
            (*this)(t,s,r,c) = (T)value;
        }
    }
}


template<class T>
mxImageScalar mxImageT<T>::Get(unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    return (this->m_grid(t,s,r,c));
}


template<class T>
mxImageScalar mxImageT<T>::Get(unsigned int s, unsigned int r, unsigned int c)
{
    return (this->m_grid(0,s,r,c));
}


template<class T>
mxImageScalar mxImageT<T>::Get(unsigned int r, unsigned int c)
{
    return (this->m_grid(0,0,r,c));
}


template<class T>
mxImageScalar mxImageT<T>::Get(unsigned int i)
{
    return (this->m_grid[i]);
}


template<class T>
int mxImageT<T>::GetBoundsInWorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max)
{
    if(this->IsEmpty()) return 0;
    
    this->WorldCoordinatesFromIndexes(-1,-1,-1,z_min,y_min,x_min);
    this->WorldCoordinatesFromIndexes(this->GetDimension_S(),this->GetDimension_R(),this->GetDimension_C(),z_max,y_max,x_max);
    
    // Check if 'min' values are really smaller than 'max' values (it may not be so due to various orientations). If not, invert them.
    if(x_max<x_min){ double x_temp = x_max; x_max = x_min; x_min = x_temp; }
    if(y_max<y_min){ double y_temp = y_max; y_max = y_min; y_min = y_temp; }
    if(z_max<z_min){ double z_temp = z_max; z_max = z_min; z_min = z_temp; }
    
    return 1;
}


template<class T>
void* mxImageT<T>::GetData()
{
    return ((void*)(this->m_grid.GetDataAddress()));
}


template<class T>
unsigned int mxImageT<T>::GetDimension_T()
{
    return this->m_grid.GetNumberOfTimeSeries();
}


template<class T>
unsigned int mxImageT<T>::GetDimension_S()
{
    return this->m_grid.GetNumberOfSlices();
}


template<class T>
unsigned int mxImageT<T>::GetDimension_R()
{
    return this->m_grid.GetNumberOfRows();
}


template<class T>
unsigned int mxImageT<T>::GetDimension_C()
{
    return this->m_grid.GetNumberOfColumns();
}


template<class T>
unsigned int mxImageT<T>::GetNumberOfDataElements()
{
    return this->m_grid.GetNumberOfDataElements();
}


template<class T>
double* mxImageT<T>::GetOrientation()
{
    return this->m_grid.GetOrientation();
}


template<class T>
void mxImageT<T>::GetOrientation_C_Vector(double &Cx, double &Cy, double &Cz)
{
    this->m_grid.GetOrientation_C_Vector(&Cx,&Cy,&Cz);
}


template<class T>
void mxImageT<T>::GetOrientation_R_Vector(double &Rx, double &Ry, double &Rz)
{
    this->m_grid.GetOrientation_R_Vector(&Rx,&Ry,&Rz);
}


template<class T>
void mxImageT<T>::GetOrientation_S_Vector(double &Sx, double &Sy, double &Sz)
{
    this->m_grid.GetOrientation_S_Vector(&Sx,&Sy,&Sz);
}


template<class T>
double mxImageT<T>::GetOrigin_T()
{
    return this->m_grid.GetOrigin_T();
}


template<class T>
double mxImageT<T>::GetOrigin_S()
{
    return this->m_grid.GetOrigin_S();
}


template<class T>
double mxImageT<T>::GetOrigin_R()
{
    return this->m_grid.GetOrigin_R();
}


template<class T>
double mxImageT<T>::GetOrigin_C()
{
    return this->m_grid.GetOrigin_C();
}


template<class T>
int mxImageT<T>::GetScalarSizeInBytes()
{
    return (sizeof(T));
}


template<class T>
double mxImageT<T>::GetSpacing_T()
{
    return this->m_grid.GetSpacing_T();
}


template<class T>
double mxImageT<T>::GetSpacing_S()
{
    return this->m_grid.GetSpacing_S();
}


template<class T>
double mxImageT<T>::GetSpacing_R()
{
    return this->m_grid.GetSpacing_R();
}


template<class T>
double mxImageT<T>::GetSpacing_C()
{
    return this->m_grid.GetSpacing_C();
}


template<class T>
int mxImageT<T>::GetVoxelMinAndMaxValues(uint64_t &min_value_included, uint64_t &max_value_included)
{
    if(this->IsEmpty()) { return 0; }
    T* data = (T*) this->GetData();
    min_value_included = max_value_included = data[0];
    for(unsigned int i=0; i<this->GetNumberOfDataElements(); i++)
    {
        if(data[i]>max_value_included) max_value_included = data[i];
        if(data[i]<min_value_included) min_value_included = data[i];
    }
    return 1;
}


template<class T>
void mxImageT<T>::GetVoxelValueFullRange(uint64_t &min_value_included, uint64_t &max_value_included)
{
    int type_size = sizeof(T);
    
    min_value_included = 0;
    max_value_included = (2^type_size) - 1;
}


template<class T>
int mxImageT<T>::IndexFromWorldCoordinates(double w_t, double w_z, double w_y, double w_x, int &out_t, int &out_s, int &out_r, int &out_c)
{
    return this->m_grid.IndexFromWorldCoordinates(w_t, w_z, w_y, w_x, out_t, out_s, out_r, out_c);
}


template<class T>
int mxImageT<T>::IndexFromWorldCoordinates(double w_z, double w_y, double w_x, int &out_s, int &out_r, int &out_c)
{
    return this->m_grid.IndexFromWorldCoordinates(w_z, w_y, w_x, out_s, out_r, out_c);
}


template<class T>
int mxImageT<T>::IsEmpty()
{
    return this->m_grid.IsEmpty();
}


template<class T>
int mxImageT<T>::IsEqualInDimensions_4D(mxImageT<T> &image)
{
    //mxImageT<T> *img = static_cast< mxImageT<T>* >(&image);
    return this->m_grid.IsEqualDimensions_4D(image.m_grid);// img->m_grid);
}


template<class T>
int mxImageT<T>::IsEqualInDimensions_3D(mxImageT<T> &image)
{
    //mxImageT<T> *img = static_cast< mxImageT<T>* >(&image);
    return this->m_grid.IsEqualDimensions_3D(image.m_grid);// img->m_grid);
}


template<class T>
int mxImageT<T>::IsEqualInDimensions_2D(mxImageT<T> &image)
{
    //mxImageT<T> *img = static_cast< mxImageT<T>* >(&image);
    return this->m_grid.IsEqualDimensions_2D(image.m_grid);// img->m_grid);
}


template<class T>
T& mxImageT<T>::operator[](unsigned int i)
{
    return this->m_grid[i];
}


template<class T>
T& mxImageT<T>::operator()(unsigned int r, unsigned int c)
{
    return this->m_grid(0,0,r,c);
}


template<class T>
T& mxImageT<T>::operator()(unsigned int s, unsigned int r, unsigned int c)
{
    return this->m_grid(0,s,r,c);
}


template<class T>
T& mxImageT<T>::operator()(unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    return this->m_grid(t,s,r,c);
}


template<class T>
void mxImageT<T>::Reset()
{
    if(this->IsEmpty()) return;
    if(this->m_is_data_owned_by_mxImage)
    {
        this->m_grid.Reset();
    }
    else
    {
        this->mxImageT<T>::DetachData();
    }
    this->m_tags.Reset();
}


template<class T>
void mxImageT<T>::Set(unsigned int t, unsigned int s, unsigned int r, unsigned int c, mxImageScalar value)
{
    this->m_grid(t,s,r,c) = (T)value;
}


template<class T>
void mxImageT<T>::Set(unsigned int s, unsigned int r, unsigned int c, mxImageScalar value)
{
    this->m_grid(0,s,r,c) = (T)value;
}


template<class T>
void mxImageT<T>::Set(unsigned int r, unsigned int c, mxImageScalar value)
{
    this->m_grid(0,0,r,c) = (T)value;
}


template<class T>
void mxImageT<T>::Set(unsigned int i, mxImageScalar value)
{
    this->m_grid[i] = (T)value;
}


template<class T>
int mxImageT<T>::SetDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    this->mxImageT<T>::Reset();
    this->m_is_data_owned_by_mxImage = 1;
    this->m_tags.SetDimensions(t,s);
    return (this->m_grid.SetDimensions(t,s,r,c));
}


template<class T>
void mxImageT<T>::SetDimensionsAndPropertiesAs(mxImageT<T> *image)
{
    this->mxImageT<T>::SetDimensionsAs(image);
    this->mxImageT<T>::SetPropertiesAs(image);
}


template<class T>
void mxImageT<T>::SetDimensionsAs(mxImageT<T> *image)
{
    this->m_grid.SetDimensions(image->GetDimension_T(),image->GetDimension_S(),image->GetDimension_R(),image->GetDimension_C());
    this->m_tags.SetDimensions(image->GetDimension_T(),image->GetDimension_S());
}


template<class T>
void mxImageT<T>::SetOrientation(double cx, double cy, double cz, double rx, double ry, double rz)
{
    this->m_grid.SetOrientation(cx, cy, cz, rx, ry, rz);
}


template<class T>
void mxImageT<T>::SetOrigin(double t, double s, double r, double c)
{
    this->m_grid.SetOrigin(t,s,r,c);
}


template<class T>
void mxImageT<T>::SetPropertiesAs(mxImageT<T> *image)
{
    this->mxImageT<T>::SetSpacing(image->GetSpacing_T(),image->GetSpacing_S(),image->GetSpacing_R(),image->GetSpacing_C());
    this->mxImageT<T>::SetOrigin(image->GetOrigin_T(),image->GetOrigin_S(),image->GetOrigin_R(),image->GetOrigin_C());
    this->mxImageT<T>::SetOrientation(image->GetOrientation()[0],image->GetOrientation()[1],image->GetOrientation()[2],image->GetOrientation()[3],image->GetOrientation()[4],image->GetOrientation()[5]);
}


template<class T>
void mxImageT<T>::SetSpacing(double t, double s, double r, double c)
{
    this->m_grid.SetSpacing(t,s,r,c);
}


template<class T>
void mxImageT<T>::SortSlices(const char *criteria1_tag_number1, const char *criteria1_tag_number2, const char *criteria2_tag_number1, const char *criteria2_tag_number2, const char *criteria3_tag_number1, const char *criteria3_tag_number2)
{
    //This array will indicate how the indexes changed during the sort.
    mxArray<unsigned int> index_array;
    index_array.SetNumberOfElements(m_tags.m_tags.GetNumberOfDataElements());
    for(unsigned int i=0; i<index_array.GetNumberOfElements(); i++) index_array[i] = i;
    
    // Do the bubble sort based on the given criteria.
    int is_change_made = 1;
    while(is_change_made)
    {
        is_change_made = 0;
        for(unsigned int i=0; i<m_tags.m_tags.GetNumberOfDataElements()-1; i++)
        {
            mxArray<double> value_array1;
            if(!m_tags.GetTag(i).GetValueAsArrayOfDoubles(criteria1_tag_number1, criteria1_tag_number2, value_array1))
            {
                std::cout<<std::endl<<"mxImageT<T>::SortSlices() error getting values from tags:"<<criteria1_tag_number1<<","<<criteria1_tag_number2;
                return;
            }
            mxArray<double> value_array2;
            
            if(!m_tags.GetTag(i+1).GetValueAsArrayOfDoubles(criteria1_tag_number1, criteria1_tag_number2, value_array2))
            {
                std::cout<<std::endl<<"mxImageT<T>::SortSlices(): error getting values from tags:"<<criteria1_tag_number1<<","<<criteria1_tag_number2;
                return;
            }
            
            double v1 = 0, v2 = 0;
            for(unsigned int j=0; j<value_array1.GetNumberOfElements() && j<value_array2.GetNumberOfElements(); j++)
            {
                if(value_array1[j]+0.00005<value_array2[j] || value_array1[j]-0.00005>value_array2[j])
                {
                    v1 = value_array1[j]; v2 = value_array2[j];
                    break;
                }
            }
            
            //Sorting based on the criteria1
            if(v1>v2)
            {
                //Invert the tag order
                mxImageSliceTag temp_tag;
                temp_tag = m_tags.GetTag(i+1);
                m_tags.GetTag(i+1) = m_tags.GetTag(i);
                m_tags.GetTag(i) = temp_tag;
                
                //Invert the index array elements order
                unsigned int temp_i;
                temp_i = index_array[i+1];
                index_array[i+1] = index_array[i];
                index_array[i] = temp_i;
                
                is_change_made = 1;
            }
            else
            {
                if(v1==v2 && criteria2_tag_number1 && criteria2_tag_number2)
                {
                    
                    if(!m_tags.GetTag(i).GetValueAsArrayOfDoubles(criteria2_tag_number1, criteria2_tag_number2, value_array1))
                    {
                        std::cout<<std::endl<<"mxImageT<T>::SortSlices(): Error getting values from tags:"<<criteria2_tag_number1<<","<<criteria2_tag_number2;
                        return;
                    }
                    
                    if(!m_tags.GetTag(i+1).GetValueAsArrayOfDoubles(criteria2_tag_number1, criteria2_tag_number2, value_array2))
                    {
                        std::cout<<std::endl<<"mxImageT<T>::SortSlices(): Error getting values from tags:"<<criteria2_tag_number1<<","<<criteria2_tag_number2;
                        return;
                    }
                    
                    
                    v1 = 0, v2 = 0;
                    for(unsigned int j=0; j<value_array1.GetNumberOfElements() && j<value_array2.GetNumberOfElements(); j++)
                    {
                        if(value_array1[j]+0.00005<value_array2[j] || value_array1[j]-0.00005>value_array2[j])
                        {
                            v1 = value_array1[j]; v2 = value_array2[j];
                            break;
                        }
                    }
                    
                    //Sorting based on the criteria2
                    if(v1>v2)
                    {
                        //Invert the tag order
                        mxImageSliceTag temp_tag;
                        temp_tag = m_tags.GetTag(i+1);
                        m_tags.GetTag(i+1) = m_tags.GetTag(i);
                        m_tags.GetTag(i) = temp_tag;
                        
                        //Invert the index array elements order
                        unsigned int temp_i;
                        temp_i = index_array[i+1];
                        index_array[i+1] = index_array[i];
                        index_array[i] = temp_i;
                        
                        is_change_made = 1;
                    }
                    else
                    {
                        if(v1==v2 && criteria3_tag_number1 && criteria3_tag_number2)
                        {
                            
                            
                            if(!m_tags.GetTag(i).GetValueAsArrayOfDoubles(criteria3_tag_number1, criteria3_tag_number2, value_array1))
                            {
                                std::cout<<std::endl<<"mxImageT<T>::SortSlices(): Error getting values from tags:"<<criteria3_tag_number1<<","<<criteria3_tag_number2;
                                return;
                            }
                            
                            if(!m_tags.GetTag(i+1).GetValueAsArrayOfDoubles(criteria3_tag_number1, criteria3_tag_number2, value_array2))
                            {
                                std::cout<<std::endl<<"mxImageT<T>::SortSlices(): Error getting values from tags:"<<criteria3_tag_number1<<","<<criteria3_tag_number2;
                                return;
                            }
                            
                            
                            v1 = 0, v2 = 0;
                            for(unsigned int j=0; j<value_array1.GetNumberOfElements() && j<value_array2.GetNumberOfElements(); j++)
                            {
                                if(value_array1[j]+0.00005<value_array2[j] || value_array1[j]-0.00005>value_array2[j])
                                {
                                    v1 = value_array1[j]; v2 = value_array2[j];
                                    break;
                                }
                            }
                            
                            
                            //Sorting based on the criteria3
                            if(v1>v2)
                            {
                                //Invert the tag order
                                mxImageSliceTag temp_tag;
                                temp_tag = m_tags.GetTag(i+1);
                                m_tags.GetTag(i+1) = m_tags.GetTag(i);
                                m_tags.GetTag(i) = temp_tag;
                                
                                //Invert the index array elements order
                                unsigned int temp_i;
                                temp_i = index_array[i+1];
                                index_array[i+1] = index_array[i];
                                index_array[i] = temp_i;
                                
                                is_change_made = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    
    //Tags are sorted, the index change order is in index_array, so we now have to sort slices.
    mxImageT<T> temp_img;//temp_img.SetSize(1,1,this->GetNumberOfRows(),this->GetNumberOfColumns());
    temp_img.Copy(this);
    for(unsigned int i=0; i<index_array.GetNumberOfElements(); i++)
    {
        int old_index = index_array[i];// new index is 'i'.
        int s_old = old_index % this->GetDimension_S(); //GetNumberOfSlices();
        int t_old = old_index / this->GetDimension_S(); //GetNumberOfSlices();
        int s_new = i % this->GetDimension_S(); //GetNumberOfSlices();
        int t_new = i / this->GetDimension_S(); //GetNumberOfSlices();
        
        for(unsigned int r=0; r<this->GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<this->GetDimension_C(); c++)
            {
                (*this)(t_new,s_new,r,c) = temp_img(t_old,s_old,r,c);
            }
        }
    }
}


template<class T>
int mxImageT<T>::WorldCoordinatesFromIndexes(int t, int s, int r, int c, double &out_w_t, double &out_w_z, double &out_w_y, double &out_w_x)
{
    return this->m_grid.WorldCoordinatesFromIndexes(t, s, r, c, out_w_t, out_w_z, out_w_y, out_w_x);
}


template<class T>
int mxImageT<T>::WorldCoordinatesFromIndexes(int s, int r, int c, double &out_w_z, double &out_w_y, double &out_w_x)
{
    double w_t;
    return this->m_grid.WorldCoordinatesFromIndexes(0, s, r, c, w_t, out_w_z, out_w_y, out_w_x);
}


template class mxImage_API mxImageT<uint8_t>;
template class mxImage_API mxImageT<uint16_t>;
template class mxImage_API mxImageT<uint32_t>;
//template class mxImage_API mxImageT<uint64_t>;







