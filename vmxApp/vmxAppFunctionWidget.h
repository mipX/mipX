/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppFunctionWidget.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxAppFunctionWidget_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppFunctionWidget_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppFunctionWidget_EXPORTS
            #define vmxAppFunctionWidget_API __declspec(dllexport)
        #else
            #define vmxAppFunctionWidget_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppFunctionWidget_EXPORTS
            #define vmxAppFunctionWidget_API __attribute__((visibility("default")))
        #else
            #define vmxAppFunctionWidget_API
        #endif
    #endif
#endif


#ifndef vmxAppFunctionWidget_H
    #define vmxAppFunctionWidget_H

#include "vmxGUIButtonGroup.h"
#include "vmxGUIWidget.h"
#include "vmxGUILabel.h"
#include "vmxGUIInputWidget.h"

#include "vmxAppWidget.h"
#include "vmxAppFunctionInterface.h"

#include "mxDataContainer.h"
#include "mxFunctionConfig.h"
#include "mxString.h"



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



// Pre-declare the classes.
class vmxAppFunctionWidget;
class vmxAppMainWidget;






//----------------------------------------------------------------------------------------------------------



class vmxAppFunctionWidget_API vmxAppFunctionDataInputWidget : public vmxGUIWidgetCollection
{
public:
    
#define vmxAppFunctionDataInputWidget_MAX_NUMBER_OF_INPUTS 8
    
    /// Pointer to container function widget.
    vmxAppFunctionWidget *m_function_widget;
    
    /// Input widget.
    vmxGUIInputWidget m_input_widget[vmxAppFunctionDataInputWidget_MAX_NUMBER_OF_INPUTS];
    
    /// Actual number of data inputs in this widget (after reading the function config structure).
    unsigned int m_number_of_data_inputs;
    
    /// Constructor.
    vmxAppFunctionDataInputWidget()
    {
        m_function_widget = NULL;
        m_number_of_data_inputs = 0;
    };
    
    /// Destructor.
    ~vmxAppFunctionDataInputWidget() {};
    
    /// Setup the widget.
    void Setup(vmxAppFunctionWidget *function_widget);
    
    /// Loads function configuration step. Return value 0 indicates that nothing is scheduled for loading
    /// (so the tab will not be shown), while any non-zero value indicates that the tab has something to show.
    int LoadConfigurationStep(mxFunctionConfigStep *config_step);
    
    /// Loads the data in the given step.
    int LoadDataContainerStep(mxFunctionConfigStep *config_step, mxDataContainerStep *container_step);
};




//----------------------------------------------------------------------------------------------------------



class vmxAppFunctionWidget_API vmxAppFunctionOptionsWidget : public vmxGUIWidgetCollection
{
public:
    
#define vmxAppFunctionOptionsWidget_MAX_NUMBER_OF_INPUTS 4
    
    /// Pointer to the function widget.
    vmxAppFunctionWidget *m_function_widget;
    
    /// Checkboxes.
    vmxGUIButtonGroup m_check_boxes[vmxAppFunctionOptionsWidget_MAX_NUMBER_OF_INPUTS];
    
    /// Actual number of checkbox inputs in this widget (after reading the function config structure).
    unsigned int m_number_of_checkbox_inputs;

    /// Checkboxes.
    vmxGUIButtonGroup m_radio_buttons[vmxAppFunctionOptionsWidget_MAX_NUMBER_OF_INPUTS];
    
    /// Actual number of radio button inputs in this widget (after reading the function config structure).
    unsigned int m_number_of_radiobutton_inputs;


    
    /// Constructor.
    vmxAppFunctionOptionsWidget()
    {
        m_number_of_checkbox_inputs = 0;
        m_number_of_radiobutton_inputs = 0;
        m_function_widget = NULL;
    };
    
    /// Destructor.
    ~vmxAppFunctionOptionsWidget() {};
    
    /// Setup the widget.
    void Setup(vmxAppFunctionWidget *function_widget);
    
    /// Loads function configuration step. Return value 0 indicates that nothing is scheduled for loading
    /// (so the tab will not be shown), while any non-zero value indicates that the tab has something to show.
    int LoadConfigurationStep(mxFunctionConfigStep *config_step);
    
