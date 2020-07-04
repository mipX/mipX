/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppFunctionInterface.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxAppFunctionInterface_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppFunctionInterface_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppFunctionInterface_EXPORTS
            #define vmxAppFunctionInterface_API __declspec(dllexport)
        #else
            #define vmxAppFunctionInterface_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppFunctionInterface_EXPORTS
            #define vmxAppFunctionInterface_API __attribute__((visibility("default")))
        #else
            #define vmxAppFunctionInterface_API
        #endif
    #endif
#endif


#ifndef vmxAppFunctionInterface_H
    #define vmxAppFunctionInterface_H





#include "mxList.h"
#include "mxString.h"
#include "mxFunctionConfig.h"
#include "mxDataContainer.h"


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



//pre-declare class.
class vmxAppMainWidget;



/// Contains the function abort indicator to communicate to the main app that the function was aborted.

class vmxAppFunctionInterface_API vmxAppFunctionInterfaceAbortIndicator
{
public:
    
    /// Abort indicator.
    int m_is_abort_requested;
    
    /// Constructor.
    vmxAppFunctionInterfaceAbortIndicator()
    {
        m_is_abort_requested = 0;
    };
    
    /// Destructor.
    ~vmxAppFunctionInterfaceAbortIndicator()
    {
    };
};



/// Interface for function connected to the main app.

class vmxAppFunctionInterface_API vmxAppFunctionInterface
{
public:
    
    /// Version of the interface.
    mxString m_version;

//    /// Contains the version of the interface.
//    vmxAppFunctionInterfaceVersion m_interface_version;
    
    /// Abort indicator.
    vmxAppFunctionInterfaceAbortIndicator m_abort_indicator;

	/// Function configuration.
	mxFunctionConfig m_function_config;

	/// Data container object. Data is written/read to/from this object.
	mxDataContainer m_data_container;
    
    /// Pointer to the app main widget.
    vmxAppMainWidget *m_app_main_widget;

//    /// Declared batch command.
//    QString m_batch_object;
//    QString m_batch_object_index;
//    QString m_batch_command;
//    QString m_batch_command_parameters;
    
    
    /// Constructor.
    vmxAppFunctionInterface() { m_app_main_widget = NULL; };
    
    /// Destructor.
    virtual ~vmxAppFunctionInterface() {};
    
    /// Get pointer to app main widget.
    vmxAppMainWidget* GetAppMainWidget() { return m_app_main_widget; };
    
	/// Get function configuration object.
	mxFunctionConfig* GetConfig() { return &m_function_config; };

	/// Get data container object.
	mxDataContainer* GetData() { return &m_data_container; };
    
	/// Get version string of the interface.
	mxString GetVersion() { return m_version; };
    
    /// Request Abort of function.
    void Abort()
    {
        m_abort_indicator.m_is_abort_requested = 1;
    };
    
//    /// Use this method to declare the batch command.
//    /// 'object' is "ObjectName"
//    /// 'index' is "INT" or "NONE"
//    /// 'command' is "CommandName"
//    /// 'parameters' is combination of "|INT||DOUBLE||STRING||Image||Skeleton||PositionList|....|Table|"
//    void DeclareBatchCall(const char *object, const char *index, const char *command, const char *parameters)
//        { m_batch_object.append(object); m_batch_object_index.append(index); m_batch_command.append(command); m_batch_command_parameters.append(parameters); };
    
    /// ONLY 2 things should be placed here: 1. extracting data from data container, 2. starting execution.
    /// Do not put anything else here, this method is executed also in Batch.
    virtual void Execute(){};

	/// Use for code that has to be executed in the main thread, e.g. rendering.
    /// Call here Execute() with other needed methods.
	virtual void StartInMainThread()
    {
        //std::cout<<std::cout<<"vmxAppFunctionInterface::StartInMainThread() was called. ";
    };

	/// Use for code that will be executed as a separate thread (not main).
	/// Call here Execute() with other needed methods.
    virtual void StartAsNewThread(){};

	/// Steps of execution.
	virtual void Step_0(){};
	virtual void Step_1(){};
	virtual void Step_2(){};
	virtual void Step_3(){};

};


/// Interface of a function factory, i.e. this is function factory base class.

class vmxAppFunctionInterface_API vmxAppFunctionFactoryInterface
{
public:
    
	/// Use to create the instance of the plugin.
	virtual vmxAppFunctionInterface* Create() = 0;

	/// Use to delete an instance of a plugin.
	virtual void Release(vmxAppFunctionInterface* function_interface) = 0;
};



//--------------------------------------------------------------------------------------------------------------------------------



/// Template of function factory interface. Allows easy definition of new interfaces.

template<class T>
class vmxAppFunctionFactoryInterfaceT : public vmxAppFunctionFactoryInterface
{
public:

	/// Use to create the instance of the plugin.
	T* Create() { return (new T()); };

	/// Use to delete an instance of a plugin.
	void Release(vmxAppFunctionInterface* function_interface)
    {
        delete ((T*)(function_interface));
    };
};



//--------------------------------------------------------------------------------------------------------------------------------



/// List of function factories. Use this structure to load multiple function factories.

class vmxAppFunctionInterface_API vmxAppFunctionFactoryList
{
public:
    
    /// List of pointers to function factories.
    mxList< vmxAppFunctionFactoryInterface* > m_list_of_function_factories;
    
    /// Constructor.
    vmxAppFunctionFactoryList() {};
    
    /// Destructor.
    ~vmxAppFunctionFactoryList() {};
};



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




#endif
