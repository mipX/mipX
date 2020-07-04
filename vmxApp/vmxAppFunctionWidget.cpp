/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppFunctionWidget.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppFunctionWidget.h"




//-------------------------------------------------------------------------------------------------------



void vmxAppFunctionDataInputWidget::Setup(vmxAppFunctionWidget *function_widget)
{
    m_function_widget = function_widget;
    
    for(int i=0; i<vmxAppFunctionDataInputWidget_MAX_NUMBER_OF_INPUTS; i++)
    {
        m_input_widget[i].SetMainWidget(function_widget->GetMainWidget());
        m_input_widget[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
        m_input_widget[i].SetFontSize(m_function_widget->m_font_size);
        m_input_widget[i].SetPlacementToLowerLeft();
        m_input_widget[i].SetPlacementOrderToBelow();
        m_input_widget[i].SetInputTypeToItem();
        m_input_widget[i].SetInteractor(m_function_widget->GetAppMainWidget()->GetInteractor());
        m_input_widget[i].SetVisibility(0);
        //    this->AddWidget(&m_input_widget[i]);
    }
}


int vmxAppFunctionDataInputWidget::LoadConfigurationStep(mxFunctionConfigStep *config_step)
{
    if(config_step->m_data_objects.IsEmpty()) return 0;
    
    this->SetVisibility(0);
    
    for(int i=0; i<vmxAppFunctionDataInputWidget_MAX_NUMBER_OF_INPUTS ; i++)
    {
        this->RemoveWidget(&(m_input_widget[i]));
    }
    
    {
        m_number_of_data_inputs = config_step->m_data_objects.GetNumberOfElements();
        
        mxListIterator< mxFunctionConfigStepDataObjectInput > it;
        int i=0;
        for(it.SetToBegin(config_step->m_data_objects), i=0; it.IsValid() && i<vmxAppFunctionDataInputWidget_MAX_NUMBER_OF_INPUTS; it.MoveToNext(), i++)
        {
            m_input_widget[i].SetDescriptionText(it.GetElementAddress()->m_text.Get_C_String());
            m_input_widget[i].ShowInputText();
            
            this->AddWidget(&m_input_widget[i]);
        }
    }
    
    
//
//
//    m_input_widget.SetDescriptionText(config_step->m_data_objects.GetBeginElementAddress()->m_text.Get_C_String());//" >> ");
//    m_input_widget.ShowInputText();
    
    return 1;
}


int vmxAppFunctionDataInputWidget::LoadDataContainerStep(mxFunctionConfigStep *config_step, mxDataContainerStep *container_step)
{
    // get all items (names of data) that have been entered
    for(int i=0; i< (int)m_number_of_data_inputs; i++)
    {
        mxList<mxString> items_list;
        m_input_widget[i].GetInputItems(items_list);
        
        //... find data that corresponds to these names in the data tree list
        mxListIterator< mxString > it;
        for(it.SetToBegin(items_list); it.IsValid(); it.MoveToNext())
        {
            mxDataObject *obj = this->m_function_widget->GetAppMainWidget()->GetDataListWidget()->GetDataObject(it.GetElementAddress()->Get_C_String());
            container_step->m_data_objects.AddToEnd(obj);
        }
    }
    
    return 1;
}



//-------------------------------------------------------------------------------------------------------




void vmxAppFunctionOptionsWidget::Setup(vmxAppFunctionWidget *function_widget)
{
    m_function_widget = function_widget;
    
    for(int i=0; i<vmxAppFunctionOptionsWidget_MAX_NUMBER_OF_INPUTS; i++)
    {
        m_check_boxes[i].SetMainWidget(function_widget->GetMainWidget());
        m_check_boxes[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
        m_check_boxes[i].SetFontSize(m_function_widget->m_font_size);
        m_check_boxes[i].SetPlacementToLowerLeft();
        m_check_boxes[i].SetInteractor(m_function_widget->GetAppMainWidget()->GetInteractor());
        m_check_boxes[i].SetButtonTypeToCheckBox();
        m_check_boxes[i].SetVisibility(0);
        
        //this->AddWidget(&m_check_boxes[i]);
    }
    
    for(int i=0; i<vmxAppFunctionOptionsWidget_MAX_NUMBER_OF_INPUTS; i++)
    {
        m_radio_buttons[i].SetMainWidget(function_widget->GetMainWidget());
        m_radio_buttons[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
        m_radio_buttons[i].SetFontSize(m_function_widget->m_font_size);
        m_radio_buttons[i].SetPlacementToLowerLeft();
        m_radio_buttons[i].SetInteractor(m_function_widget->GetAppMainWidget()->GetInteractor());
        m_radio_buttons[i].SetButtonTypeToRadioButton();
        m_radio_buttons[i].SetVisibility(0);

        //this->AddWidget(&m_radio_buttons[i]);
    }
}


int vmxAppFunctionOptionsWidget::LoadConfigurationStep(mxFunctionConfigStep *config_step)
{
    //std::cout<<std::endl<<"vmxAppFunctionOptionsWidget::LoadConfigurationStep()";
    
    if(config_step->m_check_box_options.IsEmpty() && config_step->m_radio_button_options.IsEmpty()) return 0;
    
    
    this->SetVisibility(0);
    
    for(int i=0; i<vmxAppFunctionOptionsWidget_MAX_NUMBER_OF_INPUTS ; i++)
    {
        this->RemoveWidget(&(m_check_boxes[i]));
        this->RemoveWidget(&(m_radio_buttons[i]));
        
        //m_check_boxes[i].SetVisibility(0);
        m_check_boxes[i].m_items.Reset();
        
        //m_radio_buttons[i].SetVisibility(0);
        m_radio_buttons[i].m_items.Reset();
    }
    
    {
        m_number_of_checkbox_inputs = config_step->m_check_box_options.GetNumberOfElements();
        
        
        mxListIterator< mxFunctionConfigStepCheckBoxInput > it;
        int i=0;
        for(it.SetToBegin(config_step->m_check_box_options), i=0; it.IsValid() && i<vmxAppFunctionOptionsWidget_MAX_NUMBER_OF_INPUTS; it.MoveToNext(), i++)
        {
            mxListIterator< mxString > its;
            for(its.SetToBegin( it.GetElementAddress()->m_list_of_options); its.IsValid(); its.MoveToNext() )
            {
                m_check_boxes[i].AddItem(its.GetElementAddress()->Get_C_String());
            }
            
            m_check_boxes[i].SetDescriptionText(it.GetElementAddress()->m_text.Get_C_String());
            m_check_boxes[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
            m_check_boxes[i].SetFontSize(m_function_widget->m_font_size);
            //m_check_boxes[i].SetVisibility(1);
            
            this->AddWidget(&m_check_boxes[i]);
        }
    }

    {
        m_number_of_radiobutton_inputs = config_step->m_radio_button_options.GetNumberOfElements();
        mxListIterator< mxFunctionConfigStepRadioButtonInput > it;
        int i=0;
        for(it.SetToBegin(config_step->m_radio_button_options), i=0; it.IsValid() && i<vmxAppFunctionOptionsWidget_MAX_NUMBER_OF_INPUTS; it.MoveToNext(), i++)
        {
            mxListIterator< mxString > its;
            int j=0;
            for(its.SetToBegin( it.GetElementAddress()->m_list_of_options), j=0; its.IsValid(); its.MoveToNext(), j++ )
            {
                if(j==it.GetElementAddress()->m_index_of_default_checked_option)
                {
                    m_radio_buttons[i].AddItem(its.GetElementAddress()->Get_C_String(), 1);
                }
                else
                {
                    m_radio_buttons[i].AddItem(its.GetElementAddress()->Get_C_String());
                }
            }
            
            m_radio_buttons[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
            m_radio_buttons[i].SetFontSize(m_function_widget->m_font_size);
            //m_radio_buttons[i].SetVisibility(1);
            
            this->AddWidget(&m_radio_buttons[i]);
        }
    }

     return 1;
}



int vmxAppFunctionOptionsWidget::LoadDataContainerStep(mxFunctionConfigStep *config_step, mxDataContainerStep *container_step)
{
    for(int i=0; i< (int)m_number_of_checkbox_inputs; i++)
    {
        mxArray<vmxGUIButtonGroupItem *> array_of_checked_items;
        m_check_boxes[i].GetCheckedItems(array_of_checked_items);
        
        mxList<mxString> list_of_chosen_options;
        for(int j=0; j< (int)array_of_checked_items.GetNumberOfElements(); j++)
        {
            list_of_chosen_options.AddToEnd(array_of_checked_items[j]->m_text);
        }
        
        container_step->m_check_box_titles_of_options.AddToEnd(m_check_boxes[i].m_description_text);
        container_step->m_check_box_chosen_options.AddToEnd(list_of_chosen_options);
    }

    
    for(int i=0; i< (int)m_number_of_radiobutton_inputs; i++)
    {
        mxArray<vmxGUIButtonGroupItem *> array_of_checked_items;
        m_radio_buttons[i].GetCheckedItems(array_of_checked_items);
        
        if(!array_of_checked_items.IsEmpty())
        {
            container_step->m_radio_button_chosen_options.AddToEnd(array_of_checked_items[0]->m_text);
        }
        container_step->m_radio_button_titles_of_options.AddToEnd(m_radio_buttons[i].m_description_text);
    }

    return 1;
}




//-------------------------------------------------------------------------------------------------------




void vmxAppFunctionParametersWidget::Setup(vmxAppFunctionWidget *function_widget)
{
    m_function_widget = function_widget;
    
    for(int i=0; i<vmxAppFunctionParametersWidget_MAX_NUMBER_OF_INPUTS; i++)
    {
        m_input_int_widgets[i].SetMainWidget(function_widget->GetMainWidget());
        m_input_int_widgets[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
        m_input_int_widgets[i].SetFontSize(m_function_widget->m_font_size);
        m_input_int_widgets[i].SetPlacementToLowerLeft();
        m_input_int_widgets[i].SetInteractor(m_function_widget->GetAppMainWidget()->GetInteractor());
        m_input_int_widgets[i].SetInputTypeToIntegerNumber(0);// SetInputTypeToDoubleNumber(0);
        m_input_int_widgets[i].SetVisibility(0);
        
        //this->AddWidget(&m_input_int_widgets[i]);
    }
    for(int i=0; i<vmxAppFunctionParametersWidget_MAX_NUMBER_OF_INPUTS; i++)
    {
        m_input_double_widgets[i].SetMainWidget(function_widget->GetMainWidget());
        m_input_double_widgets[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
        m_input_double_widgets[i].SetFontSize(m_function_widget->m_font_size);
        m_input_double_widgets[i].SetPlacementToLowerLeft();
        m_input_double_widgets[i].SetInteractor(m_function_widget->GetAppMainWidget()->GetInteractor());
        m_input_double_widgets[i].SetInputTypeToDoubleNumber(0);
        m_input_double_widgets[i].SetVisibility(0);
        
        //this->AddWidget(&m_input_double_widgets[i]);
    }
}


int vmxAppFunctionParametersWidget::LoadConfigurationStep(mxFunctionConfigStep *config_step)
{
    //std::cout<<std::endl<<"vmxAppFunctionParametersWidget::LoadConfigurationStep()"<<std::endl;
    
    if(config_step->m_integers.IsEmpty() && config_step->m_doubles.IsEmpty()) return 0;
    
    //std::cout<<std::endl<<"m_integers: "<<config_step->m_integers.GetNumberOfElements()<<", m_doubles:"<<config_step->m_doubles.GetNumberOfElements()<<std::endl;
    

    this->SetVisibility(0);

    for(int i=0; i<vmxAppFunctionParametersWidget_MAX_NUMBER_OF_INPUTS ; i++)
    {
        this->RemoveWidget(&(m_input_int_widgets[i]));
        this->RemoveWidget(&(m_input_double_widgets[i]));
    }
    
    {
        m_number_of_int_inputs = config_step->m_integers.GetNumberOfElements();

        mxListIterator< mxFunctionConfigStepIntegerInput > it;
        int i=0;
        for(it.SetToBegin(config_step->m_integers), i=0; it.IsValid() && i<vmxAppFunctionParametersWidget_MAX_NUMBER_OF_INPUTS; it.MoveToNext(), i++)
        {
            m_input_int_widgets[i].SetInputTypeToIntegerNumber(it.GetElementAddress()->m_default_value);
            m_input_int_widgets[i].SetDescriptionText(it.GetElementAddress()->m_text.Get_C_String());

            m_input_int_widgets[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
            m_input_int_widgets[i].SetFontSize(m_function_widget->m_font_size);
            m_input_int_widgets[i].SetVisibility(1);
            
            this->AddWidget(&m_input_int_widgets[i]);
        }
    }

    {
        m_number_of_double_inputs = config_step->m_doubles.GetNumberOfElements();

        mxListIterator< mxFunctionConfigStepDoubleInput > it;
        int i=0;
        for(it.SetToBegin(config_step->m_doubles), i=0; it.IsValid() && i<vmxAppFunctionParametersWidget_MAX_NUMBER_OF_INPUTS; it.MoveToNext(), i++)
        {
            //std::cout<<" f ";
            m_input_double_widgets[i].SetInputTypeToDoubleNumber(it.GetElementAddress()->m_default_value);
            m_input_double_widgets[i].SetDescriptionText(it.GetElementAddress()->m_text.Get_C_String());

            m_input_double_widgets[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
            m_input_double_widgets[i].SetFontSize(m_function_widget->m_font_size);
            m_input_double_widgets[i].SetVisibility(1);
            
            this->AddWidget(&m_input_double_widgets[i]);
        }
    }

    return 1;
}


int vmxAppFunctionParametersWidget::LoadDataContainerStep(mxFunctionConfigStep *config_step, mxDataContainerStep *container_step)
{
    for(int i=0; i< (int)m_number_of_int_inputs; i++)
    {
        int n;
        m_input_int_widgets[i].m_input.ToNumber(n);
        container_step->m_integers.AddToEnd(n);
    }

    for(int i=0; i< (int)m_number_of_double_inputs; i++)
    {
        double n;
        m_input_double_widgets[i].m_input.ToNumber(n);
        container_step->m_doubles.AddToEnd(n);
    }

    return 1;
}




//-------------------------------------------------------------------------------------------------------




void vmxAppFunctionStringsWidget::Setup(vmxAppFunctionWidget *function_widget)
{
    m_function_widget = function_widget;
    
    for(int i=0; i<vmxAppFunctionStringsWidget_MAX_NUMBER_OF_INPUTS; i++)
    {
        m_input_string_widgets[i].SetMainWidget(function_widget->GetMainWidget());
        m_input_string_widgets[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
        m_input_string_widgets[i].SetFontSize(m_function_widget->m_font_size);
        m_input_string_widgets[i].SetPlacementToLowerLeft();
        m_input_string_widgets[i].SetInteractor(m_function_widget->GetAppMainWidget()->GetInteractor());
        m_input_string_widgets[i].SetInputTypeToText("");
        m_input_string_widgets[i].SetVisibility(0);
        
        //this->AddWidget(&m_input_string_widgets[i]);
    }
}


int vmxAppFunctionStringsWidget::LoadConfigurationStep(mxFunctionConfigStep *config_step)
{
    //std::cout<<std::endl<<"vmxAppFunctionStringsWidget::LoadConfigurationStep()";
    
    if(config_step->m_strings.IsEmpty()) return 0;
    
    this->SetVisibility(0);
    
    for(int i=0; i<vmxAppFunctionStringsWidget_MAX_NUMBER_OF_INPUTS ; i++)
    {
        this->RemoveWidget(&m_input_string_widgets[i]);
        //m_input_string_widgets[i].SetVisibility(0);
        //m_input_string_widgets[i].m_items.Reset();
    }
    
    {
        m_number_of_string_inputs = config_step->m_strings.GetNumberOfElements();
        
        mxListIterator< mxFunctionConfigStepStringInput > it;
        int i=0;
        for(it.SetToBegin(config_step->m_strings), i=0; it.IsValid() && i<vmxAppFunctionStringsWidget_MAX_NUMBER_OF_INPUTS; it.MoveToNext(), i++)
        {
            m_input_string_widgets[i].SetInputTypeToText(it.GetElementAddress()->m_default_string.Get_C_String());
            m_input_string_widgets[i].SetDescriptionText(it.GetElementAddress()->m_text.Get_C_String());
            
            m_input_string_widgets[i].SetColor(m_function_widget->m_color[0],m_function_widget->m_color[1],m_function_widget->m_color[2]);
            m_input_string_widgets[i].SetFontSize(m_function_widget->m_font_size);
            //m_input_string_widgets[i].SetVisibility(1);
            
            this->AddWidget(&m_input_string_widgets[i]);
        }
    }
    return 1;
}



int vmxAppFunctionStringsWidget::LoadDataContainerStep(mxFunctionConfigStep *config_step, mxDataContainerStep *container_step)
{
    for(int i=0; i< (int)m_number_of_string_inputs; i++)
    {
        container_step->m_strings.AddToEnd(m_input_string_widgets[i].m_input);
    }
    
    return 1;
}




//-------------------------------------------------------------------------------------------------------




vmxAppFunctionWidget::vmxAppFunctionWidget()
{
    m_font_size = 20;
    m_color[0] = 1; m_color[1] = 0; m_color[2] = 0;
}


void vmxAppFunctionWidget::Setup(vmxAppFunctionFactoryInterface *function_factory_interface, vmxAppFunctionInterface *function_interface)
{
	m_function_interface = function_interface;
	m_function_factory_interface = function_factory_interface;

    //	m_is_abort_requested = 0;
	//	is_allowed_to_close = 0;

}


vmxAppFunctionWidget::~vmxAppFunctionWidget()
{
	//// Release the function interface object
	//this->m_function_factory_interface->Release(m_function_interface);

//!!!!!!!!!! NEED TO DO RELEASE FOR EACH TAB !!!!!!!!!!!!
}


void vmxAppFunctionWidget::SetAppMainWidget(vmxAppMainWidget *app_main_widget)
{
    m_app_main_widget = app_main_widget;

    this->m_strings_widget.Setup(this);
    this->m_parameters_widget.Setup(this);
    this->m_options_widget.Setup(this);
    this->m_data_input_widget.Setup(this);
    
    m_push_buttons.SetMainWidget(m_app_main_widget);
    m_push_buttons.SetButtonTypeToPushButton();
    m_push_buttons.SetDescriptionText("");
    m_push_button_item_back = m_push_buttons.AddItem("Back");
    m_push_button_item_start = m_push_buttons.AddItem("Start");
    m_push_button_item_next = m_push_buttons.AddItem("Next");
    m_push_button_item_close = m_push_buttons.AddItem("Close");
    m_push_buttons.SetColor(m_color[0],m_color[1],m_color[2]);
    m_push_buttons.SetFontSize(m_font_size);
    m_push_buttons.SetPlacementToLowerLeft();
    m_push_buttons.SetPlacementOrderToBelow();
    m_push_buttons.SetVisibility(0);
    
    {
        //vmxGUIConnection *c =
        vmxGUIConnection::New(&m_push_buttons, ButtonClickedEvent, &m_push_buttons, this, vmxAppFunctionWidget::Slot_PushButtonPressed);
    }
    
    m_function_title_label.SetMainWidget(m_app_main_widget);
    m_function_title_label.SetColor(m_color[0],m_color[1],m_color[2]);
    m_function_title_label.SetFontSize(m_font_size);
    m_function_title_label.SetPlacementToLowerLeft();
    m_function_title_label.SetPlacementOrderToAbove();

};


int vmxAppFunctionWidget::LoadConfiguration(mxFunctionConfig *config)
{
	this->RemoveAllTabs();

    this->m_function_title_label.SetText(config->GetTitleLong().Get_C_String());
    this->m_function_title_label.SetVisibility(1);
    
    mxFunctionConfigStep *config_step = config->GetCurrentStep();

    if(m_data_input_widget.LoadConfigurationStep(config_step))
    {
        m_data_input_widget.SetVisibility(1);
    }
    if(m_options_widget.LoadConfigurationStep(config_step))
    {
        m_options_widget.SetVisibility(1);
    }
    if(m_parameters_widget.LoadConfigurationStep(config_step))
    {
        m_parameters_widget.SetVisibility(1);
    }
    if(m_strings_widget.LoadConfigurationStep(config_step))
    {
        m_strings_widget.SetVisibility(1);
    }


    
    if(config->GetIndexOfCurrentStep()+1 < config->GetNumberOfSteps())
    {
        this->m_push_button_item_next->m_text.Assign("Next");
        this->m_push_button_item_start->m_text.Assign("");
    }
    else
    {
        this->m_push_button_item_next->m_text.Assign("");
        this->m_push_button_item_start->m_text.Assign("Start");
    }
    if(config->GetIndexOfCurrentStep()==0)
    {
        this->m_push_button_item_back->m_text.Assign("");
    }
    else
    {
        this->m_push_button_item_back->m_text.Assign("Back");
    }
    
    this->m_push_buttons.SetVisibility(1);
    //    this->SetCancelButtonVisibility(0);
    //    this->SetCloseButtonVisibility(1);

    
    
    this->m_app_main_widget->RedoPlacement();

	return 1;
}


int vmxAppFunctionWidget::LoadDataContainer(mxFunctionConfig *input_config, mxDataContainer *data_container, unsigned int step)
{
    //std::cout<<std::endl<<"vmxAppFunctionWidget::LoadDataContainer() entered!!!";
    
	mxFunctionConfigStep *config_step = input_config->GetStep(step);
	mxDataContainerStep *data_container_step = data_container->AddStep(step);
	data_container_step->Reset();
    
    
    //--- OPTIONS --
    m_options_widget.LoadDataContainerStep(config_step,data_container_step);
    //---

    //--- DATA ---
    m_data_input_widget.LoadDataContainerStep(config_step,data_container_step);
    //---

    //--- PARAMETERS ---
    m_parameters_widget.LoadDataContainerStep(config_step,data_container_step);
    //---

    //--- STRINGS ---
    m_strings_widget.LoadDataContainerStep(config_step,data_container_step);
    //---

	return 1;
}


void vmxAppFunctionWidget::RemoveAllTabs()
{
    m_data_input_widget.SetVisibility(0);
    m_options_widget.SetVisibility(0);
    m_parameters_widget.SetVisibility(0);
    m_strings_widget.SetVisibility(0);
    m_push_buttons.SetVisibility(0);
    
    m_function_title_label.SetVisibility(0);
}


int vmxAppFunctionWidget::Slot_PushButtonPressed(vmxGUIConnection *connection)
{
    vmxAppFunctionWidget *function_widget = static_cast< vmxAppFunctionWidget* > (connection->GetSlotCallerObject());
    if(!function_widget) return 0;
    
    vmxGUIButtonGroupItem *item = function_widget->m_push_buttons.GetPushedItem();
    if(!item) return 0;
    
    /// Close button pressed.
    if(item == function_widget->m_push_button_item_close)
    {
        function_widget->RemoveAllTabs();
        return 1;
    }
    
    /// Start button pressed.
    if(item == function_widget->m_push_button_item_start)
    {
        if(function_widget->m_function_interface->GetConfig()->GetIndexOfCurrentStep() == function_widget->m_function_interface->GetConfig()->GetNumberOfSteps()-1)
        {
            function_widget->LoadDataContainer(function_widget->m_function_interface->GetConfig(), function_widget->m_function_interface->GetData(),
                                               function_widget->m_function_interface->GetConfig()->GetIndexOfCurrentStep());
            function_widget->m_function_interface->StartInMainThread();
            
            function_widget->GetAppMainWidget()->RedoPlacement();
        }
        return 1;
    }

    /// Back button pressed.
    if(item == function_widget->m_push_button_item_back)
    {
        if(function_widget->m_function_interface->GetConfig()->GetIndexOfCurrentStep()>0)
        {
            
        }
        return 1;
    }

    /// Next button pressed.
    if(item == function_widget->m_push_button_item_next)
    {
        if(function_widget->m_function_interface->GetConfig()->GetIndexOfCurrentStep() < function_widget->m_function_interface->GetConfig()->GetNumberOfSteps()-1)
        {
            function_widget->LoadDataContainer(function_widget->m_function_interface->GetConfig(), function_widget->m_function_interface->GetData(),
                                               function_widget->m_function_interface->GetConfig()->GetIndexOfCurrentStep());
            function_widget->m_function_interface->GetConfig()->IncrementIndexOfCurrentStep();
            if(function_widget->m_function_interface->GetConfig()->GetIndexOfCurrentStep() == 1)
            {
                function_widget->m_function_interface->Step_1();
            }
            if(function_widget->m_function_interface->GetConfig()->GetIndexOfCurrentStep() == 2)
            {
                function_widget->m_function_interface->Step_2();
            }
            if(function_widget->m_function_interface->GetConfig()->GetIndexOfCurrentStep() == 3)
            {
                function_widget->m_function_interface->Step_3();
            }
            function_widget->LoadConfiguration(function_widget->m_function_interface->GetConfig());
        }
        return 1;
    }

    
    
    return 0;

}
