/*=========================================================================
 
 Program:   mipx
 Module:    mxString.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxString_USE_SOURCE_CODE)
    #define mxString_API
#else
    #if defined(_MSC_VER)
        #ifdef mxString_EXPORTS
            #define mxString_API __declspec(dllexport)
        #else
            #define mxString_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxString_EXPORTS
            #define mxString_API __attribute__((visibility("default")))
        #else
            #define mxString_API
        #endif
    #endif
#endif



#ifndef mxString_H
	#define mxString_H


#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "mxList.h"


template class mxString_API std::basic_string<char>;
template class mxString_API std::allocator<char>;


/// String class as a wrapper around std::string.

class mxString_API mxString
{
    
private:

	/// Data is a standard string.
	std::string m_string;

public:

	/// Constructor.
	mxString();

	/// Destructor.
	~mxString();

	/// Get standard string (std::string).
	std::string& GetStdString();
    
    /// Get the string as a const C-style string (null terminator is appended).
    /// Notice: the created C-style string is owned by this->m_string and should not be deleted.
    const char* Get_C_String();
    
	/// Clear the string (make it an empty string).
	void Clear();

	/// Check if this string is empty.
    int IsEmpty();
    
	/// Get number of characters in the string.
	unsigned int GetNumberOfCharacters();
    
    /// Get number of characters in the string. The same as GetNumberOfCharacters(), for those who prefer Length() in strings.
    unsigned int Length();
    
    /// Set string from an existing string.
    void Assign(mxString &s);

    /// Set string from a single character.
    void Assign(const char c);
    
    /// Set string from a c-type string.
    void Assign(const char *s);
    
    /// Set string from an existing standard string.
    void Assign(const std::string &s);
    
	/// Append string to (an existing) this string.
    void Append(mxString &s);

    /// Append character to (an existing) this string.
    void Append(const char c);
    
    /// Append c-type string to (an existing) this string.
    void Append(const char *s);
    
    /// Append standard string to (an existing) this string.
    void Append(const std::string &s);
    
    /// Convert int number to string and append to this string.
    void AppendNumber(int number);
    
    /// Convert double number to string and append to this string.
    void AppendNumber(double number);
    
	/// Convert int number to (this) string.
    void AssignNumber(int number);
	
    /// Convert double number to (this) string.
    void AssignNumber(double number, int number_of_decimals = 2);

	/// Convert string to int number.
	int ToNumber(int &output_number);
	
	/// Convert string to double number.
    int ToNumber(double &output_number);
    
    /// Convert string to 3D position coordinates.
    int ToPosition(double &output_coordinate1, double &output_coordinate2, double &output_coordinate3);
    
    /// Convert path to Windows path (replacing '/' with '\').
    void ToWindowsPath();
    
    /// Convert path to Linux path (replacing '\' with '/').
    void ToLinuxPath();

	/// Extract all int numbers to list.
	void ExtractNumbers(mxList<int> &list_of_ints);
    
    /// Extract all double numbers to list.
    void ExtractNumbers(mxList<double> &list_of_doubles);
	
    /// Extract hexadecimal strings to list.
    void ExtractHexadecimalStrings(mxList<mxString> &list_of_hexadecimal_strings);
    
    /// Extract serial numbers as strings of int numbers with dots (signes '+' and '-' will not be extracted).
    /// Example: "21 1.2.430 UI 10007.4.3.4.2.1.6" will be extracted as: "21", "1.2.430", "10007.4.3.4.2.1.6".
    void ExtractSerialsOfNumbersAndDots(mxList<mxString> &list_of_serials_of_numbers_and_dots);

	/// Extracts the last int number and gives as output the rest of the string in 'remaining_string'.
	/// Example: "data_2_00009_" will be extracted as: remaining_string = "data_2_", number = 9.
	void ExtractLastNumber(mxString &remaining_string, int &number);

	/// Extract the first encountered string containing non-int_number characters.
	/// Example: "data00001_03abc' will result in "data".
	void ExtractNonIntNumberString(mxString &extracted_string);
    
    /// Extract string between input characters.
    /// Example: For string "[1, -5, 2]", start_character '[', end_character ']', the output is: "1, -5, 2".
    int ExtractString(char start_character, char end_character, mxString &output);

    /// Extract strings between input characters to list.
    /// Example: For string "{asc}{-5}{op2}", start_character '{', end_character '}', the output is: "asc","-5","op2".
    int ExtractStrings(char start_character, char end_character, mxList<mxString> &output_list);
    
    /// Extract the first string that is common for this and input string.
    /// Example: For "data_0324asd" and "data_1224", the output is: "data_".
    int ExtractCommonString(mxString &input, mxString &output);
    
    /// Extract the first string that is different for this and input string (difference is copied from this string).
    /// Example: For "str_first", input_string: "str_second", the output is: "first".
    int ExtractDifferenceString(mxString &input_string, mxString &output_difference_string);
    
    /// Remove the extension from the file name/path.
    /// Example: For "x:\dir\file.png" extracts: "x:\dir\file" and "png".
    int ExtractFileNameAndFileExtension(mxString &output_file_name_with_path, mxString &output_extension);

    /// Extract file path parts: directory, file name and extension.
    /// Example: For "x:\dir\file.png" extracts: "x:\dir\", "file" and "png". ('\' can be replaced with '/').
    int ExtractFilePathParts(mxString &output_dir, mxString &output_file_name, mxString &output_extension);
    
    /// For path string get the path of directory level up.
    /// Example: For "x:\dir\dir2\" level up is: "x:\dir\". ('\' can be replaced with '/').
    int PathLevelUp(mxString &output_dir);
    
    static char PathSeparator()
    {
    #if defined(_MSC_VER)
            return '\\';// for Microsoft Windows.
    #else
            return '/';// for Unix-type systems.
    #endif
    };

	/// Replace one input character with the other one.
	void ReplaceCharacters(char character_to_replace, char replacement_character);
    
	/// Inserts a number at places marked with marker_character.
    /// Example: For "file###_asd", n=52, marker_character='#', output is: "file052_asd".
	int InsertNumber(int n, char marker_character);
    
    /// Check if the string has pattern of pattern_string, where number_marker_character indicates any number in the original
    /// string and non_number_marker_character indicates any non-number character in the original string.
    /// Example: "file_%##%", number_marker_character='#', non_number_marker_character='%', covers strings: "file_w01g", "file_c64v", "file_t53g".
    int IsOfPattern(mxString &pattern_string, char number_marker_character, char non_number_marker_character);
    
    /// Inverts capitalization of characters (non-capital become capital and capital become non-capital).
    /// Example: "New York" becomes: "nEW yORK".
    void InvertCapitalization();

    /// Indexing operator.
	char& operator [](int r){return this->m_string[r];};
    
	/// Assignment operator.
	mxString& operator =(mxString &s);
	mxString& operator =(char *s);
	mxString& operator =(const char *s);
	
    /// Equality operator.
	int operator ==(mxString &s);
	int operator ==(char *s);
	int operator ==(const char *s);

    /// Non-equality operator.
	int operator !=(mxString &s);
	int operator !=(char *s);
    int operator !=(const char *s);

    /// Comparison operators.
	int operator <(mxString &s);
	int operator >(mxString &s);

    /// Printing friend operators.
	mxString_API friend std::ostream& operator << (std::ostream &o, mxString &s);
	mxString_API friend std::stringstream& operator << (std::stringstream &o, mxString &s);
};


#endif