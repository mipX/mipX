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


int mxVectorProcessing::ProjectPositionToLine(mxPosition &line_position1, mxPosition &line_position2, mxPosition &position_to_project, mxPosition &output_projected_position, double &n)
{
    if(line_position1.IsEqualPositionIn_3D(line_position2)) return 0;
    
    //'n' is the coefficient stating where the position falls on the vector defined by 'vector_position1' and 'vector_position2'
    
    //The derived formula is:
    // n(x1^2 - 2x1x2 + x2^2) + x1x2- x1^2 + x1x3 - x2x3 + n(y1^2 - 2y1y2 + y2^2) + y1y2- y1^2 + y1y3 - y2y3 + n(z1^2 - 2z1z2 + z2^2) + z1z2- z1^2 + z1z3 - z2z3 = 0
    
    //'p1' will be the part NOT multiplied by 'n': x1x2- x1^2 + x1x3 - x2x3 + y1y2- y1^2 + y1y3 - y2y3 + z1z2- z1^2 + z1z3 - z2z3.
    double p1 = line_position1.C()*line_position2.C() - line_position1.C()*line_position1.C() + line_position1.C()*position_to_project.C() - line_position2.C()*position_to_project.C() +
    line_position1.R()*line_position2.R() - line_position1.R()*line_position1.R() + line_position1.R()*position_to_project.R() - line_position2.R()*position_to_project.R() +
    line_position1.S()*line_position2.S() - line_position1.S()*line_position1.S() + line_position1.S()*position_to_project.S() - line_position2.S()*position_to_project.S();
    
    //'p2' will be the part multiplied by 'n': (x1^2 - 2x1x2 + x2^2) + (y1^2 - 2y1y2 + y2^2) + (z1^2 - 2z1z2 + z2^2).
    double p2 = line_position1.C()*line_position1.C() - 2*line_position1.C()*line_position2.C() + line_position2.C()*line_position2.C() +
    line_position1.R()*line_position1.R() - 2*line_position1.R()*line_position2.R() + line_position2.R()*line_position2.R() +
    line_position1.S()*line_position1.S() - 2*line_position1.S()*line_position2.S() + line_position2.S()*line_position2.S();
    
    //Now the equation becomes: n*p2 + p1 = 0  -->  n = -p1/p2.
    
    //Obviously, 'n' can't be calculated if p2=0, (there is a zero vector or the position is such that no singe projection can be made), so return 0.
    if(p2==0) return 0;
    
    n = -p1/p2;
    
    //If 'n' is in range [0,1] the projection position is ON the vector, if greater than 1 it is outside vector in the direction of the vector,
    // if smaller than 0 it is outside the vector in the opposite direction than the vector direction.
    
    if(n==0) output_projected_position = line_position1;
    else
    {
        double dc = line_position2.C() - line_position1.C();
        double dr = line_position2.R() - line_position1.R();
        double ds = line_position2.S() - line_position1.S();
        
        output_projected_position.C() = line_position1.C() + n*dc;
        output_projected_position.R() = line_position1.R() + n*dr;
        output_projected_position.S() = line_position1.S() + n*ds;
    }
    
    return 1;
}


