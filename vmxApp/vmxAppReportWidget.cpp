/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppReportWidget.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppReportWidget.h"








vmxAppReportGenerator::vmxAppReportGenerator()
{    
}


vmxAppReportGenerator::~vmxAppReportGenerator()
{
}


void vmxAppReportGenerator::SetAppMainWidget(vmxAppMainWidget *app_main_widget)
{
    m_app_main_widget = app_main_widget;
}



//void vmxAppReportGenerator::Create_XTK_ReportForGraph(vmxGraph *graph, const char *file_name)
//{
//    mxString report_html_file_path;
//    report_html_file_path.Assign(file_name);
//
//    mxString bds, str_dir, str_name, str_ext;
//    bds.Append(file_name);
//    bds.ExtractFilePathParts(str_dir,str_name,str_ext);
//
//    if(str_ext!="htm" && str_ext!="HTM" && str_ext!="html" && str_ext!="HTML") report_html_file_path.Append(".html");
//
//
//
//    //-- absolute folder path for report --
//    mxString report_files_dir_absolute;
//    report_files_dir_absolute.Assign(str_dir);
//    report_files_dir_absolute.Append(str_name);
//    report_files_dir_absolute.Append("_files");
//    report_files_dir_absolute.Append(mxString::PathSeparator());
//    //{
//        std::cout<<std::endl<<"report_files_dir_absolute='"<<report_files_dir_absolute<<"'";
//        Mkdir(report_files_dir_absolute.Get_C_String());
//    //std::string stringpath = "/Users/danilobabin/B/";
//    //int status = mkdir(stringpath.c_str(),0777);
//    //if (!std::experimental::filesystem::exists(report_files_dir_absolute.Get_C_String.Get_C_String()))
//    //{
//    //    std::experimental::filesystem::create_directories(report_files_dir_absolute.Get_C_String.Get_C_String());
//    //}
//    //}
//
//    //--
//
//
//    //-- relative folder path for report --
//    mxString report_files_dir_relative;
//    report_files_dir_relative.Append("./");
//    report_files_dir_relative.Append(str_name);
//    report_files_dir_relative.Append("_files");
//    report_files_dir_relative.Append(mxString::PathSeparator());
//    //--
//
//
//    //-- absolute folder path for webgl --
//    mxString webgl_files_dir_absolute;
//    webgl_files_dir_absolute.Assign(report_files_dir_absolute);
//    webgl_files_dir_absolute.Append(str_name);
//    webgl_files_dir_absolute.Append("_webgl_files");
//    webgl_files_dir_absolute.Append(mxString::PathSeparator());
//    //{
//        std::cout<<std::endl<<"webgl_files_dir_absolute='"<<webgl_files_dir_absolute<<"'";
//        Mkdir(webgl_files_dir_absolute.Get_C_String());
//    //if (!std::experimental::filesystem::exists(webgl_files_dir_absolute.Get_C_String()))
//    //{
//    //    std::experimental::filesystem::create_directories(webgl_files_dir_absolute.Get_C_String());
//    //}
//    //}
//
//    //--
//
//
//    //-- relative folder path for webgl --
//    mxString webgl_files_dir_relative;
//    webgl_files_dir_relative.Assign("./");
//    webgl_files_dir_relative.Append(str_name);
//    webgl_files_dir_relative.Append("_webgl_files");
//    webgl_files_dir_relative.Append(mxString::PathSeparator());
//    //--
//
//
//    /// File to which the report is being written.
//    std::ofstream file;
//
//    /// Files to which the WebGL code is being written
//    std::ofstream file_webgl_html;// html
//    std::ofstream file_webgl_js;// javascript
//
//
//    //-- Open requested files --
//    mxString webgl_html_file_path;
//    webgl_html_file_path.Assign(report_files_dir_absolute);
//    webgl_html_file_path.Append(str_name);
//    webgl_html_file_path.Append("_webgl.html");
//
//    mxString webgl_js_file_path;
//    webgl_js_file_path.Assign(webgl_files_dir_absolute);
//    webgl_js_file_path.Append(str_name);
//    webgl_js_file_path.Append("_webgl.js");
//
//    file.open(report_html_file_path.Get_C_String(), std::ios::binary);
//    file_webgl_html.open(webgl_html_file_path.Get_C_String(), std::ios::binary);
//    file_webgl_js.open(webgl_js_file_path.Get_C_String(), std::ios::binary);
//    //--
//
//
////    //-- Copy app icon --
////    mxString new_icon_path;
////    new_icon_path.Assign(report_files_dir_absolute);
////    new_icon_path.Append("app_icon");
////    utils::CopyFile(const char *input, const char *output)
//////    QFile::copy(m_icon_path.toLatin1().data(), new_icon_path.toLatin1().data());
//////    QString new_icon_path_relative = m_report_files_dir_relative;
//////    new_icon_path_relative.append("app_icon");
////    //--
////
//
//
//
//    //----- Copy XTK file -----
//    mxString old_xtk_file_path;
//    old_xtk_file_path.Assign(MIPX_FILES_XTK_PATH);
//    old_xtk_file_path.Append("xtk.js");
//    mxString new_xtk_file_path;
//    new_xtk_file_path.Assign(report_files_dir_absolute);
//    new_xtk_file_path.Append("xtk.js");
//    std::cout<<std::endl<<"old_xtk_file_path='"<<old_xtk_file_path<<"'";
//    std::cout<<std::endl<<"new_xtk_file_path='"<<new_xtk_file_path<<"'";
//    CopyFile(old_xtk_file_path.Get_C_String(), new_xtk_file_path.Get_C_String());
//    //-----
//
//    //----- Copy XTK GUI file -----
//    mxString old_xtk_gui_file_path;
//    old_xtk_gui_file_path.Assign(MIPX_FILES_XTK_PATH);
//    old_xtk_gui_file_path.Append("xtk_xdat.gui.js");
//    mxString new_xtk_gui_file_path;
//    new_xtk_gui_file_path.Assign(report_files_dir_absolute);
//    new_xtk_gui_file_path.Append("xtk_xdat.gui.js");
//    std::cout<<std::endl<<"old_xtk_gui_file_path='"<<old_xtk_gui_file_path<<"'";
//    std::cout<<std::endl<<"new_xtk_gui_file_path='"<<new_xtk_gui_file_path<<"'";
//    CopyFile(old_xtk_gui_file_path.Get_C_String(), new_xtk_gui_file_path.Get_C_String());
//    //-----
//
//
//
//    //----- WRITE REPORT -----
//
//    // HTML
//    file<<"<html>"<<std::endl;
//    {
//        // HEAD
//        file<<"<head>"<<std::endl;
//        {
//            file<<"<title>Report</title>"<<std::endl;
//
//            // XTK
//            file<<"<script type=\"text/javascript\" src=\"http://get.goXTK.com/xtk_edge.js\"></script>"<<std::endl;
//            file<<"<script type=\"text/javascript\" src=\"http://get.goXTK.com/xtk_xdat.gui.js\"></script>"<<std::endl;
//            file<<"<script type=\"text/javascript\" src=\""<<webgl_js_file_path.Get_C_String()<<"\"></script>"<<std::endl;
//            file<<"<link rel=\"stylesheet\" type=\"text/css\" href=\"demo.css\">"<<std::endl;
//
//            // Google analytics
//            file<<"<script type=\"text/javascript\">"<<std::endl;
//            file<<"var _gaq = _gaq || [];"<<std::endl;
//            file<<"_gaq.push(['_setAccount', 'UA-43414750-1']);"<<std::endl;
//            file<<"_gaq.push(['_trackPageview']);"<<std::endl;
//            file<<"(function() {"<<std::endl;
//            file<<"var ga = document.createElement('script');"<<std::endl;
//            file<<"ga.type = 'text/javascript';"<<std::endl;
//            file<<"ga.async = true;"<<std::endl;
//            file<<"ga.src = ('https:' == document.location.protocol ? 'https://ssl'"<<std::endl;
//            file<<": 'http://www') +"<<std::endl;
//            file<<"'.google-analytics.com/ga.js';"<<std::endl;
//            file<<"var s = document.getElementsByTagName('script')[0];"<<std::endl;
//            file<<"s.parentNode.insertBefore(ga, s);"<<std::endl;
//            file<<"})();"<<std::endl;
//            file<<"</script>"<<std::endl;
//        }
//        file<<"</head>"<<std::endl;
//
//        // BODY
//        file<<"<body bgcolor=\"#333\">"<<std::endl;
//        {
//            // DIV
//            file<<"<div style='position:absolute;top:10px;left:10px;'>"<<std::endl;
//            {
//                file<<"<h1 class=\"onair\"> <div class='circle'></div> <font color=\"red\">Report</font></h1>"<<std::endl;
//                file<<"<details>"<<std::endl;
//                file<<"<summary><font color=\"red\">About</font></summary>"<<std::endl;
//                file<<"<div class='content'>"<<std::endl;
//                file<<"<font color=\"red\"><b>#</b>Key functions: (R) reset view (H) hide controls.</br></font>"<<std::endl;
//                file<<"<font color=\"red\">Generated by MIPX app, designed by Danilo Babin, imec-IPI-UGent.</br></font>"<<std::endl;
//                file<<"</details>"<<std::endl;
//            }
//            file<<"</div>"<<std::endl;
//        }
//        file<<"</body>"<<std::endl;
//    }
//    file<<"</html>"<<std::endl;
//}



void vmxAppReportGenerator::Create_XTK_ReportForGraph(vmxGraph *graph, const char *file_name)
{
    mxString report_html_file_path;
    report_html_file_path.Assign(file_name);
    
    mxString bds, str_dir, str_name, str_ext;
    bds.Append(file_name);
    bds.ExtractFilePathParts(str_dir,str_name,str_ext);
    
    if(str_ext!="htm" && str_ext!="HTM" && str_ext!="html" && str_ext!="HTML") report_html_file_path.Append(".html");
    
    
    
    //-- absolute folder path for report --
    mxString report_files_dir_absolute;
    report_files_dir_absolute.Assign(str_dir);
    report_files_dir_absolute.Append(str_name);
    //report_files_dir_absolute.Append("_files");
    report_files_dir_absolute.Append(mxString::PathSeparator());
    //{
    std::cout<<std::endl<<"report_files_dir_absolute='"<<report_files_dir_absolute<<"'";
    Mkdir(report_files_dir_absolute.Get_C_String());
    //std::string stringpath = "/Users/danilobabin/B/";
    //int status = mkdir(stringpath.c_str(),0777);
    //if (!std::experimental::filesystem::exists(report_files_dir_absolute.Get_C_String.Get_C_String()))
    //{
    //    std::experimental::filesystem::create_directories(report_files_dir_absolute.Get_C_String.Get_C_String());
    //}
    //}
    
    //--
    
    
    //-- relative folder path for report --
    mxString report_files_dir_relative;
    report_files_dir_relative.Append("./");
    report_files_dir_relative.Append(str_name);
    //report_files_dir_relative.Append("_files");
    report_files_dir_relative.Append(mxString::PathSeparator());
    //--
    
    
    //-- absolute folder path for webgl --
    mxString webgl_files_dir_absolute;
    webgl_files_dir_absolute.Assign(report_files_dir_absolute);
    webgl_files_dir_absolute.Append(str_name);
    webgl_files_dir_absolute.Append("_webgl_files");
    webgl_files_dir_absolute.Append(mxString::PathSeparator());
    //{
    std::cout<<std::endl<<"webgl_files_dir_absolute='"<<webgl_files_dir_absolute<<"'";
    Mkdir(webgl_files_dir_absolute.Get_C_String());
    //if (!std::experimental::filesystem::exists(webgl_files_dir_absolute.Get_C_String()))
    //{
    //    std::experimental::filesystem::create_directories(webgl_files_dir_absolute.Get_C_String());
    //}
    //}
    
    //--
    
    
    //-- relative folder path for webgl --
    mxString webgl_files_dir_relative;
    webgl_files_dir_relative.Assign("./");
    webgl_files_dir_relative.Append(str_name);
    webgl_files_dir_relative.Append("_webgl_files");
    webgl_files_dir_relative.Append(mxString::PathSeparator());
    //--
    
    
    /// File to which the report is being written.
    //std::ofstream file;
    
    /// Files to which the WebGL code is being written
    std::ofstream file;//_webgl_html;// html
    std::ofstream file_webgl_js;// javascript
    
    
    //-- Open requested files --
    mxString webgl_html_file_path;
    webgl_html_file_path.Assign(report_files_dir_absolute);
    webgl_html_file_path.Append(str_name);
    webgl_html_file_path.Append("_webgl.html");
    
    mxString webgl_js_file_path;
    webgl_js_file_path.Assign(webgl_files_dir_absolute);
    webgl_js_file_path.Append(str_name);
    webgl_js_file_path.Append("_webgl.js");
    
    mxString webgl_js_file_path_relative;
    webgl_js_file_path_relative.Assign(webgl_files_dir_relative);
    webgl_js_file_path_relative.Append(str_name);
    webgl_js_file_path_relative.Append("_webgl.js");
    
    //file.open(report_html_file_path.Get_C_String(), std::ios::binary);
    file.open(webgl_html_file_path.Get_C_String(), std::ios::binary);//file_webgl_html.open(webgl_html_file_path.Get_C_String(), std::ios::binary);
    file_webgl_js.open(webgl_js_file_path.Get_C_String(), std::ios::binary);
    //--
    
    
    //    //-- Copy app icon --
    //    mxString new_icon_path;
    //    new_icon_path.Assign(report_files_dir_absolute);
    //    new_icon_path.Append("app_icon");
    //    utils::CopyFile(const char *input, const char *output)
    ////    QFile::copy(m_icon_path.toLatin1().data(), new_icon_path.toLatin1().data());
    ////    QString new_icon_path_relative = m_report_files_dir_relative;
    ////    new_icon_path_relative.append("app_icon");
    //    //--
    //
    
    
    
    //----- Copy XTK file -----
    mxString old_xtk_file_path;
    old_xtk_file_path.Assign(MIPX_FILES_XTK_PATH);
    old_xtk_file_path.Append("xtk.js");
    mxString new_xtk_file_path;
    new_xtk_file_path.Assign(webgl_files_dir_absolute);//report_files_dir_absolute);
    new_xtk_file_path.Append("xtk.js");
    std::cout<<std::endl<<"old_xtk_file_path='"<<old_xtk_file_path<<"'";
    std::cout<<std::endl<<"new_xtk_file_path='"<<new_xtk_file_path<<"'";
    CopyFile(old_xtk_file_path.Get_C_String(), new_xtk_file_path.Get_C_String());
    //-----
    
    //----- Copy XTK EDGE file -----
    mxString old_xtk_edge_file_path;
    old_xtk_edge_file_path.Assign(MIPX_FILES_XTK_PATH);
    old_xtk_edge_file_path.Append("xtk_edge.js");
    mxString new_xtk_edge_file_path;
    new_xtk_edge_file_path.Assign(webgl_files_dir_absolute);//report_files_dir_absolute);
    new_xtk_edge_file_path.Append("xtk_edge.js");
    std::cout<<std::endl<<"old_xtk_edge_file_path='"<<old_xtk_edge_file_path<<"'";
    std::cout<<std::endl<<"new_xtk_edge_file_path='"<<new_xtk_edge_file_path<<"'";
    CopyFile(old_xtk_edge_file_path.Get_C_String(), new_xtk_edge_file_path.Get_C_String());
    //-----

    
    //----- Copy XTK GUI file -----
    mxString old_xtk_gui_file_path;
    old_xtk_gui_file_path.Assign(MIPX_FILES_XTK_PATH);
    old_xtk_gui_file_path.Append("xtk_xdat.gui.js");
    mxString new_xtk_gui_file_path;
    new_xtk_gui_file_path.Assign(webgl_files_dir_absolute);//report_files_dir_absolute);
    new_xtk_gui_file_path.Append("xtk_xdat.gui.js");
    std::cout<<std::endl<<"old_xtk_gui_file_path='"<<old_xtk_gui_file_path<<"'";
    std::cout<<std::endl<<"new_xtk_gui_file_path='"<<new_xtk_gui_file_path<<"'";
    CopyFile(old_xtk_gui_file_path.Get_C_String(), new_xtk_gui_file_path.Get_C_String());
    //-----
    
    
    // Save graph mesh
    mxString graph_mesh_path_absolute;
    graph_mesh_path_absolute.Assign(webgl_files_dir_absolute);
    graph_mesh_path_absolute.Append(graph->GetObjectName());
    graph_mesh_path_absolute.Append(".vtk");
    graph->ExportMeshToVTK(graph_mesh_path_absolute.Get_C_String());
    
    
    //----- WRITE REPORT -----
    
    // HTML
    file<<"<html>"<<std::endl<<std::endl;
    {
        // HEAD
        file<<"<head>"<<std::endl<<std::endl;
        {
            file<<"<title>Report</title>"<<std::endl<<std::endl;
            
            // XTK
            
            file<<"<script type=\"text/javascript\" src=\""<<webgl_files_dir_relative.Get_C_String()<<"xtk_edge.js\"></script>"<<std::endl; //file<<"<script type=\"text/javascript\" src=\"http://get.goXTK.com/xtk_edge.js\"></script>"<<std::endl;
            file<<"<script type=\"text/javascript\" src=\""<<webgl_files_dir_relative.Get_C_String()<<"xtk_xdat.gui.js\"></script>"<<std::endl; //file<<"<script type=\"text/javascript\" src=\"http://get.goXTK.com/xtk_xdat.gui.js\"></script>"<<std::endl;
            file<<"<script type=\"text/javascript\" src=\""<<webgl_js_file_path_relative.Get_C_String()<<"\"></script>"<<std::endl;
            //file<<"<link rel=\"stylesheet\" type=\"text/css\" href=\"demo.css\">"<<std::endl<<std::endl;
            
//            // Google analytics
//            file<<"<script type=\"text/javascript\">"<<std::endl;
//            file<<"var _gaq = _gaq || [];"<<std::endl;
//            file<<"_gaq.push(['_setAccount', 'UA-43414750-1']);"<<std::endl;
//            file<<"_gaq.push(['_trackPageview']);"<<std::endl;
//            file<<"(function() {"<<std::endl;
//            file<<"var ga = document.createElement('script');"<<std::endl;
//            file<<"ga.type = 'text/javascript';"<<std::endl;
//            file<<"ga.async = true;"<<std::endl;
//            file<<"ga.src = ('https:' == document.location.protocol ? 'https://ssl'"<<std::endl;
//            file<<": 'http://www') +"<<std::endl;
//            file<<"'.google-analytics.com/ga.js';"<<std::endl;
//            file<<"var s = document.getElementsByTagName('script')[0];"<<std::endl;
//            file<<"s.parentNode.insertBefore(ga, s);"<<std::endl;
//            file<<"})();"<<std::endl;
//            file<<"</script>"<<std::endl<<std::endl;
        }
        file<<"</head>"<<std::endl<<std::endl;
        
        // BODY
        file<<"<body bgcolor=\"#333\">"<<std::endl<<std::endl;
        {
            // DIV
            file<<"<div style='position:absolute;top:10px;left:10px;'>"<<std::endl;
            {
                file<<"<h1 class=\"onair\"> <div class='circle'></div> <font color=\"red\">Report</font></h1>"<<std::endl;
                file<<"<details>"<<std::endl;
                file<<"<summary><font color=\"red\">About</font></summary>"<<std::endl;
                file<<"<div class='content'>"<<std::endl;
                file<<"<font color=\"red\"><b>#</b>Key functions: (R) reset view (H) hide controls.</br></font>"<<std::endl;
                file<<"<font color=\"red\">Generated by MIPX app, designed by Danilo Babin, imec-IPI-UGent.</br></font>"<<std::endl;
                file<<"</details>"<<std::endl;
            }
            file<<"</div>"<<std::endl;
        }
        file<<"</body>"<<std::endl<<std::endl;
    }
    file<<"</html>"<<std::endl;
    
    
    // WRITE WEBGL JS
    file_webgl_js<<"window.onload = function() {"<<std::endl<<std::endl;
    
    file_webgl_js<<"   var r = new X.renderer3D();"<<std::endl;
    file_webgl_js<<"   r.bgColor = [0.2, 0.2, 0.2];"<<std::endl;
    file_webgl_js<<"   r.init();"<<std::endl<<std::endl;
    
    file_webgl_js<<"   var "<<graph->GetObjectName().Get_C_String()<<"Mesh = new X.mesh();"<<std::endl;
    file_webgl_js<<"   "<<graph->GetObjectName().Get_C_String()<<"Mesh.file = '"<<webgl_files_dir_relative.Get_C_String()<<graph->GetObjectName().Get_C_String()<<".vtk';"<<std::endl;
    file_webgl_js<<"   "<<graph->GetObjectName().Get_C_String()<<"Mesh.caption = '"<<graph->GetObjectName().Get_C_String()<<"';"<<std::endl;
    //file_webgl_js<<"   "<<graph->GetObjectName().Get_C_String()<<"Mesh.color = [0.25, 0.25, 0.7];"<<std::endl;
    file_webgl_js<<"   "<<graph->GetObjectName().Get_C_String()<<"Mesh.magicmode = true;"<<std::endl<<std::endl;
    file_webgl_js<<"   "<<graph->GetObjectName().Get_C_String()<<"Mesh.opacity = 1;"<<std::endl<<std::endl;
    
    file_webgl_js<<"   r.onShowtime = function() {"<<std::endl;
    file_webgl_js<<"      var gui = new dat.GUI();"<<std::endl;
    file_webgl_js<<"      var "<<graph->GetObjectName().Get_C_String()<<"Gui = gui.addFolder('"<<graph->GetObjectName().Get_C_String()<<"');"<<std::endl;
    file_webgl_js<<"      var "<<graph->GetObjectName().Get_C_String()<<"OpacityController = "<<graph->GetObjectName().Get_C_String()<<"Gui.add("<<graph->GetObjectName().Get_C_String()<<"Mesh, 'opacity', 0, 1).listen();"<<std::endl;
    file_webgl_js<<"      var "<<graph->GetObjectName().Get_C_String()<<"VisibleController = "<<graph->GetObjectName().Get_C_String()<<"Gui.add("<<graph->GetObjectName().Get_C_String()<<"Mesh, 'visible');"<<std::endl;
    file_webgl_js<<"      "<<graph->GetObjectName().Get_C_String()<<"Gui.open();"<<std::endl;
    file_webgl_js<<"   };"<<std::endl<<std::endl;
    
    file_webgl_js<<"   r.add("<<graph->GetObjectName().Get_C_String()<<"Mesh);"<<std::endl;
    
    file_webgl_js<<"   r.camera.position = [320, -30, 100];"<<std::endl;
    file_webgl_js<<"   r.render();"<<std::endl;
    file_webgl_js<<"};"<<std::endl;
    
    
    file.close();
    file_webgl_js.close();
    
    

}


void vmxAppReportGenerator::Create_XTK_ReportForSkeleton(vmxSkeleton *skeleton, const char *file_name)
{
    mxString report_html_file_path;
    report_html_file_path.Assign(file_name);
    
    mxString bds, str_dir, str_name, str_ext;
    bds.Append(file_name);
    bds.ExtractFilePathParts(str_dir,str_name,str_ext);
    
    if(str_ext!="htm" && str_ext!="HTM" && str_ext!="html" && str_ext!="HTML") report_html_file_path.Append(".html");
    
    
    
    //-- absolute folder path for report --
    mxString report_files_dir_absolute;
    report_files_dir_absolute.Assign(str_dir);
    report_files_dir_absolute.Append(str_name);
    //report_files_dir_absolute.Append("_files");
    report_files_dir_absolute.Append(mxString::PathSeparator());
    //{
    std::cout<<std::endl<<"report_files_dir_absolute='"<<report_files_dir_absolute<<"'";
    Mkdir(report_files_dir_absolute.Get_C_String());
    //std::string stringpath = "/Users/danilobabin/B/";
    //int status = mkdir(stringpath.c_str(),0777);
    //if (!std::experimental::filesystem::exists(report_files_dir_absolute.Get_C_String.Get_C_String()))
    //{
    //    std::experimental::filesystem::create_directories(report_files_dir_absolute.Get_C_String.Get_C_String());
    //}
    //}
    
    //--
    
    
    //-- relative folder path for report --
    mxString report_files_dir_relative;
    report_files_dir_relative.Append("./");
    report_files_dir_relative.Append(str_name);
    //report_files_dir_relative.Append("_files");
    report_files_dir_relative.Append(mxString::PathSeparator());
    //--
    
    
    //-- absolute folder path for webgl --
    mxString webgl_files_dir_absolute;
    webgl_files_dir_absolute.Assign(report_files_dir_absolute);
    webgl_files_dir_absolute.Append(str_name);
    webgl_files_dir_absolute.Append("_webgl_files");
    webgl_files_dir_absolute.Append(mxString::PathSeparator());
    //{
    std::cout<<std::endl<<"webgl_files_dir_absolute='"<<webgl_files_dir_absolute<<"'";
    Mkdir(webgl_files_dir_absolute.Get_C_String());
    //if (!std::experimental::filesystem::exists(webgl_files_dir_absolute.Get_C_String()))
    //{
    //    std::experimental::filesystem::create_directories(webgl_files_dir_absolute.Get_C_String());
    //}
    //}
    
    //--
    
    
    //-- relative folder path for webgl --
    mxString webgl_files_dir_relative;
    webgl_files_dir_relative.Assign("./");
    webgl_files_dir_relative.Append(str_name);
    webgl_files_dir_relative.Append("_webgl_files");
    webgl_files_dir_relative.Append(mxString::PathSeparator());
    //--
    
    
    /// File to which the report is being written.
    //std::ofstream file;
    
    /// Files to which the WebGL code is being written
    std::ofstream file;//_webgl_html;// html
    std::ofstream file_webgl_js;// javascript
    
    
    //-- Open requested files --
    mxString webgl_html_file_path;
    webgl_html_file_path.Assign(report_files_dir_absolute);
    webgl_html_file_path.Append(str_name);
    webgl_html_file_path.Append("_webgl.html");
    
    mxString webgl_js_file_path;
    webgl_js_file_path.Assign(webgl_files_dir_absolute);
    webgl_js_file_path.Append(str_name);
    webgl_js_file_path.Append("_webgl.js");
    
    mxString webgl_js_file_path_relative;
    webgl_js_file_path_relative.Assign(webgl_files_dir_relative);
    webgl_js_file_path_relative.Append(str_name);
    webgl_js_file_path_relative.Append("_webgl.js");
    
    //file.open(report_html_file_path.Get_C_String(), std::ios::binary);
    file.open(webgl_html_file_path.Get_C_String(), std::ios::binary);//file_webgl_html.open(webgl_html_file_path.Get_C_String(), std::ios::binary);
    file_webgl_js.open(webgl_js_file_path.Get_C_String(), std::ios::binary);
    //--
    
    
    //    //-- Copy app icon --
    //    mxString new_icon_path;
    //    new_icon_path.Assign(report_files_dir_absolute);
    //    new_icon_path.Append("app_icon");
    //    utils::CopyFile(const char *input, const char *output)
    ////    QFile::copy(m_icon_path.toLatin1().data(), new_icon_path.toLatin1().data());
    ////    QString new_icon_path_relative = m_report_files_dir_relative;
    ////    new_icon_path_relative.append("app_icon");
    //    //--
    //
    
    
    
    //----- Copy XTK file -----
    mxString old_xtk_file_path;
    old_xtk_file_path.Assign(MIPX_FILES_XTK_PATH);
    old_xtk_file_path.Append("xtk.js");
    mxString new_xtk_file_path;
    new_xtk_file_path.Assign(webgl_files_dir_absolute);//report_files_dir_absolute);
    new_xtk_file_path.Append("xtk.js");
    std::cout<<std::endl<<"old_xtk_file_path='"<<old_xtk_file_path<<"'";
    std::cout<<std::endl<<"new_xtk_file_path='"<<new_xtk_file_path<<"'";
    CopyFile(old_xtk_file_path.Get_C_String(), new_xtk_file_path.Get_C_String());
    //-----
    
    //----- Copy XTK EDGE file -----
    mxString old_xtk_edge_file_path;
    old_xtk_edge_file_path.Assign(MIPX_FILES_XTK_PATH);
    old_xtk_edge_file_path.Append("xtk_edge.js");
    mxString new_xtk_edge_file_path;
    new_xtk_edge_file_path.Assign(webgl_files_dir_absolute);//report_files_dir_absolute);
    new_xtk_edge_file_path.Append("xtk_edge.js");
    std::cout<<std::endl<<"old_xtk_edge_file_path='"<<old_xtk_edge_file_path<<"'";
    std::cout<<std::endl<<"new_xtk_edge_file_path='"<<new_xtk_edge_file_path<<"'";
    CopyFile(old_xtk_edge_file_path.Get_C_String(), new_xtk_edge_file_path.Get_C_String());
    //-----
    
    
    //----- Copy XTK GUI file -----
    mxString old_xtk_gui_file_path;
    old_xtk_gui_file_path.Assign(MIPX_FILES_XTK_PATH);
    old_xtk_gui_file_path.Append("xtk_xdat.gui.js");
    mxString new_xtk_gui_file_path;
    new_xtk_gui_file_path.Assign(webgl_files_dir_absolute);//report_files_dir_absolute);
    new_xtk_gui_file_path.Append("xtk_xdat.gui.js");
    std::cout<<std::endl<<"old_xtk_gui_file_path='"<<old_xtk_gui_file_path<<"'";
    std::cout<<std::endl<<"new_xtk_gui_file_path='"<<new_xtk_gui_file_path<<"'";
    CopyFile(old_xtk_gui_file_path.Get_C_String(), new_xtk_gui_file_path.Get_C_String());
    //-----
    
    
    // Save graph mesh
    mxString graph_mesh_path_absolute;
    graph_mesh_path_absolute.Assign(webgl_files_dir_absolute);
    graph_mesh_path_absolute.Append(skeleton->GetObjectName());
    graph_mesh_path_absolute.Append(".vtk");
    skeleton->ExportMeshToVTKFile(graph_mesh_path_absolute.Get_C_String());
    
    
    //----- WRITE REPORT -----
    
    // HTML
    file<<"<html>"<<std::endl<<std::endl;
    {
        // HEAD
        file<<"<head>"<<std::endl<<std::endl;
        {
            file<<"<title>Report</title>"<<std::endl<<std::endl;
            
            // XTK
            
            file<<"<script type=\"text/javascript\" src=\""<<webgl_files_dir_relative.Get_C_String()<<"xtk_edge.js\"></script>"<<std::endl; //file<<"<script type=\"text/javascript\" src=\"http://get.goXTK.com/xtk_edge.js\"></script>"<<std::endl;
            file<<"<script type=\"text/javascript\" src=\""<<webgl_files_dir_relative.Get_C_String()<<"xtk_xdat.gui.js\"></script>"<<std::endl; //file<<"<script type=\"text/javascript\" src=\"http://get.goXTK.com/xtk_xdat.gui.js\"></script>"<<std::endl;
            file<<"<script type=\"text/javascript\" src=\""<<webgl_js_file_path_relative.Get_C_String()<<"\"></script>"<<std::endl;
            //file<<"<link rel=\"stylesheet\" type=\"text/css\" href=\"demo.css\">"<<std::endl<<std::endl;
            
            //            // Google analytics
            //            file<<"<script type=\"text/javascript\">"<<std::endl;
            //            file<<"var _gaq = _gaq || [];"<<std::endl;
            //            file<<"_gaq.push(['_setAccount', 'UA-43414750-1']);"<<std::endl;
            //            file<<"_gaq.push(['_trackPageview']);"<<std::endl;
            //            file<<"(function() {"<<std::endl;
            //            file<<"var ga = document.createElement('script');"<<std::endl;
            //            file<<"ga.type = 'text/javascript';"<<std::endl;
            //            file<<"ga.async = true;"<<std::endl;
            //            file<<"ga.src = ('https:' == document.location.protocol ? 'https://ssl'"<<std::endl;
            //            file<<": 'http://www') +"<<std::endl;
            //            file<<"'.google-analytics.com/ga.js';"<<std::endl;
            //            file<<"var s = document.getElementsByTagName('script')[0];"<<std::endl;
            //            file<<"s.parentNode.insertBefore(ga, s);"<<std::endl;
            //            file<<"})();"<<std::endl;
            //            file<<"</script>"<<std::endl<<std::endl;
        }
        file<<"</head>"<<std::endl<<std::endl;
        
        // BODY
        file<<"<body bgcolor=\"#333\">"<<std::endl<<std::endl;
        {
            // DIV
            file<<"<div style='position:absolute;top:10px;left:10px;'>"<<std::endl;
            {
                file<<"<h1 class=\"onair\"> <div class='circle'></div> <font color=\"red\">Report</font></h1>"<<std::endl;
                file<<"<details>"<<std::endl;
                file<<"<summary><font color=\"red\">About</font></summary>"<<std::endl;
                file<<"<div class='content'>"<<std::endl;
                file<<"<font color=\"red\"><b>#</b>Key functions: (R) reset view (H) hide controls.</br></font>"<<std::endl;
                file<<"<font color=\"red\">Generated by MIPX app, designed by Danilo Babin, imec-IPI-UGent.</br></font>"<<std::endl;
                file<<"</details>"<<std::endl;
            }
            file<<"</div>"<<std::endl;
        }
        file<<"</body>"<<std::endl<<std::endl;
    }
    file<<"</html>"<<std::endl;
    
    
    // WRITE WEBGL JS
    file_webgl_js<<"window.onload = function() {"<<std::endl<<std::endl;
    
    file_webgl_js<<"   var r = new X.renderer3D();"<<std::endl;
    file_webgl_js<<"   r.bgColor = [0.2, 0.2, 0.2];"<<std::endl;
    file_webgl_js<<"   r.init();"<<std::endl<<std::endl;
    
    file_webgl_js<<"   var "<<skeleton->GetObjectName().Get_C_String()<<"Mesh = new X.mesh();"<<std::endl;
    file_webgl_js<<"   "<<skeleton->GetObjectName().Get_C_String()<<"Mesh.file = '"<<webgl_files_dir_relative.Get_C_String()<<skeleton->GetObjectName().Get_C_String()<<".vtk';"<<std::endl;
    file_webgl_js<<"   "<<skeleton->GetObjectName().Get_C_String()<<"Mesh.caption = '"<<skeleton->GetObjectName().Get_C_String()<<"';"<<std::endl;
    //file_webgl_js<<"   "<<graph->GetObjectName().Get_C_String()<<"Mesh.color = [0.25, 0.25, 0.7];"<<std::endl;
    file_webgl_js<<"   "<<skeleton->GetObjectName().Get_C_String()<<"Mesh.magicmode = true;"<<std::endl<<std::endl;
    file_webgl_js<<"   "<<skeleton->GetObjectName().Get_C_String()<<"Mesh.opacity = 1;"<<std::endl<<std::endl;
    
    file_webgl_js<<"   r.onShowtime = function() {"<<std::endl;
    file_webgl_js<<"      var gui = new dat.GUI();"<<std::endl;
    file_webgl_js<<"      var "<<skeleton->GetObjectName().Get_C_String()<<"Gui = gui.addFolder('"<<skeleton->GetObjectName().Get_C_String()<<"');"<<std::endl;
    file_webgl_js<<"      var "<<skeleton->GetObjectName().Get_C_String()<<"OpacityController = "<<skeleton->GetObjectName().Get_C_String()<<"Gui.add("<<skeleton->GetObjectName().Get_C_String()<<"Mesh, 'opacity', 0, 1).listen();"<<std::endl;
    file_webgl_js<<"      var "<<skeleton->GetObjectName().Get_C_String()<<"VisibleController = "<<skeleton->GetObjectName().Get_C_String()<<"Gui.add("<<skeleton->GetObjectName().Get_C_String()<<"Mesh, 'visible');"<<std::endl;
    file_webgl_js<<"      "<<skeleton->GetObjectName().Get_C_String()<<"Gui.open();"<<std::endl;
    file_webgl_js<<"   };"<<std::endl<<std::endl;
    
    file_webgl_js<<"   r.add("<<skeleton->GetObjectName().Get_C_String()<<"Mesh);"<<std::endl;
    
    file_webgl_js<<"   r.camera.position = [320, -30, 100];"<<std::endl;
    file_webgl_js<<"   r.render();"<<std::endl;
    file_webgl_js<<"};"<<std::endl;
    
    
    file.close();
    file_webgl_js.close();

}
