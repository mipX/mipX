/*=========================================================================
 
 Program:   mipx
 Module:    mxFileOperations.h

 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#ifndef mxFileOperations_H
	#define mxFileOperations_H


#include "mxString.h"

#include <iostream>
#include <cassert>

#include <sstream>
#include <sys/stat.h>
#include <fstream>


// For windows 'mkdir'
#ifdef _WIN32
#include <direct.h>
#endif




namespace mxFileOperations
{

    /// Copy input to output file.
    void CopyFile(const char *input, const char *output)
    {
        std::ifstream  src(input, std::ios::binary);
        std::ofstream  dst(output,   std::ios::binary);
        dst << src.rdbuf();
    };
    
    /// Check if a file exists (and is accessible).
    int IsFileCreated(const char *filename)
    {
        if (FILE *file = fopen(filename, "r"))
        {
            fclose(file);
            return 1;
        }
        else
        {
            return 0;
        }

    };
    
    
    /// DOES NOT WORK ON MAC FOR SOME REASON!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /// Check if a folder exists. path can end with separator ('/' or '\\'), but also without it.
    /// Return TRUE if folder exists, otherwise FALSE.
    int IsFolderCreated(const char *foldername)
    {
        std::cout<<std::endl<<"IsFolderCreated: '"<<foldername<<"'";
//        mxString s;
//        s.Assign(foldername);
//
//        mxString s_folder;
//
//        if(s[s.GetNumberOfCharacters()-1] == '/' || s[s.GetNumberOfCharacters()-1] == '\\')
//        {
//            for(int i=0; i<s.GetNumberOfCharacters()-1; i++)
//            {
//                s_folder.Append(s[i]);
//            }
//        }
//        else
//        {
//            s_folder.Assign(foldername);
//        }
        
        struct stat st;
        stat(foldername, &st);//stat(s_folder.Get_C_String(), &st);
        
        return ( st.st_mode & S_IFDIR ) ? 1 : 0;
    };
    
    
//    /**
//     * Portable wrapper for mkdir. Internally used by Mkdir()
//     * @param[in] path the full path of the directory to create.
//     * @return zero on success, otherwise -1.
//     */
//    int _mkdir(const char *path)
//    {
//#ifdef _WIN32
//        return ::_mkdir(path);
//#else
//#if _POSIX_C_SOURCE
//        return ::mkdir(path);
//#else
//        // for MAC
//        return ::mkdir(path, 0777); //0755);
//#endif
//#endif
//    };
    
    
    /**
     * Recursive, portable wrapper for mkdir.
     * @param[in] path the full path of the directory to create.
     * @return zero on success, otherwise -1.
     */
    int MakeDirectory(const char *path)
    {
#ifdef _WIN32
        return ::_mkdir(path);
#else
#if _POSIX_C_SOURCE
        return ::mkdir(path);
#else
        // for MAC
        return ::mkdir(path, 0777); //0755);
#endif
#endif
        
        return 1;
        
        
//        std::string current_level = "";
//        std::string level;
//        std::stringstream ss(path);
//
//        // split path using slash as a separator
//        while (std::getline(ss, level, '/'))
//        {
//            current_level += level; // append folder to the current level
//
//            // create current level
//            if (!FolderExists(current_level) && _mkdir(current_level.c_str()) != 0)
//                return -1;
//
//            current_level += "/"; // don't forget to append a slash
//        }
//
//        return 0;
    };

    
};



#endif
