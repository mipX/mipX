/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppReportWidget.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxAppReportWidget_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppReportWidget_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppReportWidget_EXPORTS
            #define vmxAppReportWidget_API __declspec(dllexport)
        #else
            #define vmxAppReportWidget_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppReportWidget_EXPORTS
            #define vmxAppReportWidget_API __attribute__((visibility("default")))
        #else
            #define vmxAppReportWidget_API
        #endif
    #endif
#endif


#ifndef vmxAppReportWidget_H
    #define vmxAppReportWidget_H



#include "tinyfiledialogs.h"


#include "vmxAppFunctionInterface.h"
#include "vmxAppWidget.h"
#include "vmxAppImageViewer.h"
#include "vmxGraph.h"
#include "vmxImage.h"
#include "vmxMesh.h"
#include "vmxProfile.h"
#include "vmxSkeleton.h"



//-------------------------------------------------------------------------------------------------------------------------


//#include <experimental/filesystem>

#include <sstream>
#include <sys/stat.h>
#include <fstream>

// for windows mkdir
#ifdef _WIN32
#include <direct.h>
#endif




//-------------------------------------------------------------------------------------------------------------------------




class vmxAppReportWidget_API vmxAppReportGenerator// : public vmxGUIWidgetCollection
{
public:
    
    void CopyFile(const char *input, const char *output)
    {
        std::ifstream  src(input, std::ios::binary);
        std::ofstream  dst(output,   std::ios::binary);
        dst << src.rdbuf();
    };
    
    
    /**
     * Checks if a folder exists
     * @param foldername path to the folder to check.
     * @return true if the folder exists, false otherwise.
     */
    bool FolderExists(std::string foldername)
    {
        struct stat st;
        stat(foldername.c_str(), &st);
        return st.st_mode & S_IFDIR;
    };
    
    
    /**
     * Portable wrapper for mkdir. Internally used by mkdir()
     * @param[in] path the full path of the directory to create.
     * @return zero on success, otherwise -1.
     */
    int _mkdir(const char *path)
    {
#ifdef _WIN32
        return ::_mkdir(path);
#else
#if _POSIX_C_SOURCE
        return ::mkdir(path, 0777); // For LINUX GCC
#else
        return ::mkdir(path, 0777);//0755); // not sure if this works on mac
#endif
#endif
    };
    
    
    /**
     * Recursive, portable wrapper for mkdir.
     * @param[in] path the full path of the directory to create.
     * @return zero on success, otherwise -1.
     */
    int Mkdir(const char *path)
    {
        #ifdef _WIN32
            return ::_mkdir(path);
        #else
            #if _POSIX_C_SOURCE
                return ::mkdir(path, 0777);
            #else
                return ::mkdir(path, 0777);//0755); // not sure if this works on mac
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
    

    
    /// Pointer to the main app.
    vmxAppMainWidget *m_app_main_widget;

//	/// Pointer to function interface of the initialized function.
//	vmxAppFunctionInterface *m_function_interface;

//	/// Pointer to function FACTORY interface of the initialized function.
//	vmxAppFunctionFactoryInterface *m_function_factory_interface;





	/// Constructor.
    vmxAppReportGenerator();

	/// Destructor.
	virtual ~vmxAppReportGenerator();
    
 //   /// Setup this object.
 //   void Setup(vmxAppFunctionFactoryInterface *function_factory_interface, vmxAppFunctionInterface *function_interface);

    /// Set the pointer to the main widget.
    void SetAppMainWidget(vmxAppMainWidget *app_main_widget);
    
    /// Get the pointer to the app level main widget.
    vmxAppMainWidget* GetAppMainWidget() { return m_app_main_widget; };


    /// Create an XTK type WebGL visualization.
    void Create_XTK_ReportForGraph(vmxGraph *graph, const char *file_name);

    /// Create an XTK type WebGL visualization.
    void Create_XTK_ReportForSkeleton(vmxSkeleton *skeleton, const char *file_name);

};




#endif
