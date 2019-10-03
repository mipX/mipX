/*=========================================================================
 
 Program:   mipx
 Module:    xmxLoaderNifti.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "xmxLoaderNifti.h"



xmxLoaderNifti::xmxLoaderNifti()
{
//    this->m_class_name.Assign("xmxLoaderNifti");
//    this->m_label_for_X_axis.Assign("x");
//    this->m_label_for_Y_axis.Assign("f(x)");
    
}


xmxLoaderNifti::~xmxLoaderNifti()
{
//    this->Reset();
}


int xmxLoaderNifti::LoadImage(const char *file_name, mxImage *image)
{
    nifti_image *nim = NULL;

//    char *fin = NULL, *fout=NULL;
//
//    int ac;
//
//    if( argc < 2 ) return show_help();   /* typing '-help' is sooo much work */
//
//    /* process user options: 4 are valid presently */
//    for( ac = 1; ac < argc; ac++ ) {
//        if( ! strncmp(argv[ac], "-h", 2) ) {
//            return show_help();
//        }
//        else if( ! strcmp(argv[ac], "-input") ) {
//            if( ++ac >= argc ) {
//                fprintf(stderr, "** missing argument for -input\n");
//                return 1;
//            }
//            fin = argv[ac];  /* no string copy, just pointer assignment */
//        }
//        else if( ! strcmp(argv[ac], "-output") ) {
//            if( ++ac >= argc ) {
//                fprintf(stderr, "** missing argument for -output\n");
//                return 2;
//            }
//            fout = argv[ac];
//        }
//        else if( ! strcmp(argv[ac], "-verb") ) {
//            if( ++ac >= argc ) {
//                fprintf(stderr, "** missing argument for -verb\n");
//                return 2;
//            }
//            nifti_set_debug_level(atoi(argv[ac]));
//        }
//        else {
//            fprintf(stderr,"** invalid option, '%s'\n", argv[ac]);
//            return 1;
//        }
//    }
//
//    if( !fin  ) { fprintf(stderr, "** missing option '-input'\n");  return 1; }
//    if( !fout ) { fprintf(stderr, "** missing option '-output'\n"); return 1; }
    
    /* read input dataset, including data */
    nim = nifti_image_read(file_name, 1);
    
    if( !nim )
    {
        std::cout<<std::endl<<"xmxLoaderNifti::LoadImage(): failed to read NIfTI image from '"<<file_name<<"'";
        return 0;
    }

    std::cout<<std::endl<<"xmxLoaderNifti::LoadImage(): loded NIfTI image from '"<<file_name<<"'";
    
