/*=========================================================================
 
 Program:   mipx
 Module:    xmxLoaderMatlab.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "xmxLoaderMatlab.h"



xmxLoaderMatlab::xmxLoaderMatlab()
{
}


xmxLoaderMatlab::~xmxLoaderMatlab()
{
}


int xmxLoaderMatlab::LoadImage(const char *file_name, const char *variable_name, mxImage *image)
{
    mat_t *mat;
    matvar_t *ing_matvar;
    
    int dimension_t = 1;
    int dimension_s = 1;
    int dimension_r = 1;
    int dimension_c = 1;
    
    mat = Mat_Open(file_name, MAT_ACC_RDONLY);
    if ( NULL == mat )
    {
        std::cout<<std::endl<<"xmxLoaderMatlab::LoadImage() error opening "<<file_name;
        return 0;
    }
    
    mxString var_name;
    //if(!variable_name) var_name.Assign(" ");
    //else var_name.Assign(variable_name);
    matvar_t *matvar;
    std::cout<<std::endl<<" variables: '";
    while ( (matvar = Mat_VarReadNextInfo(mat)) != NULL )
    {
        if(!variable_name)
        {
            var_name.Assign(matvar->name);
        }
        
        std::cout<<matvar->name<<"' ";
        Mat_VarFree(matvar);
        matvar = NULL;
    }
    
    ing_matvar = Mat_VarRead(mat,var_name.Get_C_String());
    if ( NULL == ing_matvar )
    {
        std::cout<<std::endl<<"xmxLoaderMatlab::LoadImage() error reading '"<<var_name.Get_C_String()<<"' variable info";
        Mat_Close(mat);
        return 0;
    }
    else
    {
        
        std::cout<<std::endl<<" reading variable: '"<<var_name.Get_C_String()<<"'";
        std::cout<<std::endl<<" data type: "<<ing_matvar->data_type;
        std::cout<<std::endl<<" class type: "<<ing_matvar->class_type;
        std::cout<<std::endl<<" number of bytes: "<<ing_matvar->nbytes;
        std::cout<<std::endl<<" data size: "<<ing_matvar->data_size;
        std::cout<<std::endl<<" rank: "<<ing_matvar->rank;
        std::cout<<std::endl<<" dims: ";
        if(ing_matvar->rank==4)
        {
            for(int i=0; i<ing_matvar->rank; i++)
            {
                std::cout<<ing_matvar->dims[i];
                if(i==0)
                {
                    dimension_t = ing_matvar->dims[i];
                    std::cout<<"=t  ";
                }
                if(i==1)
                {
                    dimension_c = ing_matvar->dims[i];
                    std::cout<<"=c  ";
                }
                if(i==2)
                {
                    dimension_r = ing_matvar->dims[i];
                    std::cout<<"=r  ";
                }
                if(i==3)
                {
                    dimension_s = ing_matvar->dims[i];
                    std::cout<<"=s  ";
                }
            }
        }
        if(ing_matvar->rank==3)
        {
            for(int i=0; i<ing_matvar->rank; i++)
            {
                std::cout<<ing_matvar->dims[i]<<"  ";
                if(i==0) dimension_c = ing_matvar->dims[i];
                if(i==1) dimension_r = ing_matvar->dims[i];
                if(i==2) dimension_s = ing_matvar->dims[i];
            }
        }
        unsigned xSize = ing_matvar->nbytes/ing_matvar->data_size ;
        std::cout<<std::endl<<" nbytes/data_size: "<<xSize;
        std::cout<<std::endl<<" data address: "<<ing_matvar->data;
        std::cout<<std::endl<<" internal: "<<ing_matvar->internal;
        
        
        
        if ( MAT_C_SINGLE == ing_matvar->class_type )
        {

            const float *data = static_cast<const float*>(ing_matvar->data) ;
            float min = data[0];
            float max = data[0];
            
            for(int i=0; i<xSize; i++) //ing_matvar->dims[0]*ing_matvar->dims[1]*ing_matvar->dims[2]; ++i)
            {
                if(data[i] > max) max = data[i];
                if(data[i] < min) min = data[i];
                //std::cout<<" "<<data[i];
            }

            float range = max - min + 1;

//            image->SetDimensions(dimension_t, dimension_s, dimension_r, dimension_c);
//            for(int i=0; i<dimension_t*dimension_s*dimension_r*dimension_c; ++i)
//            {
//                image->operator[](i) = (data[i] - min)/range * 65535.0;
//            }

//            // t,s,r,c
//            image->SetDimensions(dimension_t, dimension_s, dimension_r, dimension_c);
//            int i=0;
//            for(unsigned int t=0; t<image->GetDimension_T(); t++)
//            {
//                for(unsigned int s=0; s<image->GetDimension_S(); s++)
//                {
//                    for(unsigned int r=0; r<image->GetDimension_R(); r++)
//                    {
//                        for(unsigned int c=0; c<image->GetDimension_C(); c++)
//                        {
//                            (*image)(t,s,r,c) = (data[i] - min)/range * 65535.0;
//                            i++;
//                        }
//                    }
//                }
//            }
            
            
//            // s,t,r,c
//            image->SetDimensions(dimension_t, dimension_s, dimension_r, dimension_c);
//            int i=0;
//            for(unsigned int s=0; s<image->GetDimension_S(); s++)
//            {
//                for(unsigned int t=0; t<image->GetDimension_T(); t++)
//                {
//                    for(unsigned int r=0; r<image->GetDimension_R(); r++)
//                    {
//                        for(unsigned int c=0; c<image->GetDimension_C(); c++)
//                        {
//                            (*image)(t,s,r,c) = (data[i] - min)/range * 65535.0;
//                            i++;
//                        }
//                    }
//                }
//            }
            
            // s,r,c,t
            image->SetDimensions(dimension_t, dimension_s, dimension_r, dimension_c);
            int i=0;
            for(unsigned int s=0; s<image->GetDimension_S(); s++)
            {
                for(unsigned int r=0; r<image->GetDimension_R(); r++)
                {
                    for(unsigned int c=0; c<image->GetDimension_C(); c++)
                    {
                        for(unsigned int t=0; t<image->GetDimension_T(); t++)
                        {
                            (*image)(t,s,r,c) = (data[i] - min)/range * 65535.0;
                            i++;
                        }
                    }
                }
            }

            
            
        }
        
//        if ( MAT_C_SINGLE == ing_matvar->class_type )
//        {
//
//            const float *data = static_cast<const float*>(ing_matvar->data) ;
//            float min = data[0];
//            float max = data[0];
//            for(int i=0; i<ing_matvar->dims[0]*ing_matvar->dims[1]*ing_matvar->dims[2]; ++i)
//            {
//                if(data[i] > max) max = data[i];
//                if(data[i] < min) min = data[i];
//                //std::cout<<" "<<data[i];
//            }
//
//            float range = max - min + 1;
//
//            image->SetDimensions(1, dimension_r, dimension_s, dimension_c);// (1,s,r,c) (1,s,c,r) (1,r,s,c)
//            for(int i=0; i<dimension_s*dimension_r*dimension_c; i++)
//            {
//                image->operator[](i) = (data[i] - min)/range * 65535.0;
//            }
//
//        }
        
        if ( MAT_C_UINT16 == ing_matvar->class_type )
        {
            const uint16_t *data = static_cast<const uint16_t*>(ing_matvar->data) ;
            //const float *data = static_cast<const float*>(ing_matvar->data) ;
            uint16_t min = data[0];
            uint16_t max = data[0];
            for(int i=0; i<xSize; i++) //for(int i=0; i<ing_matvar->dims[0]*ing_matvar->dims[1]*ing_matvar->dims[2]; ++i)
            {
                if(data[i] > max) max = data[i];
                if(data[i] < min) min = data[i];
                //std::cout<<" "<<data[i];
            }
            
            double range = max - min + 1;
            
            image->SetDimensions(dimension_t, dimension_s, dimension_r, dimension_c);
            for(int i=0; i<dimension_t*dimension_s*dimension_r*dimension_c; ++i)
            {
                image->operator[](i) = ((double)(data[i] - min))/range * 65535.0;
            }
            
        }
        
        Mat_VarFree(ing_matvar);
    }
    Mat_Close(mat);
    
    return 1;

}


int xmxLoaderMatlab::SaveImage(const char *file_name, mxImage *image, const char *variable_name)
{
    if(!file_name) return 0;

    mxString file_name_with_path, extension, file_name_string;
    file_name_string.Assign(file_name);
    file_name_string.ExtractFileNameAndFileExtension(file_name_with_path, extension);

    if(extension!="mat" && extension!="MAT")
    {
        file_name_string.Append(".mat");
    }
    
    int rank = 1;
    if(image->GetDimension_R()>1) rank = 2;
    if(image->GetDimension_S()>1) rank = 3;
    if(image->GetDimension_T()>1) rank = 4;
    
    size_t dims[4];
    dims[0] = image->GetDimension_C();
    dims[1] = image->GetDimension_R();
    dims[2] = image->GetDimension_S();
    dims[3] = image->GetDimension_T();
    
    mat_t    *matfp;
    matvar_t *matvar = NULL;
    
    matfp = Mat_CreateVer(file_name_string.Get_C_String(), NULL, MAT_FT_DEFAULT);
    if(!matfp)
    {
        std::cout<<std::endl<<"xmxLoaderMatlab::SaveImage() error opening "<<file_name;
        return 0;
    }

    if(image->GetScalarSizeInBytes()==1)
    {
        matvar = Mat_VarCreate(variable_name, MAT_C_UINT8, MAT_T_UINT8, rank, dims, image->GetData(), 0);
    }
    if(image->GetScalarSizeInBytes()==2)
    {
        matvar = Mat_VarCreate(variable_name, MAT_C_UINT16, MAT_T_UINT16, rank, dims, image->GetData(), 0);
    }
    if(image->GetScalarSizeInBytes()==4)
    {
        matvar = Mat_VarCreate(variable_name, MAT_C_UINT32, MAT_T_UINT32, rank, dims, image->GetData(), 0);
    }

    if(!matvar)
    {
        std::cout<<std::endl<<"xmxLoaderMatlab::SaveImage() error creating variable '"<<variable_name<<"'";
        Mat_Close(matfp);
        return 0;
    }
    
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvar);

    Mat_Close(matfp);
    return 1;
}


