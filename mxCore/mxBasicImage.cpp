/*=========================================================================
 
 Program:   mipx
 Module:    mxImage.h
 
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxImage.h"



template<class T>
mxImageT<T>::mxImageT()
{
}


template<class T>
mxImageT<T>::~mxImageT()
{
	this->Reset();
}


template<class T>
int mxImageT<T>::AttachData(T *data_address, unsigned int n_of_columns, unsigned int n_of_rows, unsigned int n_of_slices, unsigned int n_of_time_series)
{
    if(!this->m_grid.AttachData(data_address,n_of_columns,n_of_rows,n_of_slices,n_of_time_series)) return 0;
    return 1;
}


template<class T>
int mxImageT<T>::CopyFrom(mxObject *image)
{
    if(image->GetClassName()!=this->GetClassName()) return 0;
    
    mxImageT<T> *m = static_cast<mxImageT<T>*>(image);
    if(m->IsEmpty()) return 0;
    
    if(!mxDataObject::CopyFrom(image)) return 0;
    
    if(!this->m_grid.CopyFrom(&(m->grid))) return 0;
    
    return 1;
}


template<class T>
mxImageT<T>& mxImageT<T>::operator =(mxImageT<T> &image)
{
	this->CopyFrom(&image);
	return(*this);
}


template<class T>
int mxImageT<T>::IsEmpty()
{
    return this->m_grid.IsEmpty();
}


template<class T>
int mxImageT<T>::IsEqual_4D_Size(mxImageT<T> &image)
{
    return this->m_grid.IsEqual_4D_Size(image.m_grid);
}


template<class T>
int mxImageT<T>::IsEqual_3D_Size(mxImageT<T> &image)
{
    return this->m_grid.IsEqual_3D_Size(image.m_grid);
}


template<class T>
int mxImageT<T>::IsEqual_2D_Size(mxImageT<T> &image)
{
    return this->m_grid.IsEqual_2D_Size(image.m_grid);
}


template<class T>
T* mxImageT<T>::GetData()
{
    return this->m_grid.GetData();
}


template<class T>
unsigned int* mxImageT<T>::GetDimensions()
{
	return this->m_grid.GetDimensions();
}


template<class T>
unsigned int mxImageT<T>::GetNumberOfTimeSeries()
{
    return this->m_grid.GetNumberOfTimeSeries();
}


template<class T>
unsigned int mxImageT<T>::GetNumberOfSlices()
{
	return this->m_grid.GetNumberOfSlices();
}


template<class T>
unsigned int mxImageT<T>::GetNumberOfRows()
{
	return this->m_grid.GetNumberOfRows();
}


template<class T>
unsigned int mxImageT<T>::GetNumberOfColumns()
{
	return this->m_grid.GetNumberOfColumns();
}


template<class T>
unsigned int mxImageT<T>::GetNumberOfDataElements()
{
    return this->m_grid.GetNumberOfDataElements();
}


template<class T>
double* mxImageT<T>::GetOrigin()
{
    return this->m_grid.GetOrigin();
}


template<class T>
double mxImageT<T>::GetOriginTime()
{
    return this->m_grid.GetOriginTime();
}


template<class T>
double mxImageT<T>::GetOriginSlices()
{
    return this->m_grid.GetOriginSlices();
}


template<class T>
double mxImageT<T>::GetOriginRows()
{
    return this->m_grid.GetOriginRows();
}


template<class T>
double mxImageT<T>::GetOriginColumns()
{
    return this->m_grid.GetOriginColumns();
}


template<class T>
void mxImageT<T>::SetOrigin(double t, double s, double r, double c)
{
    this->m_grid.SetOrigin(t,s,r,c);
}


template<class T>
double* mxImageT<T>::GetSpacing()
{
    return this->m_grid.GetSpacing();
}


template<class T>
double mxImageT<T>::GetSpacingTime()
{
    return this->m_grid.GetSpacingTime();
}


template<class T>
double mxImageT<T>::GetSpacingSlices()
{
    return this->m_grid.GetSpacingSlices();
}


template<class T>
double mxImageT<T>::GetSpacingRows()
{
    return this->m_grid.GetSpacingRows();
}


template<class T>
double mxImageT<T>::GetSpacingColumns()
{
    return this->m_grid.GetSpacingColumns();
}


template<class T>
void mxImageT<T>::SetSpacing(double t, double s, double r, double c)
{
	this->m_grid.SetSpacing(t,s,r,c);
}


template<class T>
double* mxImageT<T>::GetOrientation()
{
    return this->m_grid.GetOrientation();
}


template<class T>
void mxImageT<T>::GetOrientation_C_Vector(double *Cx, double *Cy, double *Cz)
{
    this->m_grid.GetOrientation_C_Vector(Cx,Cy,Cz);
}


template<class T>
void mxImageT<T>::GetOrientation_R_Vector(double *Rx, double *Ry, double *Rz)
{
    this->m_grid.GetOrientation_R_Vector(Rx,Ry,Rz);
}


template<class T>
void mxImageT<T>::GetOrientation_S_Vector(double *Sx, double *Sy, double *Sz)
{
     this->m_grid.GetOrientation_S_Vector(Sx,Sy,Sz);
}


template<class T>
void mxImageT<T>::SetOrientation(double cx, double cy, double cz, double rx, double ry, double rz)
{
    this->m_grid.SetOrientation(cx, cy, cz, rx, ry, rz);
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


template<class T>
int mxImageT<T>::GetBoundsInWorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max)
{
    if(this->IsEmpty()) return 0;

    this->WorldCoordinatesFromIndexes(-1,-1,-1,z_min,y_min,x_min);
    this->WorldCoordinatesFromIndexes(this->GetNumberOfSlices(),this->GetNumberOfRows(),this->GetNumberOfColumns(),z_max,y_max,x_max);
    
    // Check if 'min' values are really smaller than 'max' values (it may not be so due to various orientations). If not, invert them.
    if(x_max<x_min){ double x_temp = x_max; x_max = x_min; x_min = x_temp; }
    if(y_max<y_min){ double y_temp = y_max; y_max = y_min; y_min = y_temp; }
    if(z_max<z_min){ double z_temp = z_max; z_max = z_min; z_min = z_temp; }
    
    return 1;
}


template<class T>
int mxImageT<T>::SetDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    this->Reset();
    return this->m_grid.SetDimensions(t,s,r,c);
}


template<class T>
void mxImageT<T>::SetDimensionsAndPropertiesAs(mxImageT<T> &image)
{
    this->SetDimensionsAs(image);
    this->SetVisualizationPropertiesAs(image);
}


template<class T>
void mxImageT<T>::SetDimensionsAs(mxImageT<T> &image)
{
    this->m_grid.SetDimensions(image.GetNumberOfTimeSeries(),image.GetNumberOfSlices(),image.GetNumberOfRows(),image.GetNumberOfColumns());
}


template<class T>
void mxImageT<T>::SetVisualizationPropertiesAs(mxImageT<T> &image)
{
    this->SetSpacing(image.GetSpacingTime(),image.GetSpacingSlices(),image.GetSpacingRows(),image.GetSpacingColumns());
    this->SetOrigin(image.GetOriginTime(),image.GetOriginSlices(),image.GetOriginRows(),image.GetOriginColumns());
    this->SetOrientation(image.m_orientation[0],image.m_orientation[1],image.m_orientation[2],image.m_orientation[3],image.m_orientation[4],image.m_orientation[5]);
}


template<class T>
void mxImageT<T>::Reset()
{
	if(this->IsEmpty()) return;
    this->m_grid.Reset();
}


template<class T>
int mxImageT<T>::ConvertToDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    return this->m_grid.ConvertToDimensions(t,s,r,c);
}


template<class T>
void mxImageT<T>::FillInWith(T value)
{
	for(unsigned int i=0; i<this->GetNumberOfDataElements(); i++){ this->m_grid.m_data_address[i] = value; }
}


template<class T>
void mxImageT<T>::FillIn_3D_ImageWith(unsigned int t, T value)
{
    for(unsigned int s=0; s<this->GetNumberOfSlices(); s++)
    {
        for(unsigned int r=0; r<this->GetNumberOfRows(); r++)
        {
            for(unsigned int c=0; c<this->GetNumberOfColumns(); c++)
            {
                (*this)(t,s,r,c) = value;
            }
        }
    }
}


template<class T>
void mxImageT<T>::FillInSliceWith(unsigned int t, unsigned int s, T value)
{
	for(unsigned int r=0; r<this->GetNumberOfRows(); r++)
	{
		for(unsigned int c=0; c<this->GetNumberOfColumns(); c++)
		{
			(*this)(t,s,r,c) = value;
		}
	}
}







