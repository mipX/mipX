/*=========================================================================
 
 Program:   mipx
 Module:    mxVectorProcessing.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxVectorProcessing.h"



int mxVectorProcessing::IsPositionOnLineSegment(mxPosition *position, mxPosition *L1, mxPosition *L2)
{
    // For the position to be on the line segment, the sum of distance2 to L1 and L2 should be equal to distance between L1 and L2.
    double d1 = position->DistanceEuclidean(*L1);
    double d2 = position->DistanceEuclidean(*L2);
    double d = L1->DistanceEuclidean(*L2);
    
    double precision = 0.0005;
    if( (d1+d2 >= d-precision) && (d1+d2 <= d+precision ) )
    {
        return 1;
    }
    return 0;
}