//    /* assign nifti_image fname/iname pair, based on output filename
//     (request to 'check' image and 'set_byte_order' here) */
//    if( nifti_set_filenames(nim, fout, 1, 1) ) return 1;
//
//    /* if we get here, write the output dataset */
//    nifti_image_write( nim );
    
    std::cout<<std::endl<<" n of dims: "<<nim->dim[0];
    std::cout<<std::endl<<" dims: ";
    for(int i=0; i<nim->dim[0]; i++)
    {
        std::cout<<nim->dim[i+1]<<"  ";
    }
    std::cout<<std::endl<<" data type: "<<nim->datatype;
    std::cout<<std::endl<<" bytes per voxel: "<<nim->nbyper;
    std::cout<<std::endl<<" number of voxels: "<<nim->nvox;
    std::cout<<std::endl<<" origin: "<<nim->qoffset_x<<"  "<<nim->qoffset_y<<"  "<<nim->qoffset_z;
    std::cout<<std::endl<<" spacing: ";
    for(int i=0; i<nim->dim[0]; i++)
    {
        std::cout<<nim->pixdim[i+1]<<"  ";
    }
    
    
    int dimension_t = 1;
    
    
    image->SetDimensions(dimension_t, nim->dim[3], nim->dim[2], nim->dim[1]);
    image->SetOrigin(0, nim->qoffset_z, nim->qoffset_y, nim->qoffset_x);
    image->SetSpacing(1, nim->pixdim[3], nim->pixdim[2], nim->pixdim[1]);
    
    
    
    if(nim->nbyper == 1)// 1 byte for voxel value.
    {
        if(nim->datatype == 2)// unsigned char (8 bits)
        {
            uint8_t *data = static_cast<uint8_t*>(nim->data) ;
            int min = data[0];
            int max = data[0];
            for(int i=0; i<nim->dim[1]*nim->dim[2]*nim->dim[3]; ++i)
            {
                if(data[i] > max) max = data[i];
                if(data[i] < min) min = data[i];
            }
            std::cout<<std::endl<<" voxel min= "<<min<<", max= "<<max;
            
            //int range = max - min + 1;
            
            for(int i=0; i<nim->dim[1]*nim->dim[2]*nim->dim[3]; ++i)
            {
                //image->operator[](i) = ((int)data[i]) - min;
                image->operator[](i) = data[i];
            }
            
            // release memory.
            nifti_image_free( nim );
            
            return 1;
        }
        
        if(nim->datatype == 256)// signed char (8 bits)
        {
            // release memory.
            nifti_image_free( nim );
            
            return 1;

        }
    }
    
    if(nim->nbyper == 2)// 2 bytes for voxel value.
    {
        if(nim->datatype == 4) // signed short (16 bits)
        {
            // release memory.
            nifti_image_free( nim );
            
            return 1;

        }
        
        if(nim->datatype == 512) // unsigned short (16 bits)
        {
            uint16_t *data = static_cast<uint16_t*>(nim->data) ;
            int min = data[0];
            int max = data[0];
            for(int i=0; i<nim->dim[1]*nim->dim[2]*nim->dim[3]; ++i)
            {
                if(data[i] > max) max = data[i];
                if(data[i] < min) min = data[i];
            }
            std::cout<<std::endl<<" voxel min= "<<min<<", max= "<<max;
            
            //int range = max - min + 1;
            
            for(int i=0; i<nim->dim[1]*nim->dim[2]*nim->dim[3]; ++i)
            {
                //image->operator[](i) = ((int)data[i]) - min;
                image->operator[](i) = data[i];
            }
            
            // release memory.
            nifti_image_free( nim );
            
            return 1;

        }
    }
    
    
    if(nim->nbyper == 4)// 4 bytes for voxel value.
    {
        if(nim->datatype == 8) // signed int (32 bits)
        {
            // release memory.
            nifti_image_free( nim );
            
            return 1;
        }
        
        if(nim->datatype == 768) // unsigned int (32 bits)
        {
            uint32_t *data = static_cast<uint32_t*>(nim->data) ;
            int min = data[0];
            int max = data[0];
            for(int i=0; i<nim->dim[1]*nim->dim[2]*nim->dim[3]; ++i)
            {
                if(data[i] > max) max = data[i];
                if(data[i] < min) min = data[i];
            }
            std::cout<<std::endl<<" voxel min= "<<min<<", max= "<<max;
            
            //int range = max - min + 1;
            
            for(int i=0; i<nim->dim[1]*nim->dim[2]*nim->dim[3]; ++i)
            {
                //image->operator[](i) = ((int)data[i]) - min;
                image->operator[](i) = data[i];
            }
            
            // release memory.
            nifti_image_free( nim );
            
            return 1;
        }
        
        if(nim->datatype == 16) // float (32 bits)
        {
            // release memory.
            nifti_image_free( nim );
            
            return 1;
        }
    }
    
    nifti_image_free( nim );
    // return FAIL if it comes to here.
    return 0;
}



//void xmxLoaderNifti::SaveToMatlabFile(const char *file_name)
//{
//    if(!file_name) return;
//    
//    mxString file_name_with_path, extension, file_name_string;
//    file_name_string.Assign(file_name);
//    file_name_string.ExtractFileNameAndFileExtension(file_name_with_path, extension);
//    
//    if(extension!="m" && extension!="M")
//    {
//        file_name_string.Append(".m");
//    }
//    
//    // open file
//    std::ofstream file;
//    file.open(file_name_string.Get_C_String(), std::ios::binary);
//    
//    // file type
//    file<<"%Single Array File: "<<std::endl;
//    
//    // version
//    file<<"%v"<<std::endl;
//    file<<"%3.0"<<std::endl;
//    
//    // labels for axes
//    file<<"%l"<<std::endl;
//    file<<"%"<<this->GetLabelFor_X_Axis().Get_C_String()<<std::endl;
//    file<<"%"<<this->GetLabelFor_Y_Axis().Get_C_String()<<std::endl;
//    
//    // tags - this is legacy to support loading with previous version of the loader - not used in new version.
//    file<<"%t"<<std::endl;
//    file<<"%"<<" "<<std::endl;
//    
//    // X values array
//    file<<"values_x = [";
//    for(unsigned int s=0; s<this->GetNumberOfSamples()-1; s++)
//    {
//        std::ostringstream oss;
//        oss.precision(4);//std::numeric_limits<double>::digits);
//        oss << std::fixed << this->m_X_values[s];
//        std::string str = oss.str();
//        
////        file.showpoint;
////        file<<std::ofstream::showpoint;
////        file.precision(4);
////        file<<std::ofstream::fixed;
//
//        file<<str;//this->m_X_values[s];
//        file<<", ";
//    }
//    file<<this->m_X_values[this->GetNumberOfSamples()-1]<<"];"<<std::endl;
//    
//    // Y values array
//    file<<"values_y = [";
//    for(unsigned int s=0; s<this->GetNumberOfSamples()-1; s++)
//    {
//        std::ostringstream oss;
//        oss.precision(4);//std::numeric_limits<double>::digits);
//        oss << std::fixed << this->m_Y_values[s];
//        std::string str = oss.str();
//
//
//        file<<str;//this->m_Y_values[s];
//        file<<", ";
//    }
//    file<<this->m_Y_values[this->GetNumberOfSamples()-1]<<"];"<<std::endl;
//    
//    // close file.
//    file.close();
//}
//
//
