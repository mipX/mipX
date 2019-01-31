/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIFilesDialog.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIFilesDialog.h"







//-----------------------------------------------------------------------------------------------------



vmxGUIFilesDialog::vmxGUIFilesDialog(vmxGUIMainWidget *main_widget)
{    
    m_clip_board = NULL;
    
    m_slot_open_files = NULL;
    m_slot_save_file = NULL;
    m_slot_select_existing_folder = NULL;
    
    m_class_name.Assign("vmxGUIFilesDialog");
    
    m_selection_type = OPEN_FILES;
    
    
    this->SetMainWidget(main_widget);
    
    this->AddWidget(&m_label);
    this->AddWidget(&m_input_text);
    this->AddWidget(&m_list_widget);
    this->AddWidget(&m_buttons);
    
    this->m_buttons.SetButtonTypeToPushButton();
    this->m_buttons.SetDescriptionText(" ");
    this->m_button_OK = this->m_buttons.AddItem("OK");
    this->m_button_Cancel = this->m_buttons.AddItem("Cancel");


    
    this->m_label.SetPlacementToUpperLeft();
    this->m_input_text.SetPlacementToUpperLeft();
    this->m_list_widget.SetPlacementToUpperLeft();
    //this->m_buttons.SetPlacementToLowerRight();
    this->m_buttons.SetPlacementToLowerLeft();
    
    
    m_list_widget.SetAccepting_LeftButtonDoubleClick_Event(1);

    m_list_widget.SetAccepting_LeftButtonDrag_Event(0);

//    m_list_widget.SetAccepting_LeftButtonDrop_Event(1); // ADDED THIS !!!!!!!!!!!!!!!!!!!!!!!!!
    
    
    this->SetVisibility(0);


}


vmxGUIFilesDialog::~vmxGUIFilesDialog()
{
}


void vmxGUIFilesDialog::BuildDialog()
{
    this->m_list_widget.BuildListWidget();
    
    mxString path_text;
    path_text.Assign("Path: ");
    path_text.Append(m_path_directory);
    this->m_label.SetText(path_text.Get_C_String());
    
    this->m_input_text.SetDescriptionText("Files: ");
    this->m_input_text.SetInputTypeToText("");
    
//    this->m_buttons.SetButtonTypeToPushButton();
//    this->m_buttons.SetDescriptionText(" ");
//    this->m_buttons.AddItem("OK");
//    this->m_buttons.AddItem("Cancel");
}



int vmxGUIFilesDialog::GetSelectedFilePath(mxString &selected_file_name)
{
    mxArray<mxString> array_of_selected_items_names;
    this->m_list_widget.GetSelectedItemsNames(array_of_selected_items_names);
    
    if(array_of_selected_items_names.IsEmpty())
    {
        selected_file_name.Assign("");
        return 0;
    }
    
    selected_file_name.Assign(this->m_path_directory);
    selected_file_name.Append(array_of_selected_items_names[0]);
    
    return 1;
}


int vmxGUIFilesDialog::GetSelectedFilePaths(mxArray<mxString> &selected_file_paths)
{
    mxArray<mxString> array_of_selected_items_names;
    this->m_list_widget.GetSelectedItemsNames(array_of_selected_items_names);
    
    if(array_of_selected_items_names.IsEmpty())
    {
        selected_file_paths.Reset();
        return 0;
    }
    
    selected_file_paths.SetNumberOfElements(array_of_selected_items_names.GetNumberOfElements());
    
    for(unsigned int i=0; i<selected_file_paths.GetNumberOfElements(); i++)
    {
        selected_file_paths[i].Assign(this->m_path_directory);
        selected_file_paths[i].Append(array_of_selected_items_names[i]);
    }
    
    return 1;
}



//void vmxGUIFilesDialog::GetColor(double &r, double &g, double &b)
//{
//    m_text_actor->GetTextProperty()->GetColor(r,g,b);
//}





int vmxGUIFilesDialog::IsVisible()
{
    return m_input_text.IsVisible();
    //return m_text_actor->GetVisibility();
}


