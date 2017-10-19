/*=========================================================================
 
 Program:   mipx
 Module:    mxSkeletonizationX.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxSkeletonizationX.h"



//int mxSkeletonizationX::Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(mxBasicImage &input, mxGeometry &g, mxBasicImage &temp_image_2x3x3, unsigned int t, unsigned int s_curent_slice, unsigned int r, unsigned int c, unsigned int s_second_slice)
//{
//    //This method will be used iteratively, so we do not perform input data validity check.
//    
//    // Pixel nomenclature: 'E' is an edge pixel, 'C' is a corner pixel, 'P' is the currently processed pixel:
//	// C1 E1 C2
//	// E2 P  E3
//	// C3 E4 C4
//    
//    mxBIPX bipx;
//    
//    // If the second slice is composed only of background pixels, we consider the slice connected to the current slice.
//    int number_of_foregroung_voxels_in_second_slice = bipx.NumberOfForegroundVoxelsInNeighborhood8_Iterative(input, g, t, s_second_slice, r, c);
//    if(number_of_foregroung_voxels_in_second_slice) return 1;
//    
//    // If the current slice has only background 8-neighborhood pixels, the second slice is not connected to the current one
//    // beacuse the second slice is NOT composed only of background pixels (see the previous 'if' command).
//    int number_of_foregroung_voxels_in_current_slice = bipx.NumberOfForegroundVoxelsInNeighborhood8_Iterative(input, g, t, s_curent_slice, r, c);
//    if(number_of_foregroung_voxels_in_current_slice) return 0;
//
//    // If the 'P' pixel of the second slice is a foreground pixel, the second slice is connected to the current slice.
//    if(input.Get(t,s_second_slice,r,c)) return 1;
//
//    // Copy values to temp neighborhood.
//    int rn,cn;
//    for(g.For_9_Neighborhood(r,c); g.Get_9_Neighborhood(rn,cn); )
//    {
//        temp_image_2x3x3.Set(1, rn-r+1, cn-c+1, input.Get(s_curent_slice,rn,cn));
//        temp_image_2x3x3.Set(0, rn-r+1, cn-c+1, input.Get(s_second_slice,rn,cn));
//    }
//  
//
//	// Check pixel C1 in the second slice
//    if(temp_image_2x3x3.Get(0,0,0))
//	{
//		if((!temp_image_2x3x3.Get(1,0,0)) && (!temp_image_2x3x3.Get(1,0,1)) && (!temp_image_2x3x3.Get(1,1,0)) && (!temp_image_2x3x3.Get(0,0,1)) && (!temp_image_2x3x3.Get(0,1,0)))
//        { return 0; }
//	}
//
//	// Check pixel C2 in the second slice
//	if(temp_image_2x3x3.Get(0,0,2))
//	{
//		if((!temp_image_2x3x3.Get(1,0,2)) && (!temp_image_2x3x3.Get(1,0,1)) && (!temp_image_2x3x3.Get(1,1,2)) && (!temp_image_2x3x3.Get(0,0,1)) && (!temp_image_2x3x3.Get(0,1,2)))
//        { return 0; }
//	}
//	
//	// Check pixel C3 in the second slice
//	if(temp_image_2x3x3.Get(0,2,0))
//	{
//		if((!temp_image_2x3x3.Get(1,2,0)) && (!temp_image_2x3x3.Get(1,2,1)) && (!temp_image_2x3x3.Get(1,1,0)) && (!temp_image_2x3x3.Get(0,2,1)) && (!temp_image_2x3x3.Get(0,1,0)))
//        { return 0; }
//	}
//
//	// Check pixel C4 in the second slice
//	if(temp_image_2x3x3.Get(0,2,2))
//	{
//		if((!temp_image_2x3x3.Get(1,2,2)) && (!temp_image_2x3x3.Get(1,2,1)) && (!temp_image_2x3x3.Get(1,1,2)) && (!temp_image_2x3x3.Get(0,2,1)) && (!temp_image_2x3x3.Get(0,1,2)))
//        { return 0; }
//	}
//
//    // If E1 and E2 of the second slice are foreground:
//	if( temp_image_2x3x3.Get(0,1,0) && temp_image_2x3x3.Get(0,0,1) && (!temp_image_2x3x3.Get(0,1,2)) && (!temp_image_2x3x3.Get(0,2,1)))
//	{
//        if(temp_image_2x3x3.Get(1,2,2) && number_of_foregroung_voxels_in_current_slice==1) { return 0; }
//	}
//
//    // If E2 and E3 of second slice are foreground:
//	if((!temp_image_2x3x3.Get(0,1,0)) && temp_image_2x3x3.Get(0,0,1) && temp_image_2x3x3.Get(0,1,2) && (!temp_image_2x3x3.Get(0,2,1)))
//	{
//        if(temp_image_2x3x3.Get(1,2,0) && number_of_foregroung_voxels_in_current_slice==1) { return 0; }
//	}
//
//    // If E3 and E4 of second slice are foreground:
//	if((!temp_image_2x3x3.Get(0,1,0)) && (!temp_image_2x3x3.Get(0,0,1)) && temp_image_2x3x3.Get(0,1,2) && temp_image_2x3x3.Get(0,2,1))
//	{
//        if(temp_image_2x3x3.Get(1,0,0) && number_of_foregroung_voxels_in_current_slice==1) { return 0; }
//	}
//
//    // If E4 and E1 of second slice are foreground:
//	if(temp_image_2x3x3.Get(0,1,0) && (!temp_image_2x3x3.Get(0,0,1)) && (!temp_image_2x3x3.Get(0,1,2)) && temp_image_2x3x3.Get(0,2,1))
//	{
//        if(temp_image_2x3x3.Get(1,0,2) && number_of_foregroung_voxels_in_current_slice==1) { return 0; }
//	}
//    
//    // If E2 and E4 of second slice are foreground:
//    if((!temp_image_2x3x3.Get(0,1,0)) && temp_image_2x3x3.Get(0,0,1) && (!temp_image_2x3x3.Get(0,1,2)) && temp_image_2x3x3.Get(0,2,1))
//	{
//		if((!temp_image_2x3x3.Get(1,1,0)) && (!temp_image_2x3x3.Get(1,1,2)) && (((!temp_image_2x3x3.Get(1,0,0)) && (!temp_image_2x3x3.Get(1,0,1)) && (!temp_image_2x3x3.Get(1,0,2))) || ((!temp_image_2x3x3.Get(1,2,0)) && (!temp_image_2x3x3.Get(1,2,1)) && (!temp_image_2x3x3.Get(1,2,2)))))
//        { return 0; }
//	}
//
//	// If E1 and E3 of second slice are foreground:
//	if(temp_image_2x3x3.Get(0,1,0) && (!temp_image_2x3x3.Get(0,0,1)) && temp_image_2x3x3.Get(0,1,2) && (!temp_image_2x3x3.Get(0,2,1)))
//	{
//		if((!temp_image_2x3x3.Get(1,0,1)) && (!temp_image_2x3x3.Get(1,2,1)) && (((!temp_image_2x3x3.Get(1,0,0)) && (!temp_image_2x3x3.Get(1,1,0)) && (!temp_image_2x3x3.Get(1,2,0))) || ((!temp_image_2x3x3.Get(1,0,2)) && (!temp_image_2x3x3.Get(1,1,2)) && (!temp_image_2x3x3.Get(1,2,2)))))
//        { return 0; }
//	}
//
//	// If E1 of second slice is foreground:
//	if(temp_image_2x3x3.Get(0,1,0) && (!temp_image_2x3x3.Get(0,0,1)) && (!temp_image_2x3x3.Get(0,1,2)) && (!temp_image_2x3x3.Get(0,2,1)))
//	{
//		if((!temp_image_2x3x3.Get(1,0,0)) && (!temp_image_2x3x3.Get(1,0,1)) && (!temp_image_2x3x3.Get(1,1,0)) && (!temp_image_2x3x3.Get(1,2,0)) && (!temp_image_2x3x3.Get(1,2,1)))
//        { return 0; }
//	}
//
//	// If E2 of second slice is foreground:
//	if((!temp_image_2x3x3.Get(0,1,0)) && temp_image_2x3x3.Get(0,0,1) && (!temp_image_2x3x3.Get(0,1,2)) && (!temp_image_2x3x3.Get(0,2,1)))
//	{
//		if((!temp_image_2x3x3.Get(1,0,0)) && (!temp_image_2x3x3.Get(1,0,1)) && (!temp_image_2x3x3.Get(1,0,2)) && (!temp_image_2x3x3.Get(1,1,0)) && (!temp_image_2x3x3.Get(1,1,2)))
//        { return 0; }
//	}
//
//    // If E3 of second slice is foreground:
//	if((!temp_image_2x3x3.Get(0,1,0)) && (!temp_image_2x3x3.Get(0,0,1)) && temp_image_2x3x3.Get(0,1,2) && (!temp_image_2x3x3.Get(0,2,1)))
//	{
//		if((!temp_image_2x3x3.Get(1,0,1)) && (!temp_image_2x3x3.Get(1,0,2)) && (!temp_image_2x3x3.Get(1,1,2)) && (!temp_image_2x3x3.Get(1,2,1)) && (!temp_image_2x3x3.Get(1,2,2)))
//        { return 0; }
//	}
//
//    // If E4 of second slice is foreground:
//	if((!temp_image_2x3x3.Get(0,1,0)) && (!temp_image_2x3x3.Get(0,0,1)) && (!temp_image_2x3x3.Get(0,1,2)) && temp_image_2x3x3.Get(0,2,1))
//	{
//		if((!temp_image_2x3x3.Get(1,1,0)) && (!temp_image_2x3x3.Get(1,1,2)) && (!temp_image_2x3x3.Get(1,2,0)) && (!temp_image_2x3x3.Get(1,2,1)) && (!temp_image_2x3x3.Get(1,2,2)))
//        { return 0; }
//	}
//
//	// For all other combinations return 1 (is connected).
//    return 1;
//}


int mxSkeletonizationX::Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(mxBasicImage &current3x3, mxBasicImage &second3x3, mxGeometry &g3x3)
{
    //This method will be used iteratively, so we do not perform input data validity check.
    
    // Pixel nomenclature: 'E' is an edge pixel, 'C' is a corner pixel, 'P' is the currently processed pixel:
    // C1 E1 C2
    // E2 P  E3
    // C3 E4 C4
    
    mxBIPX bipx;
    
    // If the second slice is composed only of background pixels, we consider the slice connected to the current slice.
    int number_of_foregroung_voxels_in_second_slice = bipx.NumberOfForegroundVoxelsInNeighborhood8_Iterative(second3x3, g3x3, 0, 0, 1, 1);
    if(number_of_foregroung_voxels_in_second_slice) return 1;
    
    // If the current slice has only background 8-neighborhood pixels, the second slice is not connected to the current one
    // beacuse the second slice is NOT composed only of background pixels (see the previous 'if' command).
    int number_of_foregroung_voxels_in_current_slice = bipx.NumberOfForegroundVoxelsInNeighborhood8_Iterative(current3x3, g3x3, 0, 0, 1, 1);
    if(number_of_foregroung_voxels_in_current_slice) return 0;
    
    // If the 'P' pixel of the second slice is a foreground pixel, the second slice is connected to the current slice.
    if(second3x3.Get(1,1)) return 1;
    
    
    // Check pixel C1 in the second slice
    if(second3x3.Get(0,0))
    {
        if((!current3x3.Get(0,0)) && (!current3x3.Get(0,1)) && (!current3x3.Get(1,0)) && (!second3x3.Get(0,1)) && (!second3x3.Get(1,0)))
        { return 0; }
    }
    
    // Check pixel C2 in the second slice
    if(second3x3.Get(0,2))
    {
        if((!current3x3.Get(0,2)) && (!current3x3.Get(0,1)) && (!current3x3.Get(1,2)) && (!second3x3.Get(0,1)) && (!second3x3.Get(1,2)))
        { return 0; }
    }
    
    // Check pixel C3 in the second slice
    if(second3x3.Get(2,0))
    {
        if((!current3x3.Get(2,0)) && (!current3x3.Get(2,1)) && (!current3x3.Get(1,0)) && (!second3x3.Get(2,1)) && (!second3x3.Get(1,0)))
        { return 0; }
    }
    
    // Check pixel C4 in the second slice
    if(second3x3.Get(2,2))
    {
        if((!current3x3.Get(2,2)) && (!current3x3.Get(2,1)) && (!current3x3.Get(1,2)) && (!second3x3.Get(2,1)) && (!second3x3.Get(1,2)))
        { return 0; }
    }
    
    // If E1 and E2 of the second slice are foreground:
    if( second3x3.Get(1,0) && second3x3.Get(0,1) && (!second3x3.Get(1,2)) && (!second3x3.Get(2,1)))
    {
        if(current3x3.Get(2,2) && number_of_foregroung_voxels_in_current_slice==1) { return 0; }
    }
    
    // If E2 and E3 of second slice are foreground:
    if((!second3x3.Get(1,0)) && second3x3.Get(0,1) && second3x3.Get(1,2) && (!second3x3.Get(2,1)))
    {
        if(current3x3.Get(2,0) && number_of_foregroung_voxels_in_current_slice==1) { return 0; }
    }
    
    // If E3 and E4 of second slice are foreground:
    if((!second3x3.Get(1,0)) && (!second3x3.Get(0,1)) && second3x3.Get(1,2) && second3x3.Get(2,1))
    {
        if(current3x3.Get(0,0) && number_of_foregroung_voxels_in_current_slice==1) { return 0; }
    }
    
    // If E4 and E1 of second slice are foreground:
    if(second3x3.Get(1,0) && (!second3x3.Get(0,1)) && (!second3x3.Get(1,2)) && second3x3.Get(2,1))
    {
        if(current3x3.Get(0,2) && number_of_foregroung_voxels_in_current_slice==1) { return 0; }
    }
    
    // If E2 and E4 of second slice are foreground:
    if((!second3x3.Get(1,0)) && second3x3.Get(0,1) && (!second3x3.Get(1,2)) && second3x3.Get(2,1))
    {
        if((!current3x3.Get(1,0)) && (!current3x3.Get(1,2)) && (((!current3x3.Get(0,0)) && (!current3x3.Get(0,1)) && (!current3x3.Get(0,2))) || ((!current3x3.Get(2,0)) && (!current3x3.Get(2,1)) && (!current3x3.Get(2,2)))))
        { return 0; }
    }
    
    // If E1 and E3 of second slice are foreground:
    if(second3x3.Get(1,0) && (!second3x3.Get(0,1)) && second3x3.Get(1,2) && (!second3x3.Get(2,1)))
    {
        if((!current3x3.Get(0,1)) && (!current3x3.Get(2,1)) && (((!current3x3.Get(0,0)) && (!current3x3.Get(1,0)) && (!current3x3.Get(2,0))) || ((!current3x3.Get(0,2)) && (!current3x3.Get(1,2)) && (!current3x3.Get(2,2)))))
        { return 0; }
    }
    
    // If E1 of second slice is foreground:
    if(second3x3.Get(1,0) && (!second3x3.Get(0,1)) && (!second3x3.Get(1,2)) && (!second3x3.Get(2,1)))
    {
        if((!current3x3.Get(0,0)) && (!current3x3.Get(0,1)) && (!current3x3.Get(1,0)) && (!current3x3.Get(2,0)) && (!current3x3.Get(2,1)))
        { return 0; }
    }
    
    // If E2 of second slice is foreground:
    if((!second3x3.Get(1,0)) && second3x3.Get(0,1) && (!second3x3.Get(1,2)) && (!second3x3.Get(2,1)))
    {
        if((!current3x3.Get(0,0)) && (!current3x3.Get(0,1)) && (!current3x3.Get(0,2)) && (!current3x3.Get(1,0)) && (!current3x3.Get(1,2)))
        { return 0; }
    }
    
    // If E3 of second slice is foreground:
    if((!second3x3.Get(1,0)) && (!second3x3.Get(0,1)) && second3x3.Get(1,2) && (!second3x3.Get(2,1)))
    {
        if((!current3x3.Get(0,1)) && (!current3x3.Get(0,2)) && (!current3x3.Get(1,2)) && (!current3x3.Get(2,1)) && (!current3x3.Get(2,2)))
        { return 0; }
    }
    
    // If E4 of second slice is foreground:
    if((!second3x3.Get(1,0)) && (!second3x3.Get(0,1)) && (!second3x3.Get(1,2)) && second3x3.Get(2,1))
    {
        if((!current3x3.Get(1,0)) && (!current3x3.Get(1,2)) && (!current3x3.Get(2,0)) && (!current3x3.Get(2,1)) && (!current3x3.Get(2,2)))
        { return 0; }
    }
    
    // For all other combinations return 1 (is connected).
    return 1;
}


//int mxSkeletonizationX::Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(mxBasicImage &input, mxGeometry &g, mxBasicImage &temp_slice_3x3, unsigned int t, unsigned int s_curent_slice, unsigned int r, unsigned int c, unsigned int s_second_slice)
//{
//	//This method will be used iteratively, so we do not perform input data validity check.
//    
//    // If the 'P' pixel of the second slice is a foreground pixel, the second slice is connected to the current slice.
//    if(input.Get(t,s_second_slice,r,c)) return 1;
//
//	// Copy values to temp slice neighborhood.
//    int rn,cn;
//	for(g.For_9_Neighborhood(r,c); g.Get_9_Neighborhood(rn,cn); )
//	{
//        if(input.Get(s_curent_slice,rn,cn) || input.Get(s_second_slice,rn,cn)) temp_slice_3x3.Set(0,rn-r+1,cn-c+1, 255);
//        else temp_slice_3x3.Set(0,rn-r+1,cn-c+1, 0);
//	}
//
//	return (this->IsRedundantInNeighborhood8_Iterative(temp_slice_3x3));
//}


int mxSkeletonizationX::Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(mxBasicImage &current3x3, mxBasicImage &second3x3, mxGeometry &g3x3)
{
    //This method will be used iteratively, so we do not perform input data validity check.
    
    // If the 'P' pixel of the second slice is a foreground pixel, the second slice is connected to the current slice.
    if(second3x3.Get(1,1)) return 1;
    
    // Copy values to temp slice neighborhood.
    mxDataObjectFactory *f = current3x3.GetFactory();
    mxBasicImage *temp_slice_3x3 = dynamic_cast<mxBasicImage*> (f->Create());
    if(!temp_slice_3x3) return 0;
    int rn,cn;
    for(g3x3.For_9_Neighborhood(1,1); g3x3.Get_9_Neighborhood(rn,cn); )
    {
        if(second3x3.Get(rn,cn) || current3x3.Get(rn,cn)) temp_slice_3x3->Set(rn,cn, 255);
        else temp_slice_3x3->Set(rn,cn, 0);
    }
    
    int is_redundant = this->IsRedundantInNeighborhood8_Iterative(*temp_slice_3x3);
    f->Release(temp_slice_3x3);

    return is_redundant;
}




int mxSkeletonizationX::IsRedundantInNeighborhood8_Iterative(mxBasicImage &temp_slice_3x3)
{
    //This method will be used iteratively, so we do not perform input data validity check.
    
    // Pixel nomenclature: 'E' is an edge pixel, 'C' is a corner pixel, 'P' is the currently processed pixel:
    // C1 E1 C2
    // E2 P  E3
    // C3 E4 C4


	// If the current pixel is background, it's not redundant (it isn't processed any way).
    if(!temp_slice_3x3.Get(1,1)) return 0;//if(!input.Get(s,r,c)) return 0;
    
    mxBIPX bipx;
    
    mxGeometry g1;
    g1.SetDimensions(1,3,3);
    
	int number_of_neighbors = bipx.NumberOfForegroundVoxelsInNeighborhood8_Iterative(temp_slice_3x3, g1, 0, 0, 1, 1);
	
	//number_of_neighbors = this->NumberOfNonZero_8_Neighbors(image,s,r,c);
	
	// Voxels that have only one foreground neighbor are not redundant by definition.
    if(number_of_neighbors<=1) return 0;

//	bdImage temp_image;
//	temp_image.SetSizeOf2DGrid(3,3);
//	temp_image.FillInWith(0);
//	bdGeometry g;
//	g.SetDimensions(1,image.GetNumberOfRows(),image.GetNumberOfColumns());
//
//	int rn,cn;
//
//	//Copy the neighborhood into the temp_image(3x3)
//	for(g.ForCoordinates(r,c,g.Neighborhood9StartIndex()); g.Get_9_Neighbors(rn,cn); )
//	{
//		temp_image(1+rn-r,1+cn-c) = image(s,rn,cn);
//	}
    
    
    // For C pixels we need to check if they have adjacent E pixels. If not and if there are more than 1
    // foreground neighbors of the current pixel, the current pixel is not redundant.

	// If C1 is foreground
    if(temp_slice_3x3.Get(0,0))
	{
		if( (!temp_slice_3x3.Get(1,0)) && (!temp_slice_3x3.Get(0,1)) && number_of_neighbors>1)
        { return 0; }
	}

    // If C3 is foreground
    if (temp_slice_3x3.Get(2,0))
	{
		if( (!temp_slice_3x3.Get(1,0)) && (!temp_slice_3x3.Get(2,1)) && number_of_neighbors>1)
        { return 0; }
	}

    // If C2 is foreground
    if (temp_slice_3x3.Get(0,2))
	{
		if( (!temp_slice_3x3.Get(1,2)) && (!temp_slice_3x3.Get(0,1)) && number_of_neighbors>1)
        { return 0; }
	}

    // If C4 is foreground
    if (temp_slice_3x3.Get(2,2))
	{
		if( (!temp_slice_3x3.Get(1,2)) && (!temp_slice_3x3.Get(2,1)) && number_of_neighbors>1)
        { return 0; }
	}
    
    // If one pair of opposite E pixels are foreground and the other pair is background, the
    // current pixel is not redundant.

	if( (!temp_slice_3x3.Get(1,0)) && (!temp_slice_3x3.Get(1,2)) && temp_slice_3x3.Get(0,1) && temp_slice_3x3.Get(2,1))
    { return 0; }

	if( (!temp_slice_3x3.Get(0,1)) && (!temp_slice_3x3.Get(2,1)) && temp_slice_3x3.Get(1,0) && temp_slice_3x3.Get(1,2))
    { return 0; }

	// For all other combinations return 1 (pixel is redundant).
	return 1;
}


int mxSkeletonizationX::IsRedundantInNeighborhood26_Iterative(mxBasicImage &previous3x3, mxBasicImage &current3x3, mxBasicImage &next3x3, mxGeometry &g3x3)
{
    // For C pixels we need to check if they have adjacent E pixels. If not and if there are more than 1
    // foreground neighbors of the current pixel, the current pixel is not redundant.
    
    // If the current pixel is background, it's not redundant (it isn't processed any way).
    if(!current3x3.Get(1,1)) return 0;//if(image(s,r,c)==0) return 0;
    
    mxBIPX bipx;
	
	// The current voxel is not redundant if it has less than 2 foreground neighbors.
    int np = bipx.NumberOfForegroundVoxelsInNeighborhood8_Iterative(previous3x3,g3x3,0,0,1,1);
    int nc = bipx.NumberOfForegroundVoxelsInNeighborhood8_Iterative(current3x3,g3x3,0,0,1,1);
    int nn = bipx.NumberOfForegroundVoxelsInNeighborhood8_Iterative(next3x3,g3x3,0,0,1,1);
    
    if(np+nc+nn < 2) return 0;
    
    if(nc==0) return 0;
    
    if( this->Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(current3x3, previous3x3, g3x3) && this->Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(current3x3, next3x3, g3x3) ) { return 1; }

    if( this->Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(current3x3, next3x3, g3x3) && this->Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(current3x3, previous3x3, g3x3) ) { return 1; }
    
	// For all other combinations the voxel is NOT redundant.
    return 0;
}




int mxSkeletonizationX::OrderedSkeletonization(mxBasicImage &input, mxBasicImage &output, unsigned int t)
{
	if(input.IsEmpty()) return 0;
    
    mxImageScalar min_gray_value = 0, max_gray_value = 0;
    input.GetVoxelMinAndMaxValues(min_gray_value, max_gray_value);
	if(max_gray_value==0) return 0;

    output.SetDimensionsAndPropertiesAs(&input);

    mxGeometry g;
	g.SetDimensions(input.GetDimension_S(),input.GetDimension_R(),input.GetDimension_C());

	
	//Go through the whole image and put all the non-zero values into the array of lists (histogram keeping all the positions of certain value,
	// index of array represents the intensity value). Copy the input image to output.
    
	mxArray< mxList< mxIndex > > histogram_array_of_lists_of_voxels;
	histogram_array_of_lists_of_voxels.SetNumberOfElements(131072); //max value is 65535 + 65536 for squared radii (min gray value is 0, but that value is not populated because it represents background).
	
	int max_label_value = 0; //This is the max value of label for ordered skel.(in other words it is the max index of the array that has a non-empty list of pixels to remove).

	for(unsigned int s=0; s<input.GetDimension_S(); s++)
	{
		for(unsigned int r=0; r<input.GetDimension_R(); r++)
		{
			for(unsigned int c=0; c<input.GetDimension_C(); c++)
			{
				if(input.Get(t,s,r,c))
				{
					if(input.Get(t,s,r,c)==max_gray_value)
					{
						// If the max value is found, we extand these values by adding the size of the largest inscribed sphere
                        // (but looking only at max_ray_values). I.e. we do distance transform on max_gray_value voxels.
						
                        // Determine the distance
                        unsigned int squared_radius = 0;
                        int is_radius_found = 0;
                        for(squared_radius=0, is_radius_found=0; squared_radius<g.GetMaxSphereSquaredRadius()-1 && !is_radius_found; squared_radius++)
                        {
                            int sn,rn,cn;
                            for(g.ForSphere(s,r,c,squared_radius); g.GetSphere(squared_radius+1, sn,rn,cn); )
                            {
                                if(input.Get(t,sn,rn,cn)!=max_gray_value)
                                {
                                    is_radius_found = 1;
                                    break;
                                }
                            }
                        }
                        
                        // Make a new combined value and add it to the array of voxel lists.
                        mxImageScalar combined_value = input.Get(t,s,r,c) + squared_radius;
                        
                        mxIndex *index = histogram_array_of_lists_of_voxels[combined_value].AddNewToEnd();
                        index->SetIndex(t,s,r,c);
                        
                        if(combined_value>max_label_value) max_label_value = combined_value;
					}
					else
					{
                        mxIndex *index = histogram_array_of_lists_of_voxels[input.Get(t,s,r,c)].AddNewToEnd();
                        index->SetIndex(t,s,r,c);
					}
				}

				// Make exact copy of input to output.
                output.Set(t,s,r,c, input.Get(t,s,r,c));
			}
		}
	}

    
    

	// Perform skeletonization
    
    mxGeometry g3x3;
    g3x3.SetDimensions(1,3,3);
    
    // Create temp slices of 3x3 neighborhood.
    mxDataObjectFactory *f = input.GetFactory();
    mxBasicImage *previous3x3 = dynamic_cast<mxBasicImage*> (f->Create());
    if(!previous3x3) return 0;
    previous3x3->SetDimensions(1,1,3,3);
    mxBasicImage *current3x3 = dynamic_cast<mxBasicImage*> (f->Create());
    if(!current3x3) return 0;
    current3x3->SetDimensions(1,1,3,3);
    mxBasicImage *next3x3 = dynamic_cast<mxBasicImage*> (f->Create());
    if(!next3x3) return 0;
    next3x3->SetDimensions(1,1,3,3);

    
    mxList< mxListNode < mxIndex > * > list_of_nodes_to_be_deleted;

    
    // Go through all label values and check redundancy of voxels.
    for(int label_value = 1; label_value<=max_label_value; label_value++)
    {
        // Put all the voxels for this label value which are redundant to the list of voxels that need to be removed.
        // 'is_changed' indicates if changes have been done to the skeleton.
        for(int is_changed = 1; is_changed; )
        {
            is_changed = 0;
            
            mxListIterator< mxIndex > it;
            for(it.SetToBegin(histogram_array_of_lists_of_voxels[label_value]); it.IsValid(); it.MoveToNext())
            {
                mxIndex *index = it.GetElementAddress();
                
                // For the current voxel copy its neighborhood to temp slices for analysis
                int rn,cn;
                for(g3x3.For_9_Neighborhood(index->R(),index->C()); g3x3.Get_9_Neighborhood(rn,cn); )
                {
                    if(index->S()-1>=0) previous3x3->Set(rn-index->R()+1, cn-index->C()+1, input.Get(index->S()-1,rn,cn));
                    else previous3x3->Set(rn-index->R()+1, cn-index->C()+1, 0);
                    
                    current3x3->Set(rn-index->R()+1, cn-index->C()+1, input.Get(index->S(),rn,cn));
                    
                    if(index->S()+1<input.GetDimension_S()) next3x3->Set(rn-index->R()+1, cn-index->C()+1, input.Get(index->S()+1,rn,cn));
                    else previous3x3->Set(rn-index->R()+1, cn-index->C()+1, 0);
                }
                
                // If the voxel is redundant put its containing node to the list of voxels to be deleted.
                if(this->IsRedundantInNeighborhood26_Iterative(*previous3x3, *current3x3, *next3x3, g3x3))
                {
                    mxListNode< mxIndex > *node = it.GetNode();
                    list_of_nodes_to_be_deleted.AddToEnd(node);
                    is_changed = 1;
                }
            }
            
            // If there were changes, we should remove the voxels in the list of voxels to be deleted from the output image
            if(is_changed)
            {
                while(!list_of_nodes_to_be_deleted.IsEmpty())
                {
                    mxListNode< mxIndex > *node = list_of_nodes_to_be_deleted.GetBeginElement();
                    
                    mxIndex *index = node->GetElementAddress();
                    
                    // We have to check the redundancy of the pixe again, since it could happen that 2 voxel wide centerlines get completely removed.
                    
                    // For the current voxel copy its neighborhood to temp slices for analysis
                    int rn,cn;
                    for(g3x3.For_9_Neighborhood(index->R(),index->C()); g3x3.Get_9_Neighborhood(rn,cn); )
                    {
                        if(index->S()-1>=0) previous3x3->Set(rn-index->R()+1, cn-index->C()+1, input.Get(index->S()-1,rn,cn));
                        else previous3x3->Set(rn-index->R()+1, cn-index->C()+1, 0);
                        
                        current3x3->Set(rn-index->R()+1, cn-index->C()+1, input.Get(index->S(),rn,cn));
                        
                        if(index->S()+1<input.GetDimension_S()) next3x3->Set(rn-index->R()+1, cn-index->C()+1, input.Get(index->S()+1,rn,cn));
                        else previous3x3->Set(rn-index->R()+1, cn-index->C()+1, 0);
                    }
                    
                    if(this->IsRedundantInNeighborhood26_Iterative(*previous3x3, *current3x3, *next3x3, g3x3))
                    {
                        output.Set(index->S(),index->R(),index->C(), 0);
                    }
                    
                    list_of_nodes_to_be_deleted.DeleteBegin();
                }
            }
        }
    }
	
	return 1;
}