    /// Loads the data in the given step.
    int LoadDataContainerStep(mxFunctionConfigStep *config_step, mxDataContainerStep *container_step);
};



//----------------------------------------------------------------------------------------------------------





class vmxAppFunctionWidget_API vmxAppFunctionParametersWidget : public vmxGUIWidgetCollection
{
public:
    
#define vmxAppFunctionParametersWidget_MAX_NUMBER_OF_INPUTS 8

    /// Pointer to container function widget.
    vmxAppFunctionWidget *m_function_widget;
    
    /// Input widget.
    vmxGUIInputWidget m_input_int_widgets[vmxAppFunctionParametersWidget_MAX_NUMBER_OF_INPUTS];
    
    /// Actual number of inputs in this widget (after reading the function config structure).
    unsigned int m_number_of_int_inputs;

    /// Input widget.
    vmxGUIInputWidget m_input_double_widgets[vmxAppFunctionParametersWidget_MAX_NUMBER_OF_INPUTS];
    
    /// Actual number of inputs in this widget (after reading the function config structure).
    unsigned int m_number_of_double_inputs;

    
    /// Constructor.
    vmxAppFunctionParametersWidget()
    {
        m_function_widget = NULL;
        m_number_of_int_inputs = 0;
        m_number_of_double_inputs = 0;
    };
    
    /// Destructor.
    ~vmxAppFunctionParametersWidget() {};
    
    /// Setup the widget.
    void Setup(vmxAppFunctionWidget *function_widget);
    
    /// Loads function configuration step. Return value 0 indicates that nothing is scheduled for loading
    /// (so the tab will not be shown), while any non-zero value indicates that the tab has something to show.
    int LoadConfigurationStep(mxFunctionConfigStep *config_step);
    
    /// Loads the data in the given step.
    int LoadDataContainerStep(mxFunctionConfigStep *config_step, mxDataContainerStep *container_step);

};




//----------------------------------------------------------------------------------------------------------




class vmxAppFunctionWidget_API vmxAppFunctionStringsWidget : public vmxGUIWidgetCollection
{
public:
    
#define vmxAppFunctionStringsWidget_MAX_NUMBER_OF_INPUTS 8
    
    /// Pointer to container function widget.
    vmxAppFunctionWidget *m_function_widget;
    
    /// Input widget.
    vmxGUIInputWidget m_input_string_widgets[vmxAppFunctionStringsWidget_MAX_NUMBER_OF_INPUTS];
    
    /// Actual number of inputs in this widget (after reading the function config structure).
    unsigned int m_number_of_string_inputs;
    
    
    /// Constructor.
    vmxAppFunctionStringsWidget()
    {
        m_function_widget = NULL;
        m_number_of_string_inputs = 0;
    };
    
    /// Destructor.
    ~vmxAppFunctionStringsWidget() {};
    
    /// Setup the widget.
    void Setup(vmxAppFunctionWidget *function_widget);
    
    /// Loads function configuration step. Return value 0 indicates that nothing is scheduled for loading
    /// (so the tab will not be shown), while any non-zero value indicates that the tab has something to show.
    int LoadConfigurationStep(mxFunctionConfigStep *config_step);
    
    /// Loads the data in the given step.
    int LoadDataContainerStep(mxFunctionConfigStep *config_step, mxDataContainerStep *container_step);

};




//----------------------------------------------------------------------------------------------------------





class vmxAppFunctionWidget_API vmxAppFunctionWidget : public vmxGUIWidgetCollection
{
public:
    
    /// Pointer to the main app.
    vmxAppMainWidget *m_app_main_widget;

	/// Pointer to function interface of the initialized function.
	vmxAppFunctionInterface *m_function_interface;

	/// Pointer to function FACTORY interface of the initialized function.
	vmxAppFunctionFactoryInterface *m_function_factory_interface;

//	/// Indicates if abort was requested during execution. It is used to determine if the new data should
//	/// be removed after execution or updated in other widgets.
//	int m_is_abort_requested;


//// Indicates if the function widget is allowed to close - the function widget is allowed to close only
//// when the MAIN thread function finishes with execution.
//int is_allowed_to_close;