void vmxGUIFilesDialog::OnLeftButtonDoubleClick()
{
    if(!this->GetEventCallerObject()) return;
    
    // If the double click originated from the list widget object in this collection...
    if(this->GetEventCallerObject()==(&m_list_widget))
    {
        int pick_pos[2];
        this->GetEventPositionOfCallerObject(pick_pos[0],pick_pos[1]);
        
        int index = m_list_widget.GetPickedItemIndex(pick_pos[0], pick_pos[1]);
        if(index<0)
        {
            return;
        }

        // ...and if it was performed on an item representing a folder, open that folder
        vmxGUIListWidgetItem *item = m_list_widget.GetItem(index);
        if(item->m_text[item->m_text.GetNumberOfCharacters()-1]==mxString::PathSeparator())//if this item is a folder
        {
            mxString name;
            name.Assign(this->m_path_directory);
            if(item->m_text=="../")
            {
                mxString name2;
                name2.Assign(this->m_path_directory);
                if(!name2.PathLevelUp(name))
                {
                    //cout<<" name="<<name<<endl;
                    return;
                }
                else
                {
                    //cout<<" name="<<name<<endl;
                    this->m_path_directory.Assign(name);
                }
            }
            else
            {
                //cout<<" name="<<name<<endl;
                name.Append(item->m_text);
            }

            
            
            this->DisplayFolder(name.Get_C_String());
            
            
            this->GetMainWidget()->RedoPlacement();
            

            
            this->GetMainWidget()->GetInteractor()->Render();
            //cout<<" "<<name.Get_C_String()<<" ";
        }
        
        //cout<<" "<<item->m_text.Get_C_String()<<" ";
        
        //this->m_interactor->Render();
        
    }
}



//
//
//void vmxGUIFilesDialog::OnKeyPress()
//{
//}
//
//
//void vmxGUIFilesDialog::OnMouseMove()
//{
//}
//
//
//void vmxGUIFilesDialog::OnLeftButtonUp()
//{
//    
//}


void vmxGUIFilesDialog::OnLeftButtonDown()
{
    cout<<" vmxGUIFilesDialog::OnLeftButtonDown() ";
    
    // If the left click originated from the list widget
    if(this->GetEventCallerObject()==(&m_list_widget))
    {
        //... update the display of selected files in the list.
        mxString files;
        this->m_input_text.SetDescriptionText("Files: ");
        mxArray<vmxGUIListWidgetItem *> *items = this->m_list_widget.GetItems();
        for(unsigned int i=0; i<items->GetNumberOfElements(); i++)
        {
            if((*items)[i]->IsSelected())
            {
                // check if the item is a folder and if not, add it to the display.
                if((*items)[i]->m_text[((*items)[i]->m_text.GetNumberOfCharacters()-1)]!='/')
                {
                    files.Append((*items)[i]->m_text.Get_C_String());
                    files.Append(";");
                }
            }
        }
        this->m_input_text.SetInputTypeToText(files.Get_C_String());
        this->m_input_text.ShowInputText();
        return;
    }
    
//    // If the left click originated from the push buttons
//    if(this->GetEventCallerObject()==(&m_buttons))
//    {
//        cout<<" m_buttons ";
//        
//        int pick_pos[2];
//        this->GetEventPositionOfCallerObject(pick_pos[0],pick_pos[1]);
//        
//        vmxGUIButtonGroupItem* item = m_buttons.GetPickedItem(pick_pos[0],pick_pos[1]);
//        if(item==this->m_button_OK)
//        {
//            cout<<" m_button_OK ";
//            if(this->m_selection_type==OPEN_FILES)
//            {
//                m_slot_open_files(this,NULL);
//                return;
//            }
//            if(this->m_selection_type==SAVE_FILE)
//            {
//                m_slot_save_file(this,NULL);
//                return;
//            }
//            if(this->m_selection_type==SELECT_EXISTING_FOLDER)
//            {
//                m_slot_select_existing_folder(this,NULL);
//                return;
//            }
//        }
//        if(item==this->m_button_Cancel)
//        {
//            cout<<" m_button_Cancel ";
//            this->SetVisibility(0);
//            this->m_main_widget->RedoPlacement();
//            this->m_main_widget->GetInteractor()->Render();
//            return;
//        }
//    }
}


