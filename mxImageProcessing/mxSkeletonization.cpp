/*=========================================================================
 
 Program:   mipx
 Module:    mxSkeletonization.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "mxSkeletonization.h"




unsigned int mxSkeletonization::NumberOfNonZero_8_Neighbors(mxImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{	
    //This method will be used iteratively, so we do not perform input data validity check.

	mxGeometry g;
	g.SetDimensions(input.GetDimension_S(),input.GetDimension_R(),input.GetDimension_C());
	
    unsigned int number_of_neighbors = 0;
	int rn,cn;
    for(g.For_8_Neighborhood(r,c); g.Get_8_Neighborhood(rn,cn);)
	{
		if(input(t,s,rn,cn)) number_of_neighbors++;
	}
	return number_of_neighbors;
}


unsigned int mxSkeletonization::NumberOfNonZero_26_Neighbors(mxImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{	
    //This method will be used iteratively, so we do not perform input data validity check.

	mxGeometry g;
	g.SetDimensions(input.GetDimension_S(),input.GetDimension_R(),input.GetDimension_C());
    
	unsigned int number_of_neighbors = 0;
	int sn,rn,cn;
    for(g.For_26_Neighborhood(s,r,c); g.Get_26_Neighborhood(sn,rn,cn);)
	{
		if(input(t,sn,rn,cn)) number_of_neighbors++;
	}
	return number_of_neighbors;
}


int mxSkeletonization::Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(mxImage &image, unsigned int t, unsigned int s, unsigned int r, unsigned int c, unsigned int slice2)
{
    //This method will be used iteratively, so we do not perform input data validity check.
    
    // Pixel nomenclature: 'E' is an edge pixel, 'C' is a corner pixel, 'P' is the currently processed pixel:
    // C1 E1 C2
    // E2 P  E3
    // C3 E4 C4
    
    int number_of_neighbors_in_slice2, number_of_neighbors_in_current_slice;
    
    // If the second slice is composed only of background pixels, we consider the slice connected to the current slice.
    if((number_of_neighbors_in_slice2 = this->NumberOfNonZero_8_Neighbors(image,t,slice2,r,c))==0) return 1;
    
    // If the current slice has only background 8-neighborhood pixels, the second slice is not connected to the current one
    // beacuse the second slice is NOT composed only of background pixels (see the previous 'if' command).
    if((number_of_neighbors_in_current_slice = this->NumberOfNonZero_8_Neighbors(image,t,s,r,c))==0) return 0;
    
    // If the 'P' pixel of the second slice is a foreground pixel, the second slice is connected to the current slice.
    if(image(t,slice2,r,c)!=0) return 1;
    
    mxGeometry g;
    g.SetDimensions(image.GetDimension_S(),image.GetDimension_R(),image.GetDimension_C());
    
    mxImage temp;
    temp.SetDimensions(1,2,3,3);
    temp.FillInWith(0);
    
    
    // Copy 2 slices from input image to temp.
    int rn,cn;
    for(g.For_9_Neighborhood(r,c); g.Get_9_Neighborhood(rn,cn); )
    {
        temp(1,1+rn-r,1+cn-c) = image(t,s,rn,cn);
        temp(0,1+rn-r,1+cn-c) = image(t,slice2,rn,cn);
    }

    // Check pixel C1 in the second slice
    if(temp(0,0,0))
    {
        if((!temp(1,0,0)) && (!temp(1,0,1)) && (!temp(1,1,0)) && (!temp(0,0,1)) && (!temp(0,1,0))) return 0;
    }
    
    // Check pixel C2 in the second slice
    if(temp(0,0,2))
    {
        if((!temp(1,0,2)) && (!temp(1,0,1)) && (!temp(1,1,2)) && (!temp(0,0,1)) && (!temp(0,1,2))) return 0;
    }
    
    // Check pixel C3 in the second slice
    if(temp(0,2,0))
    {
        if((!temp(1,2,0)) && (!temp(1,2,1)) && (!temp(1,1,0)) && (!temp(0,2,1)) && (!temp(0,1,0))) return 0;
    }
    
    // Check pixel C4 in the second slice
    if(temp(0,2,2))
    {
        if((!temp(1,2,2)) && (!temp(1,2,1)) && (!temp(1,1,2)) && (!temp(0,2,1)) && (!temp(0,1,2))) return 0;
    }
    
    // If E1 and E2 of the second slice are foreground:
    if((temp(0,1,0)) && (temp(0,0,1)) && (!temp(0,1,2)) && (!temp(0,2,1)))
    {
        if(temp(1,2,2) && number_of_neighbors_in_current_slice==1) return 0;
    }
    
    // If E2 and E3 of second slice are foreground:
    if((!temp(0,1,0)) && (temp(0,0,1)) && (temp(0,1,2)) && (!temp(0,2,1)))
    {
        if(temp(1,2,0) && number_of_neighbors_in_current_slice==1) return 0;
    }
    
    // If E3 and E4 of second slice are foreground:
    if((!temp(0,1,0)) && (!temp(0,0,1)) && (temp(0,1,2)) && (temp(0,2,1)))
    {
        if(temp(1,0,0) && number_of_neighbors_in_current_slice==1) return 0;
    }
    
    // If E4 and E1 of second slice are foreground:
    if((temp(0,1,0)) && (!temp(0,0,1)) && (!temp(0,1,2)) && (temp(0,2,1)))
    {
        if(temp(1,0,2) && number_of_neighbors_in_current_slice==1) return 0;
    }
    
    // If E2 and E4 of second slice are foreground:
    if((!temp(0,1,0)) && (temp(0,0,1)) && (!temp(0,1,2)) && (temp(0,2,1)))
    {
        if((!temp(1,1,0)) && (!temp(1,1,2)) && (((!temp(1,0,0)) && (!temp(1,0,1)) && (!temp(1,0,2))) || ((!temp(1,2,0)) && (!temp(1,2,1)) && (!temp(1,2,2))))) return 0;
    }
    
    // If E1 and E3 of second slice are foreground:
    if((temp(0,1,0)) && (!temp(0,0,1)) && (temp(0,1,2)) && (!temp(0,2,1)))
    {
        if((!temp(1,0,1)) && (!temp(1,2,1)) && (((!temp(1,0,0)) && (!temp(1,1,0)) && (!temp(1,2,0))) || ((!temp(1,0,2)) && (!temp(1,1,2)) && (!temp(1,2,2))))) return 0;
    }
    
    // If E1 of second slice is foreground:
    if((temp(0,1,0)) && (!temp(0,0,1)) && (!temp(0,1,2)) && (!temp(0,2,1)))
    {
        if((!temp(1,0,0)) && (!temp(1,0,1)) && (!temp(1,1,0)) && (!temp(1,2,0)) && (!temp(1,2,1))) return 0;
    }
    
    // If E2 of second slice is foreground:
    if((!temp(0,1,0)) && (temp(0,0,1)) && (!temp(0,1,2)) && (!temp(0,2,1)))
    {
        if((!temp(1,0,0)) && (!temp(1,0,1)) && (!temp(1,0,2)) && (!temp(1,1,0)) && (!temp(1,1,2))) return 0;
    }
    
    // If E3 of second slice is foreground:
    if((!temp(0,1,0)) && (!temp(0,0,1)) && (temp(0,1,2)) && (!temp(0,2,1)))
    {
        if((!temp(1,0,1)) && (!temp(1,0,2)) && (!temp(1,1,2)) && (!temp(1,2,1)) && (!temp(1,2,2)))  return 0;
    }
    
    // If E4 of second slice is foreground:
    if((!temp(0,1,0)) && (!temp(0,0,1)) && (!temp(0,1,2)) && (temp(0,2,1)))
    {
        if((!temp(1,1,0)) && (!temp(1,1,2)) && (!temp(1,2,0)) && (!temp(1,2,1)) && (!temp(1,2,2)))  return 0;
    }
    
    // For all other combinations return 1 (is connected).
    return 1;
}



int mxSkeletonization::Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(mxImage &image, unsigned int t, unsigned int s, unsigned int r, unsigned int c, unsigned int slice2)
{
    //This method will be used iteratively, so we do not perform input data validity check.
    
    // If the 'P' pixel of the second slice is a foreground pixel, the second slice is connected to the current slice.
    if(image(t,slice2,r,c)!=0) return 1;

	mxGeometry g;
	g.SetDimensions(image.GetDimension_S(),image.GetDimension_R(),image.GetDimension_C());
	
	mxImage temp3x3;
	temp3x3.SetDimensions(1,1,3,3);
	temp3x3.FillInWith(0);

	int rn,cn;

	// Combine values of the two slices and enter them to temp slice 3x3 neighborhood.
    for(g.For_9_Neighborhood(r,c); g.Get_9_Neighborhood(rn,cn); )
	{
		if(image(t,s,rn,cn)!=0 || image(t,slice2,rn,cn)!=0) temp3x3(0,rn-r+1,cn-c+1) = 1;
	}

	return(this->IsRedundantInNeighborhood8_Iterative(temp3x3,0,0,1,1));
}


int mxSkeletonization::IsRedundantInNeighborhood8_Iterative(mxImage &image, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    //This method will be used iteratively, so we do not perform input data validity check.
    
    // Pixel nomenclature: 'E' is an edge pixel, 'C' is a corner pixel, 'P' is the currently processed pixel:
    // C1 E1 C2
    // E2 P  E3
    // C3 E4 C4
    
    // If the voxel is at the image edge, make it redundant.
    if(r==0 || c==0 || r==image.GetDimension_R() || c==image.GetDimension_C()) return 1;
    
   // If the current pixel is background, it's not redundant (it isn't processed any way).
    if(!image(t,s,r,c)) return 0;
    
    int number_of_neighbors;
    
    number_of_neighbors = this->NumberOfNonZero_8_Neighbors(image,t,s,r,c);
    
    // Voxels that have only one foreground neighbor are not redundant by definition.
    if(number_of_neighbors<=1) return 0;
    
    
    // For C pixels we need to check if they have adjacent E pixels. If not and if there are more than 1
    // foreground neighbors of the current pixel, the current pixel is not redundant.
    
    // If C1 is foreground
	if(image(t,s,r-1,c-1)!=0)
	{
		if( image(t,s,r,c-1)==0 && image(t,s,r-1,1)==0 && number_of_neighbors>1) return 0;
	}

	// If C3 is foreground
	if (image(t,s,r+1,c-1)!=0)
	{
		if( image(t,s,r,c-1)==0 && image(t,s,r+1,1)==0 && number_of_neighbors>1) return 0;
	}

	// If C2 is foreground
	if (image(t,s,r-1,2)!=0)
	{
		if( image(t,s,r,2)==0 && image(t,s,r-1,1)==0 && number_of_neighbors>1) return 0;
	}

	// If C4 is foreground
	if (image(t,s,r+1,2)!=0)
	{
		if( image(t,s,r,2)==0 && image(t,s,r+1,1)==0 && number_of_neighbors>1) return 0;
	}

    // If one pair of opposite E pixels are foreground and the other pair is background, the
    // current pixel is not redundant.

    if(image(t,s,r,c-1)==0 && image(t,s,r,2)==0 && image(t,s,r-1,1)!=0 && image(t,s,r+1,1)!=0)
		return 0;

	if(image(t,s,r-1,1)==0 && image(t,s,r+1,1)==0 && image(t,s,r,c-1)!=0 && image(t,s,r,2)!=0)
		return 0;
    
    // For all other combinations return 1 (pixel is redundant).
    return 1;
}



int mxSkeletonization::IsRedundantInNeighborhood26_Iterative(mxImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    //This method will be used iteratively, so we do not perform input data validity check.


	//If the image contains only one slice
	if(input.GetDimension_S()==1) return (this->IsRedundantInNeighborhood8_Iterative(input,t,0,r,c));

	
    // If the current pixel is background, it's not redundant (it isn't processed any way).
    if(!input(t,s,r,c)) return 0;
    
    // The current voxel is not redundant if it has less than 2 foreground neighbors.
	if(this->NumberOfNonZero_26_Neighbors(input,t,s,r,c)<2) return 0;

	// If the current slice contains only the processed pixel P, P is not redundant.
    if(this->NumberOfNonZero_8_Neighbors(input,t,s,r,c)==0) return 0;


	// For the first slice of the image
    if(s==0)
	{
        return this->Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(input,t,s,r,c,s+1);
	}

	// For the last slice of the image
	if(s==input.GetDimension_S()-1)
	{
        return this->Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(input,t,s,r,c,s-1);
	}

    // In other non-edge cases
	if( (this->Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(input,t,s,r,c,s-1)) && (this->Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(input,t,s,r,c,s+1)) ) return 1;
	
	if( (this->Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(input,t,s,r,c,s+1)) && (this->Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(input,t,s,r,c,s-1)) ) return 1;
	
    // For all other combinations return 0 (voxel is not redundant).
	return 0;
}



int mxSkeletonization::OrderedSkeletonization(mxImage &input, mxImage &output, unsigned int t)
{
    if(input.IsEmpty()) return 0;
    
    //Find the max value in the image
    mxImageScalar max_gray_value, min_gray_value;
    input.GetVoxelMinAndMaxValues(min_gray_value, max_gray_value);
    
    if(max_gray_value==0) return 0;
    
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(),input.GetDimension_R(),input.GetDimension_C());
    
    output.SetDimensionsAndPropertiesAs(&input);

    
    // Pass through the image in raster-scan order and insert all the non-zero gray value voxels to the array of lists of voxel indexes.
    mxArray< mxList< mxIndex > > histogram_array_of_lists_of_voxels;
    
    // We extend the histogram by calculations of squared distance for which we allow 65536 (radius value 256),while we allow for 65535 gray values, hence 65535 + 65536 + 1 is the size.
    histogram_array_of_lists_of_voxels.SetNumberOfElements(131072);
    
    // Largest found label value in the segmented image.
    int max_label_value = 0;
    
    
    for(unsigned int s=0; s<input.GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<input.GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<input.GetDimension_C(); c++)
            {
                if(input(t,s,r,c))
                {
                    if(input(t,s,r,c)==max_gray_value)
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
                                if(input(t,sn,rn,cn)!=max_gray_value)
                                {
                                    is_radius_found = 1;
                                    break;
                                }
                            }
                        }

                        // Make a new combined value and add it to the array of voxel lists.
                        mxImageScalar combined_value = input(t,s,r,c) + squared_radius;

                        mxIndex *index = histogram_array_of_lists_of_voxels[combined_value].AddNewToEnd();
                        index->SetIndex(t,s,r,c);

                        if(combined_value>max_label_value) max_label_value = combined_value;
                    }
                    else
                    {
                        mxIndex *index = histogram_array_of_lists_of_voxels[input(t,s,r,c)].AddNewToEnd();
                        index->SetIndex(t,s,r,c);
                    }
                }

                // Make exact copy of input to output.
                output(t,s,r,c) = input(t,s,r,c);
            }
        }
    }

    
    std::cout<<"Skeletonization: ";
    
    //----- Do the skeletonization -----
    
    // Perform ordered skeletonization by iterative removal of voxels (by checking their redundancy).

    

    // Go through all label values and check redundancy of voxels.
    for(int label_value = 1; label_value<=max_label_value; label_value++)
    {
        mxList< mxListNode < mxIndex > * > list_of_nodes_to_be_deleted;
        
        //std::cout<<" label="<<label_value<<" ";

        // Put all the voxels for this label value which are redundant to the list of voxels that need to be removed.
        // 'is_changed' indicates if changes have been done to the skeleton.

        mxListIterator< mxIndex > it;
        for(it.SetToBegin(histogram_array_of_lists_of_voxels[label_value]); it.IsValid(); it.MoveToNext())
        {
            //std::cout<<" F ";
            mxIndex *index = it.GetElementAddress();

            // If the voxel is redundant put its containing node to the list of voxels to be deleted.
            if(this->IsRedundantInNeighborhood26_Iterative(output, index->T(), index->S(), index->R(), index->C()))
            {
                //std::cout<<" R1 ";
                mxListNode< mxIndex > *node = it.GetNode();
                list_of_nodes_to_be_deleted.AddToEnd(node);
            }
        }

        // If there were changes, we should remove the voxels in the list of voxels to be deleted from the output image
        
    
        while(!list_of_nodes_to_be_deleted.IsEmpty())
        {
            mxListNode< mxIndex > *node = list_of_nodes_to_be_deleted.GetBeginElement();

            mxIndex *index = node->GetElementAddress();

            // We have to check the redundancy of the pixe again, since it could happen that 2 voxel wide centerlines get completely removed.

            if(this->IsRedundantInNeighborhood26_Iterative(output, index->T(), index->S(), index->R(), index->C()))
            {
                //std::cout<<" R2 ";
                output(t,index->S(),index->R(),index->C()) = 0;
            }
            
            list_of_nodes_to_be_deleted.DeleteBegin();
        }
    }
    
    return 1;
}