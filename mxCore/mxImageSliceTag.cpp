/*=========================================================================
 
 Program:   mipx
 Module:    mxImageSliceTag.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxImageSliceTag.h"



mxArray< mxImageSliceTagNumber > mxImageSliceTag::m_dicom_tag_names;




mxImageSliceTag::mxImageSliceTag()
{
	if(m_dicom_tag_names.IsEmpty())
	{
		m_dicom_tag_names.SetNumberOfElements(5);
        
		int i=0;
        
        this->m_dicom_tag_names[i].m_number1.Assign("0018");
        this->m_dicom_tag_names[i].m_number2.Assign("1060");
        this->m_dicom_tag_names[i].m_description.Assign("Trigger Time");
        i++;
        
        this->m_dicom_tag_names[i].m_number1.Assign("0020");
        this->m_dicom_tag_names[i].m_number2.Assign("0013");
        this->m_dicom_tag_names[i].m_description.Assign("Image Number");
        i++;
        
        this->m_dicom_tag_names[i].m_number1.Assign("0020");
        this->m_dicom_tag_names[i].m_number2.Assign("0032");
        this->m_dicom_tag_names[i].m_description.Assign("Patient Position");
        i++;
        
        this->m_dicom_tag_names[i].m_number1.Assign("0020");
        this->m_dicom_tag_names[i].m_number2.Assign("0037");
        this->m_dicom_tag_names[i].m_description.Assign("Image Orientation RC");
        i++;
        
        this->m_dicom_tag_names[i].m_number1.Assign("0020");
        this->m_dicom_tag_names[i].m_number2.Assign("1041");
        this->m_dicom_tag_names[i].m_description.Assign("Slice Location");
        i++;
	}

	m_dicom_tag_values.SetNumberOfElements(m_dicom_tag_names.GetNumberOfElements());
}


mxString* mxImageSliceTag::GetValue(const char *tag_number1, const char *tag_number2)
{
    int index = this->IndexOfDicomTagNumber(tag_number1, tag_number2);
    if(index >= 0) { return( &(this->m_dicom_tag_values[index]) ); }
    return NULL;
}


int mxImageSliceTag::GetValueAsDouble(const char *tag_number1, const char *tag_number2, double &output_value)
{
    mxString *v = this->GetValue(tag_number1, tag_number2);
    if(!v) return 0;
    
    if(!v->ToNumber(output_value)) return 0;
    
    return 1;
}


int mxImageSliceTag::GetValueAsListOfDoubles(const char *tag_number1, const char *tag_number2, mxList< double > &output_values)
{
    mxString *v = this->GetValue(tag_number1, tag_number2);
    if(!v) return 0;
    
    v->ExtractNumbers(output_values);
    if(output_values.IsEmpty()) return 0;
    
    return 1;
}


int mxImageSliceTag::GetValueAsArrayOfDoubles(const char *tag_number1, const char *tag_number2, mxArray< double > &output_values)
{
    mxList< double > l;
    
    if(!this->GetValueAsListOfDoubles(tag_number1, tag_number2, l)) return 0;
    
    output_values.SetNumberOfElements(l.GetNumberOfElements());
    
    mxListIterator< double > it;
    int i=0;
    for(it.SetToBegin(l), i=0; it.IsValid() && i<(int)output_values.GetNumberOfElements(); it.MoveToNext(), i++)
    {
        output_values[i] = it.GetElement();
    }
    return 1;
}


int mxImageSliceTag::IndexOfDicomTagNumber(const char *tag_number1, const char *tag_number2)
{
    for(int i=0; i<(int)m_dicom_tag_names.GetNumberOfElements(); i++)
    {
        if( (m_dicom_tag_names[i].m_number1 == tag_number1) && (m_dicom_tag_names[i].m_number2 == tag_number2) )
        {
            return i;
        }
    }
    return -1;
}


void mxImageSliceTag::SaveValuesToString(mxString &s)
{
	mxString temp;
	for(unsigned int i=0; i<m_dicom_tag_names.GetNumberOfElements(); i++)
	{
        s.Append("<");
        s.Append("("); s.Append(m_dicom_tag_names[i].m_number1); s.Append(","); s.Append(m_dicom_tag_names[i].m_number2); s.Append(")");
        m_dicom_tag_values[i].ReplaceCharacters('\n', ' ');
        s.Append("{"); s.Append(m_dicom_tag_values[i]); s.Append("}");
        s.Append(">");
        
	}

}


int mxImageSliceTag::LoadNumbersFromString(mxString &s)
{
    if(s.IsEmpty()) return 0;
    
    mxList< mxString > list_of_tags;
    s.ExtractStrings('<','>',list_of_tags);
    
    mxListIterator<mxString> it;
    for(it.SetToBegin(list_of_tags); it.IsValid(); it.MoveToNext())
    {
        mxString tag_numbers;
        it.GetElementAddress()->ExtractString('(',')',tag_numbers);
        
        mxList< mxString > tag_numbers_list;
        tag_numbers.ExtractHexadecimalStrings(tag_numbers_list);
        
        if(tag_numbers_list.GetNumberOfElements() != 2)
        {
            return 0;
        }
        
        int index = this->IndexOfDicomTagNumber(tag_numbers_list.GetBeginElement().Get_C_String(), tag_numbers_list.GetEndElement().Get_C_String());
        
        if(index>=0)
        {
            if(!it.GetElementAddress()->ExtractString('[',']', this->m_dicom_tag_names[index].m_description)) return 0;
        }
        
    }
    
    return 1;
}


int mxImageSliceTag::LoadValuesFromString(mxString &s)
{
	if(s.IsEmpty()) return 0;

	mxList< mxString > list_of_tags;
	s.ExtractStrings('<','>',list_of_tags);
    
	mxListIterator<mxString> it;
	for(it.SetToBegin(list_of_tags); it.IsValid(); it.MoveToNext())
	{
        mxString tag_numbers;
        it.GetElementAddress()->ExtractString('(',')',tag_numbers);
        
        mxList< mxString > tag_numbers_list;
        tag_numbers.ExtractHexadecimalStrings(tag_numbers_list);
        
        if(tag_numbers_list.GetNumberOfElements() != 2)
        {
            return 0;
        }
        
        int index = this->IndexOfDicomTagNumber(tag_numbers_list.GetBeginElement().Get_C_String(), tag_numbers_list.GetEndElement().Get_C_String());
        
        if(index>=0)
        {
            if(!it.GetElementAddress()->ExtractString('{','}' , this->m_dicom_tag_values[index])) return 0;
        }
        
	}

	return 1;
}


mxImageTag::mxImageTag()
{
}


mxImageSliceTag& mxImageTag::GetTag(int t, int s)
{
    return m_tags(t,s);
}


mxImageSliceTag& mxImageTag::GetTag(int i)
{
    return m_tags[i];
}


void mxImageTag::SetDimensions(int t, int s)
{
    m_tags.SetDimensions(t,s);
}


void mxImageTag::SaveToFile(const char *file_name)
{
    std::ofstream file;
    file.open(file_name, std::ios::binary);
    
    file<<"%mxImageTag"<<std::endl;
    file<<"v"<<std::endl;
    file<<"1.0"<<std::endl;

//    // Save slice tag numbers and descriptions
//    file<<"n"<<std::endl;
//    mxImageSliceTag::m_dicom_tag_names[0].
//    this->m_tags(0,0).
//    for(unsigned int t=0; t<this->m_tags.GetNumberOfRows(); t++)
//    {
//        for(unsigned int s=0; s<this->m_tags.GetNumberOfColumns(); s++)
//        {
//            mxString mxs;
//            this->m_tags(t,s). SaveValuesToString(mxs);
//            file<<mxs.Get_C_String()<<std::endl;
//        }
//    }

    
    // Save slice tag values
    file<<"t"<<std::endl;
    for(unsigned int t=0; t<this->m_tags.GetNumberOfRows(); t++)
    {
        for(unsigned int s=0; s<this->m_tags.GetNumberOfColumns(); s++)
        {
            mxString mxs;
            this->m_tags(t,s).SaveValuesToString(mxs);
            file<<mxs.Get_C_String()<<std::endl;
        }
    }
    
    file.close();
}


int mxImageTag::LoadFromFile(const char *file_name)
{
    if(this->m_tags.IsEmpty()) return 0;
    
    std::ifstream input_file;
    input_file.open(file_name, std::ios::binary);
    if(!input_file)
    {
        std::cout<<std::endl<<"mxImageTag::LoadFromFile() error: cannot open file: "<<file_name;
        return 0;
    }
    
    char text_line[100000];
    int line_size = 99999;
    
    if(!input_file.getline(text_line, line_size)) return 0;
    if(text_line[0]!='%' || text_line[1]!='m' || text_line[2]!='x' || text_line[3]!='I' || text_line[4]!='m' || text_line[5]!='a' || text_line[6]!='g' ||
       text_line[7]!='e' || text_line[8]!='T' || text_line[9]!='a' || text_line[10]!='g') { input_file.close(); return 0; }
    
    if(!input_file.getline(text_line, line_size)) return 0;
    if(text_line[0]!='v') return 0;
    if(!input_file.getline(text_line, line_size)) return 0;
    mxString bds;
    bds.Append(text_line);
    mxList<int> list;
    bds.ExtractNumbers(list);
    if(list.IsEmpty())
    {
        std::cout<<std::endl<<"mxImageTag::LoadFromFile() error: no version value found!";
        return 0;
    }
    if(list.GetBeginElement()>1)
    {
        std::cout<<std::endl<<"mxImageTag::LoadFromFile() error: version higher than supported, current version is "<<list.GetBeginElement();
        return 0;
    }
    
    if(!input_file.getline(text_line, line_size)) return 0;
    if(text_line[0]!='t') return 0;
    
    for(unsigned int t=0; t<this->m_tags.GetNumberOfRows(); t++)
    {
        for(unsigned int s=0; s<this->m_tags.GetNumberOfColumns(); s++)
        {
            if(!input_file.getline(text_line, line_size)) return 0;
            
            mxString mxs;
            mxs.Assign(text_line);
            this->GetTag(t,s).LoadValuesFromString(mxs);
        }
    }
    
    return 1;
}
