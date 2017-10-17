/*=========================================================================
 
 Program:   mipx
 Module:    mxRegularGrid.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#ifndef mxRegularGrid_DEF
	#define mxRegularGrid_DEF



#include "mxMatrix.h"


/// Regular grid template that will be used to store elements/voxels (e.g. create images).

template<class T>
class mxRegularGrid : public mxMatrix<T>
{
    
protected:

    /// Orientation matrix (in 3D). The values sequentially are: Cx,Cy,Cz, Rx,Ry,Rz, Sx,Sy,Sz.
    /// C vector is directed in increasing direction of the columns, R vector is directed in increasing direction of the rows,
    /// and S vector is calculated from input R and C vectors.
    double m_orientation[9];
    
    /// Origin (in 3D+time). The values sequentially are: C,R,S,T or in world coordinates: x,y,z,time.
    /// The units for origin for C,R,S (x,y,z) coordinates is mm and for begin (origin) time (T) is ms.
    double m_origin[4];
    
    /// Spacing (in 3D+time). The values sequentially are: C,R,S,T or in world coordinates: x,y,z,time.
    /// The units for spacing between slices/pixels is mm and for time (T) is ms.
    double m_spacing[4];

    
public:
	
	/// Constructor.
	mxRegularGrid();
    
    /// Destructor.
	virtual ~mxRegularGrid();
    
	/// Copy from input regular grid.
	virtual int Copy(mxRegularGrid<T> *grid);

    /// Copy from input regular grid.
    int Copy(mxMatrix<T> *matrix);
    
    /// Get orientation.
    double* GetOrientation();
    
    /// Get orientation Columns (C) vector.
    void GetOrientation_C_Vector(double *Cx, double *Cy, double *Cz);
    
    /// Get orientation Rows (R) vector.
    void GetOrientation_R_Vector(double *Rx, double *Ry, double *Rz);
    
    /// Get orientation Slices (S) vector.
    void GetOrientation_S_Vector(double *Sx, double *Sy, double *Sz);
    
    /// Get origin.
    double* GetOrigin();
    
    /// Get origin for columns (X axis).
    double GetOrigin_C();
    
    /// Get origin for rows (Y axis).
    double GetOrigin_R();
    
    /// Get origin for slices (Z axis).
    double GetOrigin_S();
    
    /// Get origin for time series.
    double GetOrigin_T();

    /// Get spacing.
    double* GetSpacing();
    
    /// Get spacing for columns.
    double GetSpacing_C();
    
    /// Get spacing for rows.
    double GetSpacing_R();
    
    /// Get spacing for slices.
    double GetSpacing_S();
    
    /// Get spacing for time series.
    double GetSpacing_T();
    
    /// Calculate index values for given world coordinates. If indexes fall outside the image range return fail 0.
    int IndexFromWorldCoordinates(double w_t, double w_s, double w_r, double w_c, int &out_t, int &out_s, int &out_r, int &out_c);
    int IndexFromWorldCoordinates(double w_s, double w_r, double w_c, int &out_s, int &out_r, int &out_c);
        
    /// Reset the object to its intial state (just after construction).
    virtual void Reset();
    
    /// Set size, element type and properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
    void SetDimensionsAndPropertiesAs(mxRegularGrid<T> &grid);
    
    /// Set orientation.
    virtual void SetOrientation(double Cx, double Cy, double Cz, double Rx, double Ry, double Rz);
    
    /// Set origin.
    virtual void SetOrigin(double t, double s, double r, double c);
    
    /// Set spacing.
    virtual void SetSpacing(double t, double s, double r, double c);
    
    /// Set visual properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
    /// Note: size is not modified.
    void SetVisualizationPropertiesAs(mxRegularGrid<T> &grid);
    
    /// Calculate world coordinates for given indexes.
    /// Notice: indexes are 'int' values on purpose: they do not have to fall inside the image index range.
    int WorldCoordinatesFromIndexes(int t, int s, int r, int c, double &out_w_t, double &out_w_z, double &out_w_y, double &out_w_x);
    int WorldCoordinatesFromIndexes(int s, int r, int c, double &out_w_z, double &out_w_y, double &out_w_x);

    /// Assignment operator.
    mxRegularGrid<T>& operator =(mxRegularGrid<T> &g);
};




template<class T>
mxRegularGrid<T>::mxRegularGrid()
{
    this->m_orientation[0] = 1; this->m_orientation[1] = 0; this->m_orientation[2] = 0;
    this->m_orientation[3] = 0; this->m_orientation[4] = 1; this->m_orientation[5] = 0;
    this->m_orientation[6] = 0; this->m_orientation[7] = 0; this->m_orientation[8] = 1;
    this->m_origin[0] = this->m_origin[1] = this->m_origin[2] = this->m_origin[3] = 0;
    this->m_spacing[0] = this->m_spacing[1] = this->m_spacing[2] = this->m_spacing[3] = 1;
}


template<class T>
mxRegularGrid<T>::~mxRegularGrid()
{
	this->mxRegularGrid<T>::Reset();
}


template<class T>
int mxRegularGrid<T>::Copy(mxRegularGrid<T> *m)
{
    if(!this->mxMatrix<T>::Copy(m)) return 0;
    
    this->m_origin[0] = m->m_origin[0]; this->m_origin[1] = m->m_origin[1];
    this->m_origin[2] = m->m_origin[2]; this->m_origin[3] = m->m_origin[3];
    
    this->m_spacing[0] = m->m_spacing[0]; this->m_spacing[1] = m->m_spacing[1];
    this->m_spacing[2] = m->m_spacing[2]; this->m_spacing[3] = m->m_spacing[3];
    
    this->m_orientation[0] = m->m_orientation[0]; this->m_orientation[1] = m->m_orientation[1];
    this->m_orientation[2] = m->m_orientation[2]; this->m_orientation[3] = m->m_orientation[3];
    this->m_orientation[4] = m->m_orientation[4]; this->m_orientation[5] = m->m_orientation[5];
    this->m_orientation[6] = m->m_orientation[6]; this->m_orientation[7] = m->m_orientation[7];
    this->m_orientation[8] = m->m_orientation[8];
    
    return 1;
}


template<class T>
int mxRegularGrid<T>::Copy(mxMatrix<T> *matrix)
{
    return (this->mxMatrix<T>::Copy(matrix));
}


template<class T>
double* mxRegularGrid<T>::GetOrientation()
{
    return this->m_orientation;
}


template<class T>
void mxRegularGrid<T>::GetOrientation_C_Vector(double *Cx, double *Cy, double *Cz)
{
    *Cx = this->m_orientation[0];
    *Cy = this->m_orientation[1];
    *Cz = this->m_orientation[2];
}


template<class T>
void mxRegularGrid<T>::GetOrientation_R_Vector(double *Rx, double *Ry, double *Rz)
{
    *Rx = this->m_orientation[3];
    *Ry = this->m_orientation[4];
    *Rz = this->m_orientation[5];
}


template<class T>
void mxRegularGrid<T>::GetOrientation_S_Vector(double *Sx, double *Sy, double *Sz)
{
    *Sx = this->m_orientation[6];
    *Sy = this->m_orientation[7];
    *Sz = this->m_orientation[8];
}


template<class T>
double* mxRegularGrid<T>::GetOrigin()
{
    return this->m_origin;
}


template<class T>
double mxRegularGrid<T>::GetOrigin_C()
{
    return this->m_origin[0];
}


template<class T>
double mxRegularGrid<T>::GetOrigin_R()
{
    return this->m_origin[1];
}


template<class T>
double mxRegularGrid<T>::GetOrigin_S()
{
    return this->m_origin[2];
}


template<class T>
double mxRegularGrid<T>::GetOrigin_T()
{
    return this->m_origin[3];
}


template<class T>
double* mxRegularGrid<T>::GetSpacing()
{
    return this->m_spacing;
}


template<class T>
double mxRegularGrid<T>::GetSpacing_C()
{
    return this->m_spacing[0];
}


template<class T>
double mxRegularGrid<T>::GetSpacing_R()
{
    return this->m_spacing[1];
}


template<class T>
double mxRegularGrid<T>::GetSpacing_S()
{
    return this->m_spacing[2];
}


template<class T>
double mxRegularGrid<T>::GetSpacing_T()
{
    return this->m_spacing[3];
}


template<class T>
int mxRegularGrid<T>::IndexFromWorldCoordinates(double w_t, double w_z, double w_y, double w_x, int &out_t, int &out_s, int &out_r, int &out_c)
{
    // index = (world_coordinates - origin) / Orientation_matrix / spacing.
    
    //Orientation is calculated as follows:
    //   [ xr yr zr 1 ]
    // M=[ xc yc zc 1 ]
    //   [ xs ys zs 1 ]
    //   [ 0  0  0  1 ]
    // xs = (yr * zc) - (zr * yc)
    // ys = (zr * xc) - (xr * zc)
    // zs = (xr * yc) - (yr * xc)
    
    double xc = this->m_orientation[0]; double yc = this->m_orientation[1]; double zc = this->m_orientation[2];
    double xr = this->m_orientation[3]; double yr = this->m_orientation[4]; double zr = this->m_orientation[5];
    double xs = this->m_orientation[6]; double ys = this->m_orientation[7]; double zs = this->m_orientation[8];
    
    
    // Since M is orthogonal matrix, to get the inverse rotation, we just need to transpose it:
    //       [ xr xc xs 1 ]
    // M^T = [ yr yc ys 1 ]
    //       [ zr zc zs 1 ]
    //       [ 0  0  0  1 ]
    //
    //
    //       [ lx ]   [ xr*lx + xc*ly + xs*lz ]
    // M^T * [ ly ] = [ yr*lx + yc*ly + ys*lz ]
    //       [ lz ]   [ zr*lx + zc*ly + zs*lz ]
    //       [  1 ]   [            1          ]
    // where lx = w_x - origin_x, ly = w_y - origin_y, lz = w_z - origin_z.
    
    double lx = w_x - this->m_origin[0]; double ly = w_y - this->m_origin[1]; double lz = w_z - this->m_origin[2];
    
    double si_x = xr*lx + xc*ly + xs*lz; double si_y = yr*lx + yc*ly + ys*lz; double si_z = zr*lx + zc*ly + zs*lz;
    
    // Finally, the image index is calculated from si values:
    out_c = (int) (si_x / this->m_spacing[0]);
    out_r = (int) (si_y / this->m_spacing[1]);
    out_s = (int) (si_z / this->m_spacing[2]);
    out_t = (int) ((w_t - this->m_origin[3]) / this->m_spacing[3]);
    
    //    // Make absolute values out of the calculated ones - it could be that visualization planes have different positive/negative directions.
    //    if(out_s<0) out_s = -out_s;
    //    if(out_r<0) out_r = -out_r;
    //    if(out_c<0) out_c = -out_c;
    
    if(out_t>= ((int)this->GetNumberOfTimeSeries())) return 0;
    if(out_s>= ((int)this->GetNumberOfSlices())) return 0;
    if(out_r>= ((int)this->GetNumberOfRows())) return 0;
    if(out_c>= ((int)this->GetNumberOfColumns())) return 0;
    
    //Without orientation taken into account, the equations are:
    //  out_s = (int)((w_s - m_origin_CRST[2]) / m_spacing_CRST[2]);
    //  out_r = (int)((w_r - m_origin_CRST[1]) / m_spacing_CRST[1]);
    //  out_c = (int)((w_c - m_origin_CRST[0]) / m_spacing_CRST[0]);
    //	if(out_t<0 || out_t>= ((int)this->GetNumberOfTimeSeries())) return 0;
    //	if(out_s<0 || out_s>= ((int)this->GetNumberOfSlices())) return 0;
    //	if(out_r<0 || out_r>= ((int)this->GetNumberOfRows())) return 0;
    //	if(out_c<0 || out_c>= ((int)this->GetNumberOfColumns())) return 0;
    
    return 1;
}


template<class T>
int mxRegularGrid<T>::IndexFromWorldCoordinates(double w_s, double w_r, double w_c, int &out_s, int &out_r, int &out_c)
{
    int t;
    return this->IndexFromWorldCoordinates(this->GetOrigin_T(), w_s, w_r, w_c, t, out_s, out_r, out_c);
}


template<class T>
void mxRegularGrid<T>::Reset()
{
    this->m_orientation[0] = 1; this->m_orientation[1] = 0; this->m_orientation[2] = 0;
    this->m_orientation[3] = 0; this->m_orientation[4] = 1; this->m_orientation[5] = 0;
    this->m_orientation[6] = 0; this->m_orientation[7] = 0; this->m_orientation[8] = 1;
    this->m_origin[0] = this->m_origin[1] = this->m_origin[2] = this->m_origin[3] = 0;
    this->m_spacing[0] = this->m_spacing[1] = this->m_spacing[2] = this->m_spacing[3] = 1;
    
    this->mxMatrix<T>::Reset();
}


template<class T>
void mxRegularGrid<T>::SetDimensionsAndPropertiesAs(mxRegularGrid<T> &grid)
{
    this->SetDimensionsAs(grid);
    this->SetVisualizationPropertiesAs(grid);
}


template<class T>
void mxRegularGrid<T>::SetOrientation(double cx, double cy, double cz, double rx, double ry, double rz)
{
    this->m_orientation[0]=cx; this->m_orientation[1]=cy; this->m_orientation[2]=cz;
    this->m_orientation[3]=rx; this->m_orientation[4]=ry; this->m_orientation[5]=rz;
    this->m_orientation[6] = (ry * cz) - (rz * cy);
    this->m_orientation[7] = (rz * cx) - (rx * cz);
    this->m_orientation[8] = (rx * cy) - (ry * cx);
}


template<class T>
void mxRegularGrid<T>::SetOrigin(double t, double s, double r, double c)
{
    this->m_origin[0]=c;
    this->m_origin[1]=r;
    this->m_origin[2]=s;
    this->m_origin[3]=t;
}


template<class T>
void mxRegularGrid<T>::SetSpacing(double t, double s, double r, double c)
{
    this->m_spacing[0]=c;
    this->m_spacing[1]=r;
    this->m_spacing[2]=s;
    this->m_spacing[3]=t;
}


template<class T>
void mxRegularGrid<T>::SetVisualizationPropertiesAs(mxRegularGrid<T> &grid)
{
    this->mxRegularGrid<T>::SetSpacing(grid.GetSpacingTime(),grid.GetSpacingSlices(),grid.GetSpacingRows(),grid.GetSpacingColumns());
    this->mxRegularGrid<T>::SetOrigin(grid.GetOriginTime(),grid.GetOriginSlices(),grid.GetOriginRows(),grid.GetOriginColumns());
    this->mxRegularGrid<T>::SetOrientation(grid.m_orientation[0],grid.m_orientation[1],grid.m_orientation[2],grid.m_orientation[3],grid.m_orientation[4],grid.m_orientation[5]);
}


template<class T>
int mxRegularGrid<T>::WorldCoordinatesFromIndexes(int t, int s, int r, int c, double &out_w_t, double &out_w_z, double &out_w_y, double &out_w_x)
{
    // world_coordinates = origin  + Orientation_matrix * (index * spacing).
    //Orientation is calculated as follows:
    //   [ xr yr zr Tx ]
    // M=[ xc yc zc Ty ]
    //   [ xs ys zs Tz ]
    //   [ 0  0  0  1  ]
    // xs = (yr * zc) - (zr * yc)
    // ys = (zr * xc) - (xr * zc)
    // zs = (xr * yc) - (yr * xc)
    
    double xc = this->m_orientation[0]; double yc = this->m_orientation[1]; double zc = this->m_orientation[2];
    double xr = this->m_orientation[3]; double yr = this->m_orientation[4]; double zr = this->m_orientation[5];
    double xs = this->m_orientation[6]; double ys = this->m_orientation[7]; double zs = this->m_orientation[8];
    
    
    // World Position:
    //       [ xr yr zr 0 ]   [lx]   [ xr*lx + yr*ly + zr*lz ]
    // R   = [ xc yc zc 0 ] * [ly] = [ xc*lx + yc*ly + zc*lz ]
    //       [ xs ys zs 0 ]   [lz]   [ xs*lx + ys*ly + zs*lz ]
    //       [ 0  0  0  1 ]   [ 0]   [          0            ]
    //
    // where lx = c * spacing_x, ly = r * spacing_y, lz = s * spacing_z.
    
    double lx = c * this->m_spacing[0];
    double ly = r * this->m_spacing[1];
    double lz = s * this->m_spacing[2];
    
    out_w_x = this->m_origin[0] + xr*lx + yr*ly + zr*lz;
    out_w_y = this->m_origin[1] + xc*lx + yc*ly + zc*lz;
    out_w_z = this->m_origin[2] + xs*lx + ys*ly + zs*lz;
    out_w_t = this->m_origin[3] + t * this->m_spacing[3];
    
    return 1;
}


template<class T>
int mxRegularGrid<T>::WorldCoordinatesFromIndexes(int s, int r, int c, double &out_w_z, double &out_w_y, double &out_w_x)
{
    double w_t;
    return this->WorldCoordinatesFromIndexes(0, s, r, c, w_t, out_w_z, out_w_y, out_w_x);
}


template<class T>
mxRegularGrid<T>& mxRegularGrid<T>::operator =(mxRegularGrid<T> &m)
{
    if (&m==this) return *this;
    this->mxRegularGrid<T>::Copy(m);
    return *this;
}



#endif