void vmxGUIFilesDialog::OnLeftButtonUp()
{
    cout<<" vmxGUIFilesDialog::OnLeftButtonUp() ";
    
    // If the left click originated from the push buttons
    if(this->GetEventCallerObject()==(&m_buttons))
    {
        //cout<<" m_buttons ";
        
        int pick_pos[2];
        this->GetEventPositionOfCallerObject(pick_pos[0],pick_pos[1]);
        
        vmxGUIButtonGroupItem* item = m_buttons.GetPickedItem(pick_pos[0],pick_pos[1]);
        if(item==this->m_button_OK)
        {
            cout<<" m_button_OK ";
            if(this->m_selection_type==OPEN_FILES)
            {
                if(!m_slot_open_files) { cout<<" vmxGUIFilesDialog::OnLeftButtonUp(): open files slot not set (m_slot_open_files is NULL)!"<<endl; }
                else
                {
                    m_slot_open_files(&m_connection_open_files);
                }
                return;
            }
            if(this->m_selection_type==SAVE_FILE)
            {
                if(!m_slot_save_file) { cout<<" vmxGUIFilesDialog::OnLeftButtonUp(): open files slot not set (m_slot_save_file is NULL)!"<<endl; }
                else
                {
                    //m_slot_save_file(this,NULL,NULL,NULL);
                }
                return;
            }
            if(this->m_selection_type==SELECT_EXISTING_FOLDER)
            {
                if(!m_slot_select_existing_folder) { cout<<" vmxGUIFilesDialog::OnLeftButtonUp(): open files slot not set (m_slot_select_existing_folder is NULL)!"<<endl; }
                else
                {
                    //m_slot_select_existing_folder(this,NULL,NULL,NULL);
                }
                return;
            }
        }
        if(item==this->m_button_Cancel)
        {
            cout<<" m_button_Cancel ";
            this->SetVisibility(0);
            this->m_main_widget->RedoPlacement();
            this->m_main_widget->GetInteractor()->Render();
            return;
        }
    }
}



//void vmxGUIFilesDialog::OnMouseWheelForward()
//{
//}
//
//
//void vmxGUIFilesDialog::OnMouseWheelBackward()
//{
//}
//


void vmxGUIFilesDialog::SetColor(double r, double g, double b)
{
    m_label.SetColor(r,g,b);
    m_input_text.SetColor(r,g,b);
    m_list_widget.SetColor(r,g,b);
    m_buttons.SetColor(r,g,b);
}


void vmxGUIFilesDialog::SetFontSize(double font_size)
{
    if(font_size<=0) return;
    
    m_label.SetFontSize(font_size);
    m_input_text.SetFontSize(font_size);
    m_list_widget.SetFontSize(font_size);
    m_buttons.SetFontSize(font_size);
}


void vmxGUIFilesDialog::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_label.SetInteractor(interactor);
    m_input_text.SetInteractor(interactor);
    m_list_widget.SetInteractor(interactor);
    m_buttons.SetInteractor(interactor);
}


int vmxGUIFilesDialog::DisplayFolder(const char *directory_path)
{
    //m_selection_type = OPEN_FILES;
    
    mxString path_directory;
    
    if(!directory_path)
    {
        char *home;
#if defined(_MSC_VER)
        home = getenv("USERPROFILE");// for Microsoft Windows use environment variable: USERPROFILE
#else
        home = getenv("HOME");// for Unix-type systems use environment variable: HOME
#endif
        path_directory.Assign(home);
    }
    else
    {
        path_directory.Assign(directory_path);
    }
    
    if(path_directory[(path_directory.GetNumberOfCharacters()-1)]!=mxString::PathSeparator())
    path_directory.Append(mxString::PathSeparator());
    
    //cout<<home;
    
    
    tinydir_dir dir;
    if(tinydir_open_sorted(&dir,path_directory.Get_C_String()) == -1)
    {
        cout<<"vmxGUIFilesDialog::DisplayFolder(): Error opening file";
        tinydir_close(&dir);
        return 0;
    }
    
    this->m_list_widget.Reset();
    
    m_path_directory.Assign(path_directory);
    
    
    this->m_list_widget.AddItem("../");
  
    //for(int i=dir.n_files-1; i>=0; i--)
    for(int i=0; i<=dir.n_files-1; i++)
    {
        tinydir_file file;
        if(tinydir_readfile_n(&dir, &file, i) == -1)
        {
            cout<<endl<<"vmxGUIFilesDialog::DisplayFolder(): Error getting file";
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
                    name.Append(file.name);
                    name.Append(mxString::PathSeparator());
                    this->m_list_widget.AddItem(name.Get_C_String()); // this->AddItem(name.Get_C_String());
                }
            }
            else
            {
                this->m_list_widget.AddItem(file.name); //this->AddItem(file.name);
            }
            //cout<<file.path<<endl;
        }
    }
    
    this->BuildDialog();

    this->SetVisibility(1);

    return 1;
}



