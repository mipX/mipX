/*=========================================================================
 
 Program:   mipx
 Module:    xmxLoaderDicom.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "xmxLoaderDicom.h"



xmxLoaderDicom::xmxLoaderDicom()
{
}


xmxLoaderDicom::~xmxLoaderDicom()
{
}


int xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(const char *directory_path)
{
    std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree";
    
    std::cout<<std::endl<<directory_path;
    
    if(!directory_path) return 0;
    
    m_path_directory.Assign(directory_path);
    
    if(m_path_directory[(m_path_directory.GetNumberOfCharacters()-1)] != mxString::PathSeparator())
        m_path_directory.Append(mxString::PathSeparator());
    
    
    mxString mipx_path_directory;
    mipx_path_directory.Assign(m_path_directory);
    mipx_path_directory.Append("mipx");
    m_mipx_path_directory.Assign(mipx_path_directory);
    m_mipx_path_directory.Append(mxString::PathSeparator());
    
    mxString mipx_path_directory_mipx_file;
    mipx_path_directory_mipx_file.Assign(mipx_path_directory);
    mipx_path_directory_mipx_file.Append(mxString::PathSeparator());
    mipx_path_directory_mipx_file.Append("mipx_dicom_info.mip");
    
    if(mxFileOperations::IsFileCreated(mipx_path_directory_mipx_file.Get_C_String()))
    {
        return 1;
    }
    
    
    // Go through all sub-directories and enter all files to a list.
    mxList< mxString > files_list;
    
    // This list will be filled and emptied along the way - when all directories are examined it will be empty.
    mxList< mxString > directories_list;
    mxString s;
    s.Assign("");
    directories_list.AddToEnd(s);//m_path_directory);
    
    while(!directories_list.IsEmpty())
    {
        mxString path_directory;
        path_directory.Assign(m_path_directory);
        path_directory.Append(directories_list.GetBeginElement());
        
        mxString path_directory_relative;
        path_directory_relative.Assign(directories_list.GetBeginElement());
        
        std::cout<<std::endl<<"path_directory='"<<path_directory<<"'";
        
        tinydir_dir dir;
        if(tinydir_open_sorted(&dir, path_directory.Get_C_String()) == -1)
        {
            std::cout<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): Error opening folder: '"<<path_directory<<"'";
            tinydir_close(&dir);
            return 0;
        }
        
        for(int i=0; i<=dir.n_files-1; i++)
        {
            tinydir_file file;
            if(tinydir_readfile_n(&dir, &file, i) == -1)
            {
                std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): Error getting file";
                tinydir_close(&dir);
                return 0;
            }
            else
            {
                if(file.is_dir)
                {
                    mxString test;
                    test.Assign(file.name);
                    if(test != "." && test != "..")
                    {
                        mxString name;
                        //name.Assign("<DIR> ");
                        name.Append(path_directory_relative);
                        name.Append(file.name);
                        name.Append(mxString::PathSeparator());
                        directories_list.AddToEnd(name);
                    }
                }
                else
                {
                    mxString s;
                    s.Assign(path_directory);
                    s.Append(file.name);
                    files_list.AddToEnd(s);
                }
                //cout<<file.path<<endl;
            }
        }
        
        directories_list.DeleteBegin();
    }
    
    mxList< mxString > patient_string_list;
    mxList< mxString > study_string_list;
    mxList< mxString > series_string_list;
    
    
    mxListIterator< mxString > it;
    for(it.SetToBegin( files_list ); it.IsValid(); it.MoveToNext())
    {
        std::cout<<std::endl<<it.GetElement();
        
        //proceed with loading the end file...
        {
            //----- Reading important tags from separate files -----
            gdcm::Reader reader;
            reader.SetFileName(it.GetElement().Get_C_String());
            
            if( !reader.Read() )
            {
                std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree() error: Could not read: "<<it.GetElement();
            }
            else
            {
                mxString patient_string;
                mxString study_string;
                mxString series_string;

                gdcm::File &file_dicom = reader.GetFile();
                gdcm::DataSet &ds = file_dicom.GetDataSet();
                
                
                //--- Patient ---
                {
                    // patient name tag
                    {
                        gdcm::Attribute<0x0010,0x0010> at;
                        at.SetFromDataSet( ds );
                        gdcm::Attribute<0x0010,0x0010>::ArrayType v = at.GetValue();
                        
                        if( v == "" ) std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): cannot read patient name tag (0x0010,0x0010).";
                        else
                        {
                            //patient_string.Assign("PATIENT_");
                            patient_string.Append(v.c_str());
                        }
                    }
                    // patient id tag
                    {
                        gdcm::Attribute<0x0010,0x0020> at;
                        at.SetFromDataSet( ds );
                        gdcm::Attribute<0x0010,0x0020>::ArrayType v = at.GetValue();
                        if( v == "" ) std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): cannot read patient ID tag (0x0010,0x0020).";
                        else
                        {
                            //patient_string.Append("_ID_");
                            patient_string.Append("_");
                            patient_string.Append(v.c_str());
                        }
                    }
                }
                //------
                
                //--- Study ---
                {
                    // study description tag
                    {
                        gdcm::Attribute<0x0008,0x0020> at;
                        at.SetFromDataSet( ds );
                        gdcm::Attribute<0x0008,0x0020>::ArrayType v = at.GetValue();
                        if( v == "" ) std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): cannot read study description tag (0x0008,0x0020).";
                        else
                        {
                            //study_string.Assign("STUDY_");
                            study_string.Append(v.c_str());
                        }
                    }
                    // study id tag
                    {
                        gdcm::Attribute<0x0008,0x1030> at;
                        at.SetFromDataSet( ds );
                        gdcm::Attribute<0x0008,0x1030>::ArrayType v = at.GetValue();
                        if( v == "" ) std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): cannot read study ID tag (0x0008,0x1030).";
                        else
                        {
                            //study_string.Append("_ID_");
                            study_string.Append("_");
                            study_string.Append(v.c_str());
                        }
                    }
                }
                //------
                
                //--- Series ---
                {
                    // series number
                    {
                        gdcm::Attribute<0x0020,0x0011> at;
                        at.SetFromDataSet( ds );
                        gdcm::Attribute<0x0020,0x0011>::ArrayType v = at.GetValue();
                        int series_number = v;
                        //series_number = at.GetValue();
                        //if(at.GetValue() == 0) std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): cannot read series description tag (0x0020,0x0011).";
                        //else
                        {
                            //series_string.Append("_");
                            series_string.AppendNumber(series_number);
                        }
                    }
                    // modality tag
                    {
                        gdcm::Attribute<0x0008,0x0060> at;
                        at.SetFromDataSet( ds );
                        gdcm::Attribute<0x0008,0x0060>::ArrayType v = at.GetValue();
                        if( v == "" ) std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): cannot read series modality tag (0x0008,0x0060).";
                        else
                        {
                            series_string.Append("_");
                            series_string.Append(v.c_str());
                        }
                    }
                    // series description
                    {
                        gdcm::Attribute<0x0008,0x103E> at;
                        at.SetFromDataSet( ds );
                        gdcm::Attribute<0x0008,0x103E>::ArrayType v = at.GetValue();
                        if( v == "" ) std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): cannot read series description tag (0x0008,0x103E).";
                        else
                        {
                            series_string.Append("_");
                            series_string.Append(v.c_str());
                        }
                    }
                    // series UID
                    {
                        gdcm::Attribute<0x0020,0x000E> at;
                        at.SetFromDataSet( ds );
                        gdcm::Attribute<0x0020,0x000E>::ArrayType v = at.GetValue();
                        if( v == "" ) std::cout<<std::endl<<"xmxLoaderDicom::DicomDirectoryToTextFileDirectoryTree(): cannot read series UID tag (0x0020,0x000E).";
                        else
                        {
                            series_string.Append("_");
                            series_string.Append(v.c_str());
                        }
                    }
                }
                //------
                
                patient_string_list.AddToEnd(patient_string);
                series_string_list.AddToEnd(series_string);
                study_string_list.AddToEnd(study_string);
                
                std::cout<<std::endl<<series_string;
                std::cout<<std::endl<<study_string;
                std::cout<<std::endl<<patient_string;
                

            }
        }
    }
    
//    mxString mipx_path_directory;
//    mipx_path_directory.Assign(m_path_directory);
//    mipx_path_directory.Append("mipx");
    
    //if(!mxFileOperations::IsFolderCreated( m_path_directory.Get_C_String() ))
    {
        //std::cout<<std::endl<<" ------ !!!!!!!! ------- ";
    
        mipx_path_directory.Append(mxString::PathSeparator());
        mxFileOperations::MakeDirectory(mipx_path_directory.Get_C_String());
    }
    
    {
        std::ofstream file;
        file.open(mipx_path_directory_mipx_file.Get_C_String(), std::ios::binary);
        file<<"%mipx_Dicom_File_List_Info"<<std::endl;
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        
        file<<"<creation_time>(";
        mxString s_time;
        s_time.Assign(std::ctime(&end_time));
        for(int i=0; i<s_time.GetNumberOfCharacters()-1; i++)
        {
            file<<s_time[i];
        }
        //file<<"<creation_time>("<<std::ctime(&end_time)<<")"<<std::endl;
        file<<")"<<std::endl;
        file.close();
    
    }
    
    
    //else
    //{
    //    m_path_directory.Append(mxString::PathSeparator());
    //}
    //mxFileOperations::MakeDirectory(m_path_directory.Get_C_String());
    
    mxListIterator< mxString > it_p, it_ser, it_stu;
    it.SetToBegin( files_list );
    it_p.SetToBegin( patient_string_list );
    it_ser.SetToBegin( series_string_list );
    it_stu.SetToBegin( study_string_list );
    for( ; it.IsValid() && it_p.IsValid() && it_stu.IsValid() && it_ser.IsValid(); it.MoveToNext(), it_p.MoveToNext(), it_stu.MoveToNext(), it_ser.MoveToNext() )
    {
        mxString path_patient;
        path_patient.Assign(mipx_path_directory);
        path_patient.Append(it_p.GetElement());
        
        
        mxString path_study;
        path_study.Assign(path_patient);
        path_study.Append(mxString::PathSeparator());
        path_study.Append(it_stu.GetElement());
        
        
        mxString path_series;
        path_series.Assign(path_study);
        path_series.Append(mxString::PathSeparator());
        path_series.Append(it_ser.GetElement());
        
        
        std::cout<<std::endl<<"path_patient= '"<<path_patient<<"'";
        std::cout<<std::endl<<"path_study= '"<<path_study<<"'";
        std::cout<<std::endl<<"path_series= '"<<path_series<<"'";
        
        //if(!mxFileOperations::IsFolderCreated( m_path_directory.Get_C_String() ))
        {
            //std::cout<<" 1 ";
            mxFileOperations::MakeDirectory(mipx_path_directory.Get_C_String());
        }
        //if(!mxFileOperations::IsFolderCreated( path_patient.Get_C_String() ))
        {
            //std::cout<<" 2 ";
            path_patient.Append(mxString::PathSeparator());
            mxFileOperations::MakeDirectory(path_patient.Get_C_String());
        }
        //if(!mxFileOperations::IsFolderCreated( path_study.Get_C_String() ))
        {
            //std::cout<<" 3 ";
            path_study.Append(mxString::PathSeparator());
            mxFileOperations::MakeDirectory(path_study.Get_C_String());
        }
        //if(!mxFileOperations::IsFolderCreated( path_series.Get_C_String() ))
        {
            //std::cout<<" 4 ";
            //path_series.Append(mxString::PathSeparator());
            //mxFileOperations::MakeDirectory(path_series.Get_C_String());
            
            mxString relative_path;
            it.GetElement().ExtractDifferenceString(m_path_directory, relative_path);
            
            std::cout<<std::endl<<"relative_path= '"<<relative_path<<"'";
            
            path_series.Append(".mip");
            
            std::ofstream file;
            if(mxFileOperations::IsFileCreated(path_series.Get_C_String()))
            {
                file.open(path_series.Get_C_String(), std::ios::app);
            }
            else
            {
                file.open(path_series.Get_C_String(), std::ios::binary);
                file<<"%mipx_Dicom_File_List"<<std::endl;
            }
            file<<relative_path.Get_C_String()<<std::endl;
            file.close();
            
            ////just for testing
            //mxImage image;
            //this->LoadSeriesFromDicomFileListFile(path_series.Get_C_String(), &image);
        }
    }
    
    return 1;

}








int xmxLoaderDicom::LoadSeriesFromDicomFileListFile(const char *file_name, mxImage *image)
{
    // There is an error when using " files->InsertNextValue()". The string does not get recorded.
    // Use instead: " files->SetNumberOfValues(); " to preset the number of strings in the array,
    // and " files->SetValue(i)" to change each value.
    
    
    std::ifstream file;
    file.open(file_name, std::ios::binary);
    if(!file)
    {
        std::cout<<std::endl<<"xmxLoaderDicom::LoadSeriesFromDicomFileListFile() Error: unable to open file: "<<file_name;
        return 0;
    }
    
    char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
    int line_size = 99999;
    
    if(!file.getline(text_line, line_size)) { file.close(); return 0; }
    if(text_line[0]!='%' || text_line[1]!='m' || text_line[2]!='i' || text_line[3]!='p' || text_line[4]!='x' || text_line[5]!='_' || text_line[6]!='D' ||
       text_line[7]!='i' || text_line[8]!='c' || text_line[9]!='o' || text_line[10]!='m' || text_line[11]!='_' || text_line[12]!='F' || text_line[13]!='i' ||
       text_line[14]!='l' || text_line[15]!='e' || text_line[16]!='_' || text_line[17]!='L' ||  text_line[18]!='i' ||  text_line[19]!='s' ||  text_line[20]!='t')
    { file.close(); return 0; }
    
    
    
    mxList< mxImageSliceTag > list_of_tags;
    
    mxList< mxString > list_of_files;
    
    //
    //    int found_error_while_reading_series = 0;
    
    
    while(file.getline(text_line, line_size))
    {
        mxString file_path;
        file_path.Assign(m_path_directory);
        file_path.Append(text_line);
        
        std::cout<<std::endl<<"file_path= '"<<file_path<<"'";
        
        list_of_files.AddToEnd(file_path);
        
        
        
        //----- Reading important tags from separate files -----
        mxImageSliceTag *tag = list_of_tags.AddNewToEnd();
        gdcm::ImageReader reader;
        reader.SetFileName(file_path.Get_C_String());
        if(!reader.Read())
        {
            std::cout<<std::endl<<"xmxLoaderDicom::LoadSeriesFromDicomFileListFile() error: cannot read image '"<<file_path<<"'";
            return 0;
            //names_of_series_that_did_not_load_successfully.append(item->text(0)); names_of_series_that_did_not_load_successfully.append("\n");
            //number_of_series_that_did_not_load_successfully++;
            //found_error_while_reading_series = 1;
            //break;
        }
        else
        {
            
            gdcm::File &file_dicom = reader.GetFile();
            gdcm::DataSet &ds = file_dicom.GetDataSet();
            
            //--- Image Position (Patient) ---
            {
                gdcm::Attribute<0x0020,0x0032> at;
                at.SetFromDataSet(ds);
                
                if( at.GetValue() == 0.0 )
                {
                    std::cout<<std::endl<<"xmxLoaderDicom::LoadSeriesFromDicomFileListFile(): cannot read image patient position tag (0x0020,0x0032).";
                }
                else
                {
                    mxString *value_string = tag->GetValue("0020","0032");
                    if(value_string)
                    {
                        mxString temp;
                        temp.AppendNumber(at[0]); value_string->Append(temp); value_string->Append(", ");
                        temp.AppendNumber(at[1]); value_string->Append(temp); value_string->Append(", ");
                        temp.AppendNumber(at[2]); value_string->Append(temp);
                    }
                }
            }
            
//            {
//
//                vtkSmartPointer<vtkDICOMImageReader> dr = vtkSmartPointer<vtkDICOMImageReader>::New();
//                //vtkSmartPointer<vtkDICOMReader> dr = vtkSmartPointer<vtkDICOMReader>::New();
//                // Read the files and get the meta data.
//                dr->SetFileName(file_path.Get_C_String());
//                dr->Update();
//                std::cout<<dr->GetPixelRepresentation();
//            }
            
            //--- Image Number ---
            {
                gdcm::Attribute<0x0020,0x0013> at;
                at.SetFromDataSet(ds);
                
                if( at.GetValue() == 0.0 )
                {
                    std::cout<<std::endl<<"xmxLoaderDicom::LoadSeriesFromDicomFileListFile(): cannot read image number tag (0x0020,0x0013).";
                }
                else
                {
                    mxString *value_string = tag->GetValue("0020","0013");
                    if(value_string)
                    {
                        value_string->AssignNumber(at.GetValue());
                    }
                }
            }
            
            //--- Image Trigger Time ---
            {
                gdcm::Attribute<0x0018,0x1060> at;
                at.SetFromDataSet(ds);
                
                if( at.GetValue() < 0.0 )
                {
                    std::cout<<std::endl<<"xmxLoaderDicom::LoadSeriesFromDicomFileListFile(): cannot read image trigger time tag (0x0018,0x1060).";
                }
                else
                {
                    mxString *value_string = tag->GetValue("0018","1060");
                    if(value_string)
                    {
                        value_string->AssignNumber(at.GetValue(),3);
                    }
                }
            }
            
            //--- Slice Location ---
            {
                gdcm::Attribute<0x0020,0x1041> at;
                at.SetFromDataSet(ds); //at.Set( ds );
                
                if( at.GetValue() == 0.0 )
                {
                    std::cout<<std::endl<<"xmxLoaderDicom::LoadSeriesFromDicomFileListFile(): cannot read image slice location (0x0020,0x1041).";
                }
                else
                {
                    mxString *value_string = tag->GetValue("0020","1041");
                    if(value_string)
                    {
                        value_string->AssignNumber(at.GetValue());
                    }
                }
            }
        }
        
        
    }
    
    
    
   
    
    
    
    // array of loaded images.
    //mxArray< mxImage > array_of_images; //mxArray< vtkSmartPointer< vtkImageData > > list_of_images;
    //array_of_images.SetNumberOfElements( list_of_files.GetNumberOfElements() );
    
    {
        int n_of_slices = list_of_files.GetNumberOfElements();
        int is_multislice_file = 0;
        mxListIterator< mxString > it;
        int i=0;
        for(it.SetToBegin(list_of_files), i=0; it.IsValid() && i<list_of_files.GetNumberOfElements(); it.MoveToNext(), i++)
        {
            gdcm::ImageReader reader;
            reader.SetFileName(it.GetElementAddress()->Get_C_String());
            if(!reader.Read())
            {
                std::cout<<std::endl<<"xmxLoaderDicom::LoadSeriesFromDicomFileListFile() error: cannot read image '"<<it.GetElementAddress()->Get_C_String()<<"'";
                return 0;
            }
            
            const gdcm::Image &gimage = reader.GetImage();
            gimage.Print( std::cout );
            
            
//            vtkSmartPointer<vtkDICOMImageReader> vtk_reader = vtkSmartPointer<vtkDICOMImageReader>::New();
//            vtk_reader->SetFileName(it.GetElementAddress()->Get_C_String());
//            vtk_reader->Update();
            
            if(i==0)
            {
                if(list_of_files.GetNumberOfElements()==1)
                {
                    if(gimage.GetDimensions()[2]>0)//if(vtk_reader->GetOutput()->GetDimensions()[2]>0)
                    {
                        n_of_slices = gimage.GetDimensions()[2]; // vtk_reader->GetOutput()->GetDimensions()[2];
                        is_multislice_file = 1;
                    }
                }
                
                image->SetDimensions(1, n_of_slices, gimage.GetDimensions()[1], gimage.GetDimensions()[0]);
                
                
                // Set image properties
                image->SetOrigin(0, gimage.GetOrigin()[2], gimage.GetOrigin()[1], gimage.GetOrigin()[0]);
                image->SetSpacing(1, gimage.GetSpacing()[2], gimage.GetSpacing()[1], gimage.GetSpacing()[0]);
            }
            
//            std::cout<<std::endl<<" pixel representation = "<<vtk_reader->GetPixelRepresentation();
//            std::cout<<std::endl<<" bits allocated = "<<vtk_reader->GetBitsAllocated();
//            std::cout<<std::endl<<" number of components = "<<vtk_reader->GetNumberOfComponents();
//            std::cout<<std::endl<<" patient position = "<<vtk_reader->GetImagePositionPatient()[0]<<", "<<vtk_reader->GetImagePositionPatient()[1]<<", "<<vtk_reader->GetImagePositionPatient()[2];
//            std::cout<<std::endl<<" pixel spacing = "<<vtk_reader->GetPixelSpacing()[0]<<", "<<vtk_reader->GetPixelSpacing()[1]<<", "<<vtk_reader->GetPixelSpacing()[2];
//            std::cout<<std::endl<<" descriptive name = "<<vtk_reader->GetDescriptiveName();
//            std::cout<<std::endl<<" transfer syntax uid = "<<vtk_reader->GetTransferSyntaxUID();
//            std::cout<<std::endl<<" patient name = "<<vtk_reader->GetPatientName();
//            std::cout<<std::endl<<" study id = "<<vtk_reader->GetStudyID();
//            std::cout<<std::endl<<" study uid = "<<vtk_reader->GetStudyUID();
            
            
            unsigned long len = gimage.GetBufferLength();
            char *buffer = new char[ len ];
            //char *buffer;
            gimage.GetBuffer(buffer);
            //gimage.GetIconImage().
            
            if( gimage.GetPhotometricInterpretation() == gdcm::PhotometricInterpretation::MONOCHROME2 )
            {
                if( gimage.GetPixelFormat() == gdcm::PixelFormat::UINT8 )
                {
                    
                    std::cout<<std::endl<<"Loading 8 bit values!";
                    
                    // loading UNSIGNED value representation
                    //if(vtk_reader->GetPixelRepresentation()==0)
                    {
                        std::cout<<std::endl<<"Loading unsigned representation!";
                        
                        int min = 255, max = 0;
                        if(is_multislice_file)
                        {
                            int i = 0;
                            for(int s=0; s<image->GetDimension_S(); s++)//for(int s=0; s<image->GetDimension_S(); s++)
                            {
                                for(int r=0; r<image->GetDimension_R(); r++)
                                {
                                    //std::cout<<" "<<r;
                                    for(int c=0; c<image->GetDimension_C(); c++)
                                    {
                                        
                                        //int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));//int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
                                        
                                        int v =  static_cast<uint8_t>(buffer[i]);
                                        
                                        if(min > v) min = v;
                                        if(max < v) max = v;
                                        
                                        i++;
                                    }
                                }
                            }
                        }
                        else
                        {
                            int i = 0;
                            //for(int s=0; s<image->GetDimension_S(); s++)
                            {
                                for(int r=0; r<image->GetDimension_R(); r++)
                                {
                                    //std::cout<<" "<<r;
                                    for(int c=0; c<image->GetDimension_C(); c++)
                                    {
                                        //int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
                                        
                                        
                                        int v =  static_cast<uint8_t>(buffer[i]);
                                        
                                        if(min > v) min = v;
                                        if(max < v) max = v;
                                        
                                        i++;
                                    }
                                }
                            }
                        }
                        
                        int range = max - min + 1;
                        
                        //double range2 = 2147483647-max - min + 1;
                        //int range = max - min + 1;
                        
                        std::cout<<std::endl<<"  min = "<<min<<",   max = "<<max<<",  range = "<<range;
                        if(is_multislice_file)
                        {
                            int i=0;
                            for(int s=0; s<image->GetDimension_S(); s++)//for(int s=0; s<image->GetDimension_S(); s++)
                            {
                                for(int r=0; r<image->GetDimension_R(); r++)
                                {
                                    for(int c=0; c<image->GetDimension_C(); c++)
                                    {
                                        //int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));;//int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
                                        
                                        int v =  static_cast<uint8_t>(buffer[i]);
                                        
                                        int temp = 255 * (v-min);
                                        
                                        image->operator()(s,r,c) = temp/range;//( ((double)(v-min))/((double)range))*255;
                                        
                                        i++;
                                    }
                                }
                            }
                        }
                        else
                        {
                            int i=0;
                            //for(int s=0; s<image->GetDimension_S(); s++)
                            {
                                for(int r=0; r<image->GetDimension_R(); r++)
                                {
                                    for(int c=0; c<image->GetDimension_C(); c++)
                                    {
                                        //int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
                                        
                                        int v =  static_cast<uint8_t>(buffer[i]);
                                        
                                        int temp = 255 * (v-min);
                                        
                                        image->operator()(i,r,c) = temp/range;//( ((double)(v-min))/((double)range))*255;
                                        
                                        i++;
                                        
                                    }
                                }
                            }
                        }
                    }
                }
                //else if( gimage.GetPixelFormat() == gdcm::PixelFormat::INT16 )
                else if( gimage.GetPixelFormat() == gdcm::PixelFormat::UINT16 )
                {
                    //*pubuffer++ = (unsigned char)std::min(255, (32768 + *buffer16) / 255);
                    
                    std::cout<<std::endl<<"Loading 16 bit values!";
                    
                    short *buffer16 = (short*)buffer;
                    
                    // loading UNSIGNED value representation
                    //if(vtk_reader->GetPixelRepresentation()==0)
                    {
                        std::cout<<std::endl<<"Loading unsigned representation!";
                        
                        double min = 999999, max=-999999;
                        
                        if(is_multislice_file)
                        {
                            int i=0;
                            for(int s=0; s<image->GetDimension_S(); s++)
                            {
                                for(int r=0; r<image->GetDimension_R(); r++)
                                {
                                    //std::cout<<" "<<r;
                                    for(int c=0; c<image->GetDimension_C(); c++)
                                    {
                                        
//                                        //double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
//                                        //int v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
//                                        int v =  *(static_cast<int16_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));
                                        
                                        int v = static_cast<int16_t>(buffer16[i]);
                                        
                                        if(min > v) min = v;
                                        if(max < v) max = v;
                                        
                                        i++;
                                    }
                                }
                            }
                        }
                        else
                        {
                            int i=0;
                            //for(int s=0; s<image->GetDimension_S(); s++)
                            {
                                for(int r=0; r<image->GetDimension_R(); r++)
                                {
                                    //std::cout<<" "<<r;
                                    for(int c=0; c<image->GetDimension_C(); c++)
                                    {
                                        
                                        ////double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
                                        ////int v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
                                        //int v =  *(static_cast<int16_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
                                        
                                        int v = static_cast<int16_t>(buffer16[i]);
                                        
                                        if(min > v) min = v;
                                        if(max < v) max = v;
                                        
                                        i++;
                                    }
                                }
                            }
                        }
                        
                        double range = max - min + 1;
                        
                        //double range2 = 2147483647-max - min + 1;
                        //int range = max - min + 1;
                        
                        std::cout<<std::endl<<"  min = "<<min<<",   max = "<<max<<",  range = "<<range;
                        
                        if(is_multislice_file)
                        {
                            int i=0;
                            for(int s=0; s<image->GetDimension_S(); s++)
                            {
                                for(int r=0; r<image->GetDimension_R(); r++)
                                {
                                    for(int c=0; c<image->GetDimension_C(); c++)
                                    {
                                        ////double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
                                        //double v =  *(static_cast<int16_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));;
                                        
                                        double v = static_cast<int16_t>(buffer16[i]);
                                        
                                        image->operator()(s,r,c) = ( ((double)(v-min))/((double)range))*65535;
                                        
                                        i++;
                                    }
                                }
                            }
                        }
                        else
                        {
                            int i=0;
                            //for(int s=0; s<image->GetDimension_S(); s++)
                            {
                                for(int r=0; r<image->GetDimension_R(); r++)
                                {
                                    for(int c=0; c<image->GetDimension_C(); c++)
                                    {
                                        ////double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
                                        //double v =  *(static_cast<int16_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
                                        
                                        double v = static_cast<int16_t>(buffer16[i]);
                                        
                                        image->operator()(i,r,c) = ( ((double)(v-min))/((double)range))*65535;
                                        
                                        i++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        
//            // Loading 8 bit values
//            if(vtk_reader->GetBitsAllocated()==8)
//            {
//                std::cout<<std::endl<<"Loading 8 bit values!";
//
//                // loading UNSIGNED value representation
//                //if(vtk_reader->GetPixelRepresentation()==0)
//                {
//                    std::cout<<std::endl<<"Loading unsigned representation!";
//
//                    int min = 255, max = 0;
//                    if(is_multislice_file)
//                    {
//                        for(int s=0; s<image->GetDimension_S(); s++)//for(int s=0; s<image->GetDimension_S(); s++)
//                        {
//                            for(int r=0; r<image->GetDimension_R(); r++)
//                            {
//                                //std::cout<<" "<<r;
//                                for(int c=0; c<image->GetDimension_C(); c++)
//                                {
//                                    int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));//int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
//
//                                    if(min > v) min = v;
//                                    if(max < v) max = v;
//                                }
//                            }
//                        }
//                    }
//                    else
//                    {
//                        //for(int s=0; s<image->GetDimension_S(); s++)
//                        {
//                            for(int r=0; r<image->GetDimension_R(); r++)
//                            {
//                                //std::cout<<" "<<r;
//                                for(int c=0; c<image->GetDimension_C(); c++)
//                                {
//                                    int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
//
//                                    if(min > v) min = v;
//                                    if(max < v) max = v;
//                                }
//                            }
//                        }
//                    }
//
//                    int range = max - min + 1;
//
//                    //double range2 = 2147483647-max - min + 1;
//                    //int range = max - min + 1;
//
//                    std::cout<<std::endl<<"  min = "<<min<<",   max = "<<max<<",  range = "<<range;
//                    if(is_multislice_file)
//                    {
//                        for(int s=0; s<image->GetDimension_S(); s++)//for(int s=0; s<image->GetDimension_S(); s++)
//                        {
//                            for(int r=0; r<image->GetDimension_R(); r++)
//                            {
//                                for(int c=0; c<image->GetDimension_C(); c++)
//                                {
//                                    int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));;//int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
//
//                                    int temp = 255 * (v-min);
//
//                                    image->operator()(s,r,c) = temp/range;//( ((double)(v-min))/((double)range))*255;
//
//                                }
//                            }
//                        }
//                    }
//                    else
//                    {
//                        //for(int s=0; s<image->GetDimension_S(); s++)
//                        {
//                            for(int r=0; r<image->GetDimension_R(); r++)
//                            {
//                                for(int c=0; c<image->GetDimension_C(); c++)
//                                {
//                                    int v =  *(static_cast<uint8_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
//
//                                    int temp = 255 * (v-min);
//
//                                    image->operator()(i,r,c) = temp/range;//( ((double)(v-min))/((double)range))*255;
//
//                                }
//                            }
//                        }
//                    }
//                }
//
//                // loading SIGNED value representation
//                //if(vtk_reader->GetPixelRepresentation()==1)
//                {
//                }
//            }
//
//
//            DICOMAppHelper ah;
//            DICOMParser dp;
//            //dp.
//            //ah.
//
//            //vtk_reader->Parser->
//
//
//
//
//            if(vtk_reader->GetBitsAllocated()==16)
//            {
//                std::cout<<std::endl<<"Loading 16 bit values!";
//
//                // loading UNSIGNED value representation
//                //if(vtk_reader->GetPixelRepresentation()==0)
//                {
//                    std::cout<<std::endl<<"Loading unsigned representation!";
//
//                    double min = 999999, max=-999999;
//
//                    if(is_multislice_file)
//                    {
//                        for(int s=0; s<image->GetDimension_S(); s++)
//                        {
//                            for(int r=0; r<image->GetDimension_R(); r++)
//                            {
//                                //std::cout<<" "<<r;
//                                for(int c=0; c<image->GetDimension_C(); c++)
//                                {
//
//                                    //double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
//                                    //int v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
//                                    int v =  *(static_cast<int16_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));
//                                    if(min > v) min = v;
//                                    if(max < v) max = v;
//                                }
//                            }
//                        }
//                    }
//                    else
//                    {
//                        //for(int s=0; s<image->GetDimension_S(); s++)
//                        {
//                            for(int r=0; r<image->GetDimension_R(); r++)
//                            {
//                                //std::cout<<" "<<r;
//                                for(int c=0; c<image->GetDimension_C(); c++)
//                                {
//
//                                    //double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
//                                    //int v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
//                                    int v =  *(static_cast<int16_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));
//                                    if(min > v) min = v;
//                                    if(max < v) max = v;
//                                }
//                            }
//                        }
//                    }
//
//                    double range = max - min + 1;
//
//                    //double range2 = 2147483647-max - min + 1;
//                    //int range = max - min + 1;
//
//                    std::cout<<std::endl<<"  min = "<<min<<",   max = "<<max<<",  range = "<<range;
//
//                    if(is_multislice_file)
//                    {
//                        for(int s=0; s<image->GetDimension_S(); s++)
//                        {
//                            for(int r=0; r<image->GetDimension_R(); r++)
//                            {
//                                for(int c=0; c<image->GetDimension_C(); c++)
//                                {
//                                    //double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
//                                    double v =  *(static_cast<int16_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));;
//
//                                    image->operator()(s,r,c) = ( ((double)(v-min))/((double)range))*65535;
//                                }
//                            }
//                        }
//                    }
//                    else
//                    {
//                        //for(int s=0; s<image->GetDimension_S(); s++)
//                        {
//                            for(int r=0; r<image->GetDimension_R(); r++)
//                            {
//                                for(int c=0; c<image->GetDimension_C(); c++)
//                                {
//                                    //double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
//                                    double v =  *(static_cast<int16_t *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, 0)));;
//
//                                    image->operator()(i,r,c) = ( ((double)(v-min))/((double)range))*65535;
//                                }
//                            }
//                        }
//                    }
//                }
//
//                // loading SIGNED value representation
//                //if(vtk_reader->GetPixelRepresentation()==1)
//                {
//                }
//            }
//
//
//
        }
    }
    
    
    {
        // Copy tags
        mxListIterator< mxImageSliceTag > it;
        int i=0;
        for(it.SetToBegin(list_of_tags), i=0; it.IsValid() && i<image->GetTag()->m_tags.GetNumberOfDataElements(); it.MoveToNext(), i++)
        {
            image->GetTag()->GetTag(i) = it.GetElement();
        }
    }
    
    
    
    
    
    
    //    vtkSmartPointer< vtkStringArray > files = vtkSmartPointer< vtkStringArray >::New();
    //
    //    // Copy the list of files into vtk array
    //    //files->SetNumberOfValues(list_of_files.GetNumberOfElements());
    //    mxListIterator< mxString > it;
    //    int i=0;
    //    for(it.SetToBegin(list_of_files), i=0; it.IsValid() && i<list_of_files.GetNumberOfElements(); it.MoveToNext(), i++)
    //    {
    //        //files->SetValue(i, it.GetElementAddress()->Get_C_String());
    //        files->InsertNextValue(it.GetElementAddress()->Get_C_String());
    //    }
    //
    //    std::cout<<std::endl<<" files->GetNumberOfValues= "<<files->GetNumberOfValues();
    //    for( i=0; i<files->GetNumberOfValues(); i++)
    //    {
    //        std::cout<<std::endl<<" files["<<i<<"]='"<<files->GetValue(i)<<"'";
    //    }
    //
    //
    //    //if(!found_error_while_reading_series)
    //    {
    //        //vtkSmartPointer<vtkGDCMImageReader2> vtk_reader = vtkSmartPointer<vtkGDCMImageReader2>::New();
    //
    //        vtkSmartPointer<vtkDICOMImageReader> vtk_reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    //
    //        //vtk_reader->SetFileNames(files);
    //        //vtk_reader->SetFileName("/Users/danilobabin/Dropbox/-DIP_IMAGES/-CORONARY_FFR_PWV/pt1/coronarography/DICOM/18033018/33470000/73708009");//files->GetValue(i));
    //        //vtk_reader->SetFileName(list_of_files.GetBeginElementAddress()->Get_C_String());// files->GetValue(i));
    //        vtk_reader->SetFileName("/Users/danilobabin/-Dip_Images/prostate.IMG");
    //        vtk_reader->Update();
    //
    //        // Copy image
    //        image->SetDimensions(1, vtk_reader->GetOutput()->GetDimensions()[2], vtk_reader->GetOutput()->GetDimensions()[1], vtk_reader->GetOutput()->GetDimensions()[0]);
    //        //        double *ptr = static_cast<double *>(vtk_reader->GetOutput()->GetScalarPointer(0, 0, 0));
    //        //        for(int i=0; i<image->GetNumberOfDataElements(); i++)
    //        //        {
    //        //            (*image)[i] = ptr[i];
    //        //        }
    //
    //        for(int s=0; s<image->GetDimension_S(); s++)
    //        {
    //            for(int r=0; r<image->GetDimension_R(); r++)
    //            {
    //                for(int c=0; c<image->GetDimension_C(); c++)
    //                {
    //                    image->operator()(s,r,c) = *(static_cast<double *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));
    //                }
    //            }
    //        }
    //
    //        // Set image properties
    //        image->SetOrigin(0, vtk_reader->GetOutput()->GetOrigin()[2], vtk_reader->GetOutput()->GetOrigin()[1], vtk_reader->GetOutput()->GetOrigin()[0]);
    //        image->SetSpacing(1, vtk_reader->GetOutput()->GetSpacing()[2], vtk_reader->GetOutput()->GetSpacing()[1], vtk_reader->GetOutput()->GetSpacing()[0]);
    //
    //
    //        // Copy tags
    //        mxListIterator< mxImageSliceTag > it;
    //        int i=0;
    //        for(it.SetToBegin(list_of_tags), i=0; it.IsValid() && i<image->GetTag()->m_tags.GetNumberOfDataElements(); it.MoveToNext(), i++)
    //        {
    //            image->GetTag()->GetTag(i) = it.GetElement();
    //        }
    //
    //    }
    
    file.close();
    
    return 1;
}