    /// title label.
    vmxGUILabel m_function_title_label;
    
    /// Font size.
    int m_font_size;
    
    /// Color in RGB.
    double m_color[3];


   
    /// Data input widget.
    vmxAppFunctionDataInputWidget m_data_input_widget;
    
    /// Options widget.
    vmxAppFunctionOptionsWidget m_options_widget;

    /// Parameters widget.
    vmxAppFunctionParametersWidget m_parameters_widget;

    /// Strings widget.
    vmxAppFunctionStringsWidget m_strings_widget;

//    /// Help widget.
//    vmxAppFunctionHelpWidget m_help_widget;
//
//    /// Progress widget.
//    vmxAppFunctionProgressWidget m_progress_widget;
//
//    /// Results widget.
//    vmxAppFunctionResultsWidget m_results_widget;


	/// Push buttons: Start, Next, Close, Back.
    vmxGUIButtonGroup m_push_buttons;
    vmxGUIButtonGroupItem *m_push_button_item_start;
    vmxGUIButtonGroupItem *m_push_button_item_next;
    vmxGUIButtonGroupItem *m_push_button_item_close;
    vmxGUIButtonGroupItem *m_push_button_item_back;
    //vmxGUIButtonGroupItem *m_push_button_item_cancel;


	/// Constructor.
    vmxAppFunctionWidget();

	/// Destructor.
	virtual ~vmxAppFunctionWidget();
    
    /// Setup this object.
    void Setup(vmxAppFunctionFactoryInterface *function_factory_interface, vmxAppFunctionInterface *function_interface);

    /// Set the pointer to the main widget.
    void SetAppMainWidget(vmxAppMainWidget *app_main_widget);
    
    /// Get the pointer to the app level main widget.
    vmxAppMainWidget* GetAppMainWidget() { return m_app_main_widget; };

//    /// Return the value of the abort indicator.
//    int IsAbortRequested();
//
//    /// Set the indicator if the abort was requested.
//    void SetAbortRequested(int is_abort_requested);

	/// Load the configuration for the current step of function. Return 1 for success, 0 for fail.
	int LoadConfiguration(mxFunctionConfig *config);

	/// Load the data container based on the input function config and already loaded function configuration. 
	int LoadDataContainer(mxFunctionConfig *input_config, mxDataContainer *data_container, unsigned int step);

//    /// Unload the data container for the given step index. This method RELEASES (from memory) all NEW data for the current step and
//    /// removes (NOT release) the existing data from the list.
//    void UnloadDataContainer(unsigned int step);

//	/// Remove all NEW data for all steps. Use this when cancelling (aborting) the function.
//	void RemoveAllNewData();

//    /// Check if any of the loaded data is locked.
//    int IsDataLocked();
//
//    /// Lock all loaded data.
//    void LockData();
//
//    /// Unlock all loaded data.
//    void UnlockData();

	/// Removes all tabs from the function widget.
	void RemoveAllTabs();

//    /// Shows progress tab
//    void ShowProgressTab();
//
//    /// Show input strings tab.
//    void ShowStringsTab();
//
//    /// Shows results tab
//    void ShowResultsTab(const QString &text);
//    void ShowResultsTab();
//
//    /// Shows user info tab and closes all other tabs that were open. All buttons are removed.
//    void ShowUserInfoTab(const QString &text);
//
//    /// Show or hide the Back push button.
//    void SetBackButtonVisibility(int is_visible =1 );
//
//    /// Show or hide the Start/Next push button.
//    void SetStartOrNextButtonVisibility(int is_visible =1 );
//
//    /// Show or hide the Close push button.
//    void SetCloseButtonVisibility(int is_visible =1 );
//
//    /// Show or hide the Cancel push button.
//    void SetCancelButtonVisibility(int is_visible =1 );

    
    /// Slot to be executed when a puch button from is pressed.
    static int Slot_PushButtonPressed(vmxGUIConnection *connection);

};


#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif



#endif