int vmxGUIFilesDialog::OpenFiles(vmxGUISlotFunction open_files_slot, const char *directory_path)
{
    m_slot_open_files = open_files_slot;
    m_selection_type = OPEN_FILES;
  
    //!!!!!!!! CHANGE THIS - CURRENTLY IT DOES NOT WORK !!!!!!!!!!!!!!!!!!!
//    m_connection_open_files.Set(this,NULL,ButtonOKClickedEvent,this,NULL,m_slot_open_files);
    
    return this->DisplayFolder(directory_path);
}



int vmxGUIFilesDialog::SaveFile(const char *directory_path)
{
    m_selection_type = SAVE_FILE;
    
    mxString path_directory;
    
    if(!directory_path)
    {
        char *home;
#if defined(_MSC_VER)
        home = getenv("USERPROFILE");// for Microsoft Windows use environment variable: USERPROFILE
#else
        home = getenv("HOME");// for Unix-type systems use environment variable: HOME
#endif
        path_directory.Assign(home);
    }
    else
    {
        path_directory.Assign(directory_path);
    }
    
    if(path_directory[(path_directory.GetNumberOfCharacters()-1)]!=mxString::PathSeparator())
        path_directory.Append(mxString::PathSeparator());
    
    //cout<<home;
    
    
    tinydir_dir dir;
    if(tinydir_open_sorted(&dir,path_directory.Get_C_String()) == -1)
    {
        cout<<"vmxGUIFilesDialog::Start(): Error opening file";
        tinydir_close(&dir);
        return 0;
    }
    
    this->m_list_widget.Reset(); //this->m_items.Reset();
    
    m_path_directory.Assign(path_directory);
    //    m_text_index_offset = 0;
    
    
    this->m_list_widget.AddItem("../");
    
    //for(int i=dir.n_files-1; i>=0; i--)
    for(int i=0; i<=dir.n_files-1; i++)
    {
        tinydir_file file;
        if(tinydir_readfile_n(&dir, &file, i) == -1)
        {
            cout<<endl<<"vmxGUIFilesDialog::Start(): Error getting file";
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
                    name.Append(file.name);
                    name.Append(mxString::PathSeparator());
                    this->m_list_widget.AddItem(name.Get_C_String()); // this->AddItem(name.Get_C_String());
                }
            }
            else
            {
                this->m_list_widget.AddItem(file.name); //this->AddItem(file.name);
            }
            //cout<<file.path<<endl;
        }
    }
    
    this->BuildDialog();
    
    this->SetVisibility(1);
    
    return 1;
}



int vmxGUIFilesDialog::SelectExistingFolder(const char *directory_path)
{
    m_selection_type = SELECT_EXISTING_FOLDER;
    
    mxString path_directory;
    
    if(!directory_path)
    {
        char *home;
#if defined(_MSC_VER)
        home = getenv("USERPROFILE");// for Microsoft Windows use environment variable: USERPROFILE
#else
        home = getenv("HOME");// for Unix-type systems use environment variable: HOME
#endif
        path_directory.Assign(home);
    }
    else
    {
        path_directory.Assign(directory_path);
    }
    
    if(path_directory[(path_directory.GetNumberOfCharacters()-1)]!=mxString::PathSeparator())
        path_directory.Append(mxString::PathSeparator());
    
    //cout<<home;
    
    
    tinydir_dir dir;
    if(tinydir_open_sorted(&dir,path_directory.Get_C_String()) == -1)
    {
        cout<<"vmxGUIFilesDialog::Start(): Error opening file";
        tinydir_close(&dir);
        return 0;
    }
    
    this->m_list_widget.Reset(); //this->m_items.Reset();
    
    m_path_directory.Assign(path_directory);
    //    m_text_index_offset = 0;
    
    
    this->m_list_widget.AddItem("../");
    
    //for(int i=dir.n_files-1; i>=0; i--)
    for(int i=0; i<=dir.n_files-1; i++)
    {
        tinydir_file file;
        if(tinydir_readfile_n(&dir, &file, i) == -1)
        {
            cout<<endl<<"vmxGUIFilesDialog::Start(): Error getting file";
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
                    name.Append(file.name);
                    name.Append(mxString::PathSeparator());
                    this->m_list_widget.AddItem(name.Get_C_String()); // this->AddItem(name.Get_C_String());
                }
            }
            else
            {
                this->m_list_widget.AddItem(file.name); //this->AddItem(file.name);
            }
            //cout<<file.path<<endl;
        }
    }
    
    this->BuildDialog();
    
    this->SetVisibility(1);
    
    return 1;
}







