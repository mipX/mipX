/*=========================================================================
 
 Program:   mipx
 Module:    mxCurve.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxCurve.h"



mxCurve::mxCurve()
{
    this->m_class_name.Assign("mxCurve");
    this->m_label_for_X_axis.Assign("x");
    this->m_label_for_Y_axis.Assign("f(x)");
}


mxCurve::~mxCurve()
{
	this->Reset();
}


int mxCurve::Copy(mxCurve *curve)
{
    if(!curve) return 0;
    if(this==curve) return 1;
    
    this->Reset();
    
    this->mxDataObject::CopyFromDataObject(curve);
    
    this->m_label_for_X_axis.Assign(curve->m_label_for_X_axis);
    this->m_label_for_Y_axis.Assign(curve->m_label_for_Y_axis);
    
    this->m_X_values.CopyFrom(curve->m_X_values);
    this->m_Y_values.CopyFrom(curve->m_Y_values);

    return 1;
}


int mxCurve::FillIn_X_ValuesAscending(double x0_included, double delta_x)
{
    if(this->m_X_values.IsEmpty()) return 0;
    
    this->m_X_values[0] = x0_included;
    for(unsigned int s=1; s<this->m_X_values.GetNumberOfElements(); s++)
    {
        this->m_X_values[s] = this->m_X_values[s-1] + delta_x;
    }
    return 1;
}


mxArray< double >* mxCurve::Get_X_Values()
{
    return &(this->m_X_values);
}


mxArray< double >* mxCurve::Get_Y_Values()
{
    return &(this->m_Y_values);
}


mxString mxCurve::GetLabelFor_X_Axis()
{
    return this->m_label_for_X_axis;
}


mxString mxCurve::GetLabelFor_Y_Axis()
{
    return this->m_label_for_Y_axis;
}


int mxCurve::GetMaximum_Y_Value(double &output_max_y_value, unsigned int &output_index_of_max_y_value)
{
    if(this->IsEmpty()) return 0;
    
    output_max_y_value = this->Y(0);
    for(unsigned int s=0; s<this->GetNumberOfSamples(); s++)
    {
        if(this->Y(s) > output_max_y_value)
        {
            output_max_y_value = this->Y(s);
            output_index_of_max_y_value = s;
        }
    }
    return 1;
}


int mxCurve::GetMinimum_Y_Value(double &output_min_y_value, unsigned int &output_index_of_min_y_value)
{
    if(this->IsEmpty()) return 0;
    
    output_min_y_value = this->Y(0);
    for(unsigned int s=0; s<this->GetNumberOfSamples(); s++)
    {
        if(this->Y(s) < output_min_y_value)
        {
            output_min_y_value = this->Y(s);
            output_index_of_min_y_value = s;
        }
    }
    return 1;
}



unsigned int mxCurve::GetNumberOfSamples()
{
    return this->m_Y_values.GetNumberOfElements();
}


int mxCurve::GetRangeOf_X_Values(double &x_min, double &x_max)
{
    if(this->IsEmpty()) { return 0; }
    
    x_min = x_max = this->X(0);
    for(unsigned int s = 1; s < this->GetNumberOfSamples(); s++)
    {
        if(this->X(s) < x_min) x_min = this->X(s);
        if(this->X(s) > x_max) x_max = this->X(s);
    }
    return 1;
}


int mxCurve::GetRangeOf_Y_Values(double &y_min, double &y_max)
{
    if(this->IsEmpty()) { return 0; }
    
    y_min = y_max = this->Y(0);
    for(unsigned int s = 1; s < this->GetNumberOfSamples(); s++)
    {
        if(this->Y(s) < y_min) y_min = this->Y(s);
        if(this->Y(s) > y_max) y_max = this->Y(s);
    }
    return 1;
}


unsigned int mxCurve::IndexOf_X_ValueInAscendingOrder(double x_value)
{
    for(unsigned int s=0; s<this->GetNumberOfSamples(); s++)
    {
        if(this->X(s) > x_value) { return s; }
    }
    return 0;
}


int mxCurve::IsEmpty()
{
    if( this->m_Y_values.IsEmpty() || (this->m_Y_values.GetNumberOfElements() != this->m_X_values.GetNumberOfElements()) ) { return 1; }
    return 0;
}


int mxCurve::Load_Legacy_SingleArrayFile_v2(const char *file_name)
{
    std::ifstream input_file;
    input_file.open(file_name,std::ios::binary);
    if(!input_file)
    {
        std::cout<<"bdCurveXY::LoadSingleArrayFile_v2() : Unable to open single array file: "<<file_name<<std::endl;
        return 0;
    }
    
    
    char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
    unsigned int amount_of_characters_to_load = 100000;
    
    if(!input_file.getline(text_line, amount_of_characters_to_load)) return 0;
    if(text_line[0]!='%' || text_line[1]!='S' || text_line[2]!='i' || text_line[3]!='n' || text_line[4]!='g' || text_line[5]!='l' || text_line[6]!='e' ||
       text_line[7]!=' ' || text_line[8]!='A' || text_line[9]!='r' || text_line[10]!='r' || text_line[11]!='a' || text_line[12]!='y' ||
       text_line[13]!=' ' || text_line[14]!='F' || text_line[15]!='i' || text_line[16]!='l' || text_line[17]!='e' ||  text_line[18]!=':')
    { input_file.close(); return 0; }
    
    // load version
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='%' && text_line[1]!='v') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='%') { input_file.close(); return 0; }
    mxString bds;
    bds.Append(&(text_line[1]));
    mxList<int> list;
    bds.ExtractNumbers(list);
    if(list.IsEmpty())
    {
        std::cout<<"bdCurveXY::LoadSingleArrayFile_v2(): No version value found!"<<std::endl;
        input_file.close();
        return 0;
    }
    if(list.GetBeginElement()>2)
    {
        std::cout<<"bdCurveXY::LoadSingleArrayFile_v2(): Version higher than supported, version: "<<list.GetBeginElement()<<std::endl;
        input_file.close();
        return 0;
    }
    
//    // read object tag.
//    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
//    if(text_line[0]!='%' && text_line[1]!='t') { input_file.close(); return 0; }
//    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
//    if(text_line[0]!='%') { input_file.close(); return 0; }
//    this->LoadTags(&(text_line[1]));
    
    // skip tag reading - check for the existing fields, but perform nothing.
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='%' && text_line[1]!='t') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='%') { input_file.close(); return 0; }
    
    
    
    //Load X array
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(!mxArrayFunctions::LoadFromStringOfDoubleValues(*(this->Get_X_Values()), text_line, amount_of_characters_to_load))
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v2(): error reading X array!"<<std::endl;
        std::cout<<"text_buffer: "<<text_line<<std::endl;
        this->Reset();
        input_file.close();
        return 0;
    }

//    if(!this->m_X_values. LoadFromString_DoubleValues(text_line))
//    {
//        cout<<"bdCurveXY::LoadSingleArrayFile_v2(): error loading X array! File name: "<<file_name<<endl;
//        cout<<"text_line = "<<text_line<<endl;
//        m_x.Reset();
//        m_y.Reset();
//        input_file.close();
//        return 0;
//    }
    
    //Load Y array
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(!mxArrayFunctions::LoadFromStringOfDoubleValues(*(this->Get_Y_Values()), text_line, amount_of_characters_to_load))
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v2(): error reading Y array!"<<std::endl;
        std::cout<<"text_buffer: "<<text_line<<std::endl;
        this->Reset();
        input_file.close();
        return 0;
    }
//    if(!m_y.LoadFromString_DoubleValues(text_line))
//    {
//        cout<<"bdCurveXY::LoadSingleArrayFile_v2(): error loading Y array! File name: "<<file_name<<endl;
//        cout<<"text_line = "<<text_line<<endl;
//        //Delete both arrays
//        m_x.Reset();
//        m_y.Reset();
//        input_file.close();
//        return 0;
//    }
    
    if(this->Get_X_Values()->GetNumberOfElements() != this->Get_Y_Values()->GetNumberOfElements())
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v2(): error: unequal size of X and Y values arrays, size_of_X="<<this->Get_X_Values()->GetNumberOfElements()<<", size_of_Y="<<this->Get_Y_Values()->GetNumberOfElements()<<std::endl;
        this->Reset();
        input_file.close();
        return 0;
    }
    
    if(this->IsEmpty())
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v2(): error: loaded curve is empty!"<<std::endl;
        input_file.close();
        return 0;
    }
    
    input_file.close();
    return 1;
    
//    if(m_x.GetNumberOfElements()!=m_y.GetNumberOfElements())
//    {
//        cout<<"bdCurveXY::LoadSingleArrayFile_v2(): loaded X and Y arrays are not of the same size: X= "<<m_x.GetNumberOfElements()<<", Y= "<<m_y.GetNumberOfElements()<<endl;
//        m_x.Reset();
//        m_y.Reset();
//        input_file.close();
//        return 0;
//    }
//
//    if(m_x.GetNumberOfElements()==0)
//    {
//        cout<<"bdCurveXY::LoadSingleArrayFile()_v2: no elemetns loaded, array is empty!"<<endl;
//        input_file.close();
//        return 0;
//    }
//
//    input_file.close();
//    return 1;
}



int mxCurve::Load_Legacy_SingleArrayFile_v2a(const char *file_name)
{
    std::ifstream file;
    file.open(file_name,std::ios::binary);
    if(!file)
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3() : Error opening file: "<<file_name<<std::endl;
        return 0;
    }
    
    const unsigned int amount_of_characters_to_load = 1000000;
    char text_buffer[amount_of_characters_to_load];
    
    
    if(!file.getline(text_buffer, amount_of_characters_to_load)) return 0;
    if(text_buffer[0]!='%' || text_buffer[1]!='S' || text_buffer[2]!='i' || text_buffer[3]!='n' || text_buffer[4]!='g' || text_buffer[5]!='l' || text_buffer[6]!='e' ||
       text_buffer[7]!=' ' || text_buffer[8]!='A' || text_buffer[9]!='r' || text_buffer[10]!='r' || text_buffer[11]!='a' || text_buffer[12]!='y' ||
       text_buffer[13]!=' ' || text_buffer[14]!='F' || text_buffer[15]!='i' || text_buffer[16]!='l' || text_buffer[17]!='e' ||  text_buffer[18]!=':')
    { file.close(); return 0; }
    
    // load version
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%' && text_buffer[1]!='v') { file.close(); return 0; }
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%') { file.close(); return 0; }
    mxString text_string;
    text_string.Append(&(text_buffer[1]));
    mxList< int > list;
    text_string.ExtractNumbers(list);
    if(list.IsEmpty())
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): No version value found!"<<std::endl;
        file.close();
        return 0;
    }
    if(list.GetBeginElement()>2)
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): Version higher than supported, version: "<<list.GetBeginElement()<<std::endl;
        file.close();
        return 0;
    }
    
    // if version is not 2, but is not higher than 2, just return 0 without notification, it will be handeled with earlier load methods.
    if(list.GetBeginElement()!=2)
    {
        return 0;
    }
    
    // read labels.
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%' && text_buffer[1]!='l') { file.close(); return 0; }
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%') { file.close(); return 0; }
    this->SetLabelFor_X_Axis(&(text_buffer[1]));
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%') { file.close(); return 0; }
    this->SetLabelFor_Y_Axis(&(text_buffer[1]));
    
    // skip tag reading - check for the existing fields, but perform nothing.
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%' && text_buffer[1]!='t') { file.close(); return 0; }
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%') { file.close(); return 0; }
    
    // read X values
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    
    //mxArrayFunctions::LoadFromStringOfDoubleValues(*(this->Get_X_Values()), text_buffer, amount_of_characters_to_load);
    
    if(!mxArrayFunctions::LoadFromStringOfDoubleValues(*(this->Get_X_Values()), text_buffer, amount_of_characters_to_load))
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): error reading X array!"<<std::endl;
        std::cout<<"text_buffer: "<<text_buffer<<std::endl;
        this->Reset();
        file.close();
        return 0;
    }
    
    // read Y values
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(!mxArrayFunctions::LoadFromStringOfDoubleValues(*(this->Get_Y_Values()), text_buffer, amount_of_characters_to_load))
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): error reading Y array!"<<std::endl;
        std::cout<<"text_buffer: "<<text_buffer<<std::endl;
        this->Reset();
        file.close();
        return 0;
    }
    
    if(this->Get_X_Values()->GetNumberOfElements() != this->Get_Y_Values()->GetNumberOfElements())
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): error: unequal size of X and Y values arrays, size_of_X="<<this->Get_X_Values()->GetNumberOfElements()<<", size_of_Y="<<this->Get_Y_Values()->GetNumberOfElements()<<std::endl;
        this->Reset();
        file.close();
        return 0;
    }
    
    if(this->IsEmpty())
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): error: loaded curve is empty!"<<std::endl;
        file.close();
        return 0;
    }
    
    file.close();
    return 1;
}




int mxCurve::Load_Legacy_SingleArrayFile_v3(const char *file_name)
{
    std::ifstream file;
    file.open(file_name,std::ios::binary);
    if(!file)
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3() : Error opening file: "<<file_name<<std::endl;
        return 0;
    }
    
    const unsigned int amount_of_characters_to_load = 1000000;
    char text_buffer[amount_of_characters_to_load];
    
    
    if(!file.getline(text_buffer, amount_of_characters_to_load)) return 0;
    if(text_buffer[0]!='%' || text_buffer[1]!='S' || text_buffer[2]!='i' || text_buffer[3]!='n' || text_buffer[4]!='g' || text_buffer[5]!='l' || text_buffer[6]!='e' ||
       text_buffer[7]!=' ' || text_buffer[8]!='A' || text_buffer[9]!='r' || text_buffer[10]!='r' || text_buffer[11]!='a' || text_buffer[12]!='y' ||
       text_buffer[13]!=' ' || text_buffer[14]!='F' || text_buffer[15]!='i' || text_buffer[16]!='l' || text_buffer[17]!='e' ||  text_buffer[18]!=':')
    { file.close(); return 0; }
    
    // load version
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%' && text_buffer[1]!='v') { file.close(); return 0; }
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%') { file.close(); return 0; }
    mxString text_string;
    text_string.Append(&(text_buffer[1]));
    mxList< int > list;
    text_string.ExtractNumbers(list);
    if(list.IsEmpty())
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): No version value found!"<<std::endl;
        file.close();
        return 0;
    }
    if(list.GetBeginElement()>3)
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): Version higher than supported, version: "<<list.GetBeginElement()<<std::endl;
        file.close();
        return 0;
    }
    
    // if version is not 3, but is not higher than 3, just return 0 without notification, it will be handeled with earlier load methods.
    if(list.GetBeginElement()!=3)
    {
        return 0;
    }
    
    // read labels.
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%' && text_buffer[1]!='l') { file.close(); return 0; }
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%') { file.close(); return 0; }
    this->SetLabelFor_X_Axis(&(text_buffer[1]));
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%') { file.close(); return 0; }
    this->SetLabelFor_Y_Axis(&(text_buffer[1]));
    
    // skip tag reading - check for the existing fields, but perform nothing.
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%' && text_buffer[1]!='t') { file.close(); return 0; }
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(text_buffer[0]!='%') { file.close(); return 0; }
    
    // read X values
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    
    //mxArrayFunctions::LoadFromStringOfDoubleValues(*(this->Get_X_Values()), text_buffer, amount_of_characters_to_load);
    
    if(!mxArrayFunctions::LoadFromStringOfDoubleValues(*(this->Get_X_Values()), text_buffer, amount_of_characters_to_load))
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): error reading X array!"<<std::endl;
        std::cout<<"text_buffer: "<<text_buffer<<std::endl;
        this->Reset();
        file.close();
        return 0;
    }
    
    // read Y values
    if(!file.getline(text_buffer, amount_of_characters_to_load)) { file.close(); return 0; }
    if(!mxArrayFunctions::LoadFromStringOfDoubleValues(*(this->Get_Y_Values()), text_buffer, amount_of_characters_to_load))
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): error reading Y array!"<<std::endl;
        std::cout<<"text_buffer: "<<text_buffer<<std::endl;
        this->Reset();
        file.close();
        return 0;
    }
    
    if(this->Get_X_Values()->GetNumberOfElements() != this->Get_Y_Values()->GetNumberOfElements())
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): error: unequal size of X and Y values arrays, size_of_X="<<this->Get_X_Values()->GetNumberOfElements()<<", size_of_Y="<<this->Get_Y_Values()->GetNumberOfElements()<<std::endl;
        this->Reset();
        file.close();
        return 0;
    }
    
    if(this->IsEmpty())
    {
        std::cout<<"mxCurve::Load_Legacy_SingleArrayFile_v3(): error: loaded curve is empty!"<<std::endl;
        file.close();
        return 0;
    }
    
    file.close();
    return 1;
}


int mxCurve::LoadMatlabFile(const char *file_name)
{
    if(this->Load_Legacy_SingleArrayFile_v3(file_name)) return 1;
    if(this->Load_Legacy_SingleArrayFile_v2a(file_name)) return 1;
    if(this->Load_Legacy_SingleArrayFile_v2(file_name)) return 1;
    return 0;
}


void mxCurve::Reset()
{
    // Do not reset the labels, if they need to be changed, it will be done with set label methods.
    
    this->m_X_values.Reset();
    this->m_Y_values.Reset();
}


void mxCurve::SaveToMatlabFile(const char *file_name)
{
    if(!file_name) return;
    
    mxString file_name_with_path, extension, file_name_string;
    file_name_string.Assign(file_name);
    file_name_string.ExtractFileNameAndFileExtension(file_name_with_path, extension);
    
    if(extension!="m" && extension!="M")
    {
        file_name_string.Append(".m");
    }
    
    // open file
    std::ofstream file;
    file.open(file_name_string.Get_C_String(), std::ios::binary);
    
    // file type
    file<<"%Single Array File: "<<std::endl;
    
    // version
    file<<"%v"<<std::endl;
    file<<"%3.0"<<std::endl;
    
    // labels for axes
    file<<"%l"<<std::endl;
    file<<"%"<<this->GetLabelFor_X_Axis().Get_C_String()<<std::endl;
    file<<"%"<<this->GetLabelFor_Y_Axis().Get_C_String()<<std::endl;
    
    // tags - this is legacy to support loading with previous version of the loader - not used in new version.
    file<<"%t"<<std::endl;
    file<<"%"<<" "<<std::endl;
    
    // X values array
    file<<"values_x = [";
    for(unsigned int s=0; s<this->GetNumberOfSamples()-1; s++)
    {
        std::ostringstream oss;
        oss.precision(4);//std::numeric_limits<double>::digits);
        oss << std::fixed << this->m_X_values[s];
        std::string str = oss.str();
        
//        file.showpoint;
//        file<<std::ofstream::showpoint;
//        file.precision(4);
//        file<<std::ofstream::fixed;

        file<<str;//this->m_X_values[s];
        file<<", ";
    }
    file<<this->m_X_values[this->GetNumberOfSamples()-1]<<"];"<<std::endl;
    
    // Y values array
    file<<"values_y = [";
    for(unsigned int s=0; s<this->GetNumberOfSamples()-1; s++)
    {
        std::ostringstream oss;
        oss.precision(4);//std::numeric_limits<double>::digits);
        oss << std::fixed << this->m_Y_values[s];
        std::string str = oss.str();


        file<<str;//this->m_Y_values[s];
        file<<", ";
    }
    file<<this->m_Y_values[this->GetNumberOfSamples()-1]<<"];"<<std::endl;
    
    // close file.
    file.close();
}


void mxCurve::Set(unsigned int index, double x_value, double y_value)
{
    this->m_X_values[index] = x_value;
    this->m_Y_values[index] = y_value;
}


void mxCurve::SetLabelFor_X_Axis(const char *label)
{
    this->m_label_for_X_axis.Assign(label);
}


void mxCurve::SetLabelFor_Y_Axis(const char *label)
{
    this->m_label_for_Y_axis.Assign(label);
}


void mxCurve::SetNumberOfSamples(unsigned int s)
{
	this->m_X_values.SetNumberOfElements(s);
	this->m_Y_values.SetNumberOfElements(s);
}


void mxCurve::Sort_X_Ascending()
{
    mxArray< int > order_of_index_change;
    mxArrayFunctions::SortAscending(this->m_X_values, order_of_index_change);
    
    mxArray< double > original_y_values;
    original_y_values.CopyFrom(this->m_Y_values);
    
    for(unsigned int s=0; s<this->GetNumberOfSamples(); s++)
    {
        this->m_Y_values[s] = original_y_values[(order_of_index_change[s])];
    }
}


double& mxCurve::X(unsigned int index)
{
    return this->m_X_values[index];
}


double& mxCurve::Y(unsigned int index)
{
	return this->m_Y_values[index];
}


double mxCurve::Y_FX(double x_value)
{
    // We assume that the x_Vaue will not be found, so we search for the closest
    // larger x value in the array sorted in ascending order.

    int s=0;
    for(s=0; s<this->GetNumberOfSamples(); s++)
    {
        if(this->m_X_values[s] > x_value)
        {
            break;
        }
    }

    // sample is before the sample with index 0 (we assume Y value 0).
    if(s==0)
    {
        return ( this->m_Y_values[0] / 2.0 );
    }

    // sample is after the sample with index 'this->GetNumberOfSamples()-1' (we assume Y value 0).
    if(s>=this->GetNumberOfSamples())
    {
        return ( this->m_Y_values[this->GetNumberOfSamples()-1] / 2.0 );
    }
    
    // sample falls between existing X values, so do the interpolation.
    return ( (this->m_Y_values[s]-this->m_Y_values[s-1]) * (x_value-this->m_X_values[s-1]) / (this->m_X_values[s]-this->m_X_values[s-1]) + this->m_Y_values[s-1] );
}



