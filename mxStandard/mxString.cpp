/*=========================================================================
 
 Program:   mipx
 Module:    mxString.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxString.h"



mxString::mxString()
{
}


mxString::~mxString()
{
}


std::string& mxString::GetStdString()
{
    return this->m_string;
}


const char* mxString::Get_C_String()
{
    return this->m_string.c_str();
}


void mxString::Clear()
{
    this->m_string.clear();
}


int mxString::IsEmpty()
{
    return this->m_string.empty();
}


unsigned int mxString::GetNumberOfCharacters()
{
    return ((unsigned int)this->m_string.length());
}


unsigned int mxString::Length()
{
    return (this->GetNumberOfCharacters());
}


void mxString::Assign(mxString &s)
{
    this->m_string.assign(s.m_string);
}


void mxString::Assign(const char c)
{
    this->m_string.assign(&c,0,1);
}


void mxString::Assign(const char *s)
{
    this->m_string.assign(s);
}


void mxString::Assign(const std::string &s)
{
    this->m_string.assign(s);
}


void mxString::Append(mxString &s)
{
    this->m_string.append(s.m_string);
}


void mxString::Append(const char c)
{
    this->m_string.append(&c,0,1);
}


void mxString::Append(const char *s)
{
    this->m_string.append(s);
}


void mxString::Append(const std::string &s)
{
    this->m_string.append(s);
}


void mxString::AppendNumber(int number)
{
    mxString s; s.AssignNumber(number);
    this->Append(s);
}


void mxString::AppendNumber(double number)
{
    mxString s; s.AssignNumber(number);
    this->Append(s);
}


void mxString::AssignNumber(int number)
{
	std::ostringstream os;
	os<<number;
    this->Clear();
	this->Assign(os.str());
}


void mxString::AssignNumber(double number, int number_of_decimals)
{
    this->Clear();

    // Record the sign and make input positive.
    if(number<0)
    {
        this->Append('-');
        number = -number;
    }
    
    // Convert double to int and from it to string.
    mxString int_string_to_add;
    for(int i=0; i<number_of_decimals+1; i++)
    {
        int n = ((int) number);
        
        mxString int_string;
        int_string.AssignNumber(n);
        int_string_to_add.Append(int_string);
        
        if(n>0)
        {
            this->Append(int_string_to_add);
            int_string_to_add.Clear();
        }
        
        number = (number - ((double)n)) * 10.0;
        
        if(i==0)
        {
            this->Append(int_string_to_add);
            int_string_to_add.Assign(".");
        }
    }
}


int mxString::ToNumber(int &output_number)
{
	if(this->IsEmpty()) return 0;
	std::istringstream i(this->GetStdString());
	return (i>>output_number) ? true : false;
}


int mxString::ToNumber(double &output_number)
{
	if(this->IsEmpty()) return 0;
    std::istringstream i(this->GetStdString());
	return (i>>output_number) ? true : false;
}


int mxString::ToPosition(double &output_coordinate1, double &output_coordinate2, double &output_coordinate3)
{
    mxList<double> extracted_position;
    this->ExtractNumbers(extracted_position);
    if(extracted_position.GetNumberOfElements()>=3)
    {
        output_coordinate1 = extracted_position[0];
        output_coordinate2 = extracted_position[1];
        output_coordinate3 = extracted_position[2];
        return 1;
    }
    return 0;
}


void mxString::ToWindowsPath()
{
    for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
    {
        if(this->m_string[i]=='/') { this->m_string[i] = '\\'; }
    }
}


void mxString::ToLinuxPath()
{
    for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
    {
        if(this->m_string[i]=='\\') { this->m_string[i] = '/'; }
    }
}


void mxString::ExtractNumbers(mxList<int> &list_of_ints)
{
	list_of_ints.Reset();

	if(this->IsEmpty()) return;

	mxList<mxString> l;
	l.AddNewToEnd();
    
    int is_sign_found = 0;
	for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
	{
		if(!is_sign_found)
		{
			if((*this)[i]=='+' || (*this)[i]=='-' || ((*this)[i]>='0' && (*this)[i]<='9'))
			{
				l.GetEndElement().Append((*this)[i]);
				is_sign_found = 1;
			}
		}
		else
		{
			if((*this)[i]>='0' && (*this)[i]<='9') l.GetEndElement().Append((*this)[i]);
			else
			{
				l.AddNewToEnd();
				is_sign_found = 0;
			}
		}
	}

	//Now we convert all the strings in list l to int values and if conversion is valid, we enter values to output list.
	while(!l.IsEmpty())
	{
		int n;
		if(l.GetBeginElement().ToNumber(n)) list_of_ints.AddToEnd(n);
		l.DeleteBegin();
	}
}


void mxString::ExtractNumbers(mxList<double> &list_of_doubles)
{
    list_of_doubles.Reset();
    
    if(this->IsEmpty()) return;
    
    mxString temp_string;
    for(unsigned int i=0; i<m_string.length(); i++)
    {
        if((*this)[i]=='+' || (*this)[i]=='-' || ((*this)[i]>='0' && (*this)[i]<='9') || (*this)[i]=='.' || (*this)[i]=='e') { temp_string.Append((*this)[i]); }
        else
        {
            // When we find a character that is not part of the number, convert the temp_string into a double and write it to output list.
            double d;
            if(temp_string.ToNumber(d)) { list_of_doubles.AddToEnd(d); }
            temp_string.Clear();
        }
    }
    
    // If the string ended with a number, the temp_string will be non-empty...
    if(!temp_string.IsEmpty())
    {
        //... and try to convert the temp_string to a double and write it to output list.
        double d;
        if(temp_string.ToNumber(d)) { list_of_doubles.AddToEnd(d); }
        temp_string.Clear();
    }
}


void mxString::ExtractHexadecimalStrings(mxList<mxString> &list_of_hexadecimal_strings)
{
    list_of_hexadecimal_strings.Reset();
    
    if(this->IsEmpty()) return;
    
    mxString *ps = list_of_hexadecimal_strings.AddNewToEnd();
    
    int is_sign_found = 0;
    for(unsigned int i=0; i<this->m_string.length(); i++)
    {
        if(!is_sign_found)
        {
            if((*this)[i]=='+' || (*this)[i]=='-' || ((*this)[i]>='0' && (*this)[i]<='9') || ((*this)[i]>='a' && (*this)[i]<='f') || ((*this)[i]>='A' && (*this)[i]<='F'))
            {
                ps->Append((*this)[i]);
                is_sign_found = 1;
            }
        }
        else
        {
            if(((*this)[i]>='0' && (*this)[i]<='9') || ((*this)[i]>='a' && (*this)[i]<='f') || ((*this)[i]>='A' && (*this)[i]<='F')) ps->Append((*this)[i]);
            else
            {
                ps = list_of_hexadecimal_strings.AddNewToEnd();
                is_sign_found = 0;
            }
        }
    }
    
    // If the last string in the list is empty just delete it.
    // This happens in case the original string ended with a non-hexadecimal character.
    if(ps->IsEmpty()) list_of_hexadecimal_strings.DeleteEnd();
}


void mxString::ExtractSerialsOfNumbersAndDots(mxList<mxString> &list_of_serials_of_numbers_and_dots)
{
    list_of_serials_of_numbers_and_dots.Reset();
    
    if(this->IsEmpty()) return;
    
    mxString *ps = list_of_serials_of_numbers_and_dots.AddNewToEnd();
    
    int is_number_or_dot_found = 0;
    for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
    {
        if(((*this)[i]>='0' && (*this)[i]<='9') || (*this)[i]=='.')
        {
            ps->Append((*this)[i]);
            is_number_or_dot_found = 1;
        }
        else
        {
            if(is_number_or_dot_found)
            {
                ps = list_of_serials_of_numbers_and_dots.AddNewToEnd();
                is_number_or_dot_found = 0;
            }
        }
    }
    
    // If the last string in the list is empty just delete it.
    // This happens in case the original string ended with a character other than number or dot.
    if(ps->IsEmpty()) list_of_serials_of_numbers_and_dots.DeleteEnd();
}


void mxString::ExtractLastNumber(mxString &remaining_string, int &number)
{
	if(this->IsEmpty()) return;

	mxList<char> list_of_chars;

	int is_number_found = 0;
    int index_to_record = 0;
	for( int i = (int) (this->GetNumberOfCharacters()-1); i>=0; i--)
	{
        if((*this)[i]=='+' || (*this)[i]=='-' || ((*this)[i]>='0' && (*this)[i]<='9'))
		{
			list_of_chars.AddToBegin((*this)[i]);
            if((*this)[i]>='0' && (*this)[i]<='9') { is_number_found = 1; }
		}
		else
		{
			if(is_number_found)
			{
				// If we have found the character that is not part of the number convert the list of chars into an int.
				mxString temp_string;
				mxListIterator<char> it;
				for(it.SetToBegin(list_of_chars); it.IsValid(); it.MoveToNext()) { temp_string.Append(it.GetElement()); }
				temp_string.ToNumber(number);
                index_to_record = i;
				break;
			}
		}
	}

	// Copy the remaining string as output (from index 0 till index 'i').
	if(is_number_found)
	{
		for(int j=0; j<=index_to_record; j++) { remaining_string.Append((*this)[j]); }
    }
	else { remaining_string = (*this); }
}


void mxString::ExtractNonIntNumberString(mxString &extracted_string)
{
	if(this->IsEmpty()) return;
    
	extracted_string.Clear();
	
	int is_first_non_number_character_found = 0;
	for(unsigned int i=0; i<m_string.length(); i++)
	{
		if(!is_first_non_number_character_found) 
		{
			if(!((*this)[i]=='+' || (*this)[i]=='-' || ((*this)[i]>='0' && (*this)[i]<='9')))
			{
				is_first_non_number_character_found = 1;
				extracted_string.Append((*this)[i]);
			}
		}
		else
		{
			if(!((*this)[i]=='+' || (*this)[i]=='-' || ((*this)[i]>='0' && (*this)[i]<='9'))) { extracted_string.Append((*this)[i]); }
			else return;
		}
	}
}


int mxString::ExtractString(char start_character, char end_character, mxString &output)
{
    output.Clear();
    
    int is_start_character_found = 0;
    int is_end_character_found = 0;
    for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
    {
        if(!is_start_character_found) { if((*this)[i] == start_character) is_start_character_found = 1; }
        else
        {
            if((*this)[i] == end_character)
            {
                is_end_character_found = 1;
                break;
            }
            else { output.Append((*this)[i]); }
        }
    }	
    
    if(is_end_character_found) return 1;
    else
    {
        output.Clear();
        return 0;
    }
}


int mxString::ExtractStrings(char start_character, char end_character, mxList<mxString> &output_list)
{
    if(this->IsEmpty()) return 0;
    
    output_list.Reset();
    
    mxString temp;
    
    int is_start_character_found = 0;
    for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
    {
        if(!is_start_character_found)
        {
            if((*this)[i]==start_character)
            {
                temp.Clear();
                is_start_character_found = 1;
            }
        }
        else
        {
            if((*this)[i]==end_character)
            {
                output_list.AddToEnd(temp);
                is_start_character_found = 0;
            }
            else { temp.Append((*this)[i]); }
        }
    }
    
    return 1;
}


int mxString::ExtractCommonString(mxString &input, mxString &output)
{
    if(this->IsEmpty() || input.IsEmpty()) return 0;
    
    output.Clear();
    for(unsigned int i=0; i<this->GetNumberOfCharacters() && i<input.GetNumberOfCharacters(); i++)
    {
        if((*this)[i]==input[i]) { output.Append((*this)[i]); }
        else { return 1; }
    }
    
    return 1;
}


int mxString::ExtractDifferenceString(mxString &input_string, mxString &output_difference_string)
{
    if(input_string.IsEmpty() || this->IsEmpty()) return 0;
    
    output_difference_string.Clear();
    
    int is_difference_found = 0;
    for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
    {
        if(!is_difference_found)
        {
            if(input_string[i]!=(*this)[i])
            {
                output_difference_string.Append((*this)[i]);
                is_difference_found = 1;
            }
        }
        else { output_difference_string.Append((*this)[i]); }
    }
    
    return 1;
}


int mxString::ExtractFileNameAndFileExtension(mxString &output_file_name_with_path, mxString &output_extension)
{
    if(this->IsEmpty()) return 0;
    
    int index_of_last_dot = -1;
    for(int i=this->GetNumberOfCharacters()-1; i>=0; i--)
    {
        if((*this)[i]=='.')
        {
            index_of_last_dot = i;
            break;
        }
    }
    
    // If the dot was found...
    if(index_of_last_dot>0)
    {
        //...copy the string up to (but NOT including) the last dot to output_file_name_with_path...
        output_file_name_with_path.Clear();
        for(int i=0; i<index_of_last_dot; i++)
        {
            output_file_name_with_path.Append((*this)[i]);
        }
        //... and copy the string from the last dot (but NOT including it) to the end of the string to output_extension.
        output_extension.Clear();
        for(int i=index_of_last_dot+1; i<this->GetNumberOfCharacters(); i++)
        {
            output_extension.Append((*this)[i]);
        }
    }
    
    return 1;
}


int mxString::ExtractFilePathParts(mxString &output_dir, mxString &output_file_name, mxString &output_extension)
{
    if(this->IsEmpty()) return 0;
    
    if(!( (((*this)[0]>='a' && (*this)[0]<='z') || ((*this)[0]>='A' && (*this)[0]<='Z')) || (*this)[0]=='\\' || (*this)[0]=='/') ) return 0;
    
    //Check for the double_dot in case when the path starts with a letter
    if( ((*this)[0]>='a' && (*this)[0]<='z') || ((*this)[0]>='A' && (*this)[0]<='Z') ) { if((*this)[1]!=':') return 0; }
    
    int index_of_last_slash = -1;
    int index_of_last_dot = -1;
    for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
    {
        if((*this)[i]=='\\' || (*this)[i]=='/') { index_of_last_slash = i; }
        if((*this)[i]=='.') { index_of_last_dot = i; }
    }

    // If the slash was found, copy the string up to and including the last slash to output_dir.
    output_dir.Clear();
    if(index_of_last_slash>0)
    {
        for(int i=0; i<=index_of_last_slash; i++) { output_dir.Append((*this)[i]); }
    }
    
    // If the dot was found, copy the string from the last slash up to (but NOT including) the last dot to output_file_name.
    output_extension.Clear();
    if(index_of_last_dot>0)
    {
        for(int i=index_of_last_dot+1; i<this->GetNumberOfCharacters(); i++) { output_extension.Append((*this)[i]); }
    }
    else
    {
        // In case dot was not found, set its index to the number of characters - needed in next lines to copy file name.
        index_of_last_dot = this->GetNumberOfCharacters();
    }
    
    // Copy the string from the last slash up to (but NOT including) the last dot to output_file_name.
    output_file_name.Clear();
    for(int i=index_of_last_slash+1; i<index_of_last_dot; i++) { output_file_name.Append((*this)[i]); }
    
    return 1;
}


int mxString::PathLevelUp(mxString &output_dir)
{
    int index_of_last_slash = -1;
    // Index this->GetNumberOfCharacters()-1 is not of interest because it is either a '/' ('\\')
    // or a letter.
    for(int i=this->GetNumberOfCharacters()-2; i>=0; i--)
    {
        if((*this)[i]=='\\' || (*this)[i]=='/') { index_of_last_slash = i; break; }
    }
    
    // If the slash was found, copy the string up to and including the last slash to output_dir.
    output_dir.Clear();
    if(index_of_last_slash>=0)
    {
        for(int i=0; i<=index_of_last_slash; i++) { output_dir.Append((*this)[i]); }
        return 1;
    }
    return 0;
}


void mxString::ReplaceCharacters(char character_to_replace, char replacement_character)
{
    for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
    {
        if((*this)[i]==character_to_replace) { (*this)[i] = replacement_character; }
    }
}


int mxString::InsertNumber(int n, char marker_character)
{
    if(this->IsEmpty()) return 0;
    
    int is_marker_character_found = 0;
	int first_marker_character_index = 0;
	int number_of_marker_characters = 0;

	for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++ )
	{
		if(!is_marker_character_found)
		{
			if((*this)[i]==marker_character)
			{
                (*this)[i] = '0';
				is_marker_character_found = 1;
				first_marker_character_index = i;
                number_of_marker_characters++;
			}
		}
		else
		{
			if((*this)[i]==marker_character)
			{
				(*this)[i] = '0';
                number_of_marker_characters++;
			}
            else break;
		}
	}

	if(!is_marker_character_found) return 0;

	mxString temp;
	temp.AssignNumber(n);

	for(int j=0; j<number_of_marker_characters && j<((int)temp.GetNumberOfCharacters()); j++)
	{
		(*this)[first_marker_character_index+number_of_marker_characters-1-j] = temp[temp.GetNumberOfCharacters()-1-j];
	}

	return 1;
}


int mxString::IsOfPattern(mxString &pattern_string, char number_marker_character, char non_number_marker_character)
{
    if(this->IsEmpty()) return 0;
    if(this->GetNumberOfCharacters()!=pattern_string.GetNumberOfCharacters()) return 0;
    
    for(unsigned int i=0; i<this->GetNumberOfCharacters(); i++)
    {
        if(pattern_string[i] == number_marker_character)
        {
            if(!((*this)[i]>='0' && (*this)[i]<='9')) { return 0; }
        }
        else
        {
            if(pattern_string[i] == non_number_marker_character)
            {
                if((*this)[i]>='0' && (*this)[i]<='9') { return 0; }
            }
            else
            {
                if(pattern_string[i] != (*this)[i]) return 0;
            }
        }
    }
    return 1;
}


void mxString::InvertCapitalization()
{
    for(unsigned int i=0; i<this->m_string.length(); i++ )
    {
        if(islower((*this)[i])) (*this)[i] = toupper((*this)[i]);
        else (*this)[i] = tolower((*this)[i]);
    }
}


mxString& mxString::operator =(mxString &s)
{
    if (&s==this) return *this;
    this->Assign(s);
    return *this;
}


mxString& mxString::operator =(char *s)
{
	if(s==NULL) return *this;
    this->Assign(s);
    return *this;
}


mxString& mxString::operator =(const char *s)
{
	if(s==NULL) return *this;
	this->Assign(s);
    return *this;
}


int mxString::operator ==(mxString &s)
{
	if(&s==this) return 1;
    if(this->IsEmpty() && s.IsEmpty()) return 1;
    if(this->GetNumberOfCharacters()!=s.GetNumberOfCharacters()) return 0;
    
	// Note: If strings are equal, compare() method returns 0.
    if(!this->m_string.compare(s.m_string)) return 1;
	return 0;
}


int mxString::operator ==(char *s)
{
	if(s==NULL) return 0;

	// Note: If strings are equal, compare() method returns 0.
	if(!this->m_string.compare(s)) return 1;
	return 0;
}


int mxString::operator ==(const char *s)
{
	if(s==NULL) return 0;

    // Note: If strings are equal, compare() method returns 0.
    if(!this->m_string.compare(s)) return 1;
	return 0;
}


int mxString::operator !=(mxString &s)
{
    return (!((*this)==s));
}


int mxString::operator !=(char *s)
{
    return (!((*this)==s));
}


int mxString::operator !=(const char *s)
{
    return (!((*this)==s));
}


int mxString::operator <(mxString &s)
{
	if(&s==this) return 0;

	unsigned int length = this->GetNumberOfCharacters();
	if(s.GetNumberOfCharacters() < this->GetNumberOfCharacters()) length = s.GetNumberOfCharacters();

	for(unsigned int i=0; i<length; i++)
	{
        if((*this)[i] >= s[i]) { return 0; }
    }
	
	return 1;
}


int mxString::operator >(mxString &s)
{
	if(&s==this) return 0;

    unsigned int length = this->GetNumberOfCharacters();
    if(s.GetNumberOfCharacters() < this->GetNumberOfCharacters()) length = s.GetNumberOfCharacters();
    
    for(unsigned int i=0; i<length; i++)
    {
        if((*this)[i] <= s[i]) { return 0; }
    }

	return 1;
}


std::ostream& operator << (std::ostream &o, mxString &s)
{ 
	o<<s.m_string;
    return o;
}


std::stringstream& operator << (std::stringstream &o, mxString &s)
{ 
	o<<s.m_string;
    return o;
}