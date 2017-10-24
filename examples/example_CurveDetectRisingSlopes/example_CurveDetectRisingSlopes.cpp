/*=========================================================================
 
 Program:   mipx
 Module:    example_CurveDetectRisingSlopes.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt

 Licensed under the BSD License 2.0.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.

 =========================================================================*/


/*
 
 The example shows the use of curve processing module to detect most prominent rising slopes.
 
 This example demonstrates the basic use of data list widget to construct data objects. The advantage is that the user can
 visualy manipulate the data using the data list widget.
 
 */



#include "mxCurve.h"
#include "mxCurveProcessing.h"
#include "vmxDataListWidget.h"




int main()
{
    // Create the main widget. This is the first step that should be done in each application. The widget contains in itself
    // a render window, a renderer for GUI objects, a renderer for 3D data view, and the interactor.
    vmxGUIMainWidget main_widget;

    
    //----- Data List Widget -----
    vmxDataListWidget v_tree;
    v_tree.GetListWidget()->SetMainWidget(&main_widget);
    v_tree.GetListWidget()->SetColor(1,1,0);
    v_tree.GetListWidget()->SetFontSize(18);
    v_tree.GetListWidget()->SetVisibility(1);
    v_tree.GetListWidget()->SetPlacementToUpperRight();
    //----------
    
    
//    // Register the vmxImage16UFactory with the data list widget.
//    // The widget will later use this factory to construct objects of the given type.
//    v_tree.AddFactory(new vmxImage16UFactory);
//    

    
    
    
    mxCurve signal;
    
    
    if(!signal.LoadMatlabFile("/Users/danilobabin/Downloads/rematlab/pyareas_Novi1_1250.m"))
    {
        cout<<" Error loading curve from Matlab file  ";
        return 0;
    }
    
    for(unsigned int s=0; s<signal.GetNumberOfSamples(); s++)
    {
        cout<<"("<<signal.X(s)<<","<<signal.Y(s)<<")";
    }
    
    mxCurve rising_slopes, rising_slopes_weighted;
    mxCurveProcessing cp;

    cp.RisingSlopePeaks(signal,rising_slopes);
    rising_slopes.SaveToMatlabFile("/Users/danilobabin/Downloads/rematlab/pyareas_Novi1_1250_rising_slopes.m");

    cp.RisingSlopePeaksWeightedBySlopeHight(signal,rising_slopes_weighted);
    rising_slopes.SaveToMatlabFile("/Users/danilobabin/Downloads/rematlab/pyareas_Novi1_1250_rising_slopes_weighted.m");


    
   
    // Start the interaction. This will call the Render() method of the render window and Start() method of the interactor.
    main_widget.StartInteraction();


    
    return 1;
}







