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



#if defined(mxVectorProcessing_USE_SOURCE_CODE) || defined(mxProcessing_USE_SOURCE_CODE)
    #define mxVectorProcessing_API
#else
    #if defined(_MSC_VER)
        #ifdef mxVectorProcessing_EXPORTS
            #define mxVectorProcessing_API __declspec(dllexport)
        #else
            #define mxVectorProcessing_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxVectorProcessing_EXPORTS
            #define mxVectorProcessing_API __attribute__((visibility("default")))
        #else
            #define mxVectorProcessing_API
        #endif
    #endif
#endif



#ifndef mxVectorProcessing_H
    #define mxVectorProcessing_H


#include "mxDataObjectFactory.h"
#include "mxFunctionObject.h"
//#include "mxGeometry.h"
#include "mxPosition.h"


/// Processing of 3D vectors.

class mxVectorProcessing_API mxVectorProcessing : public mxFunctionObject
{    
public:

    /// Constructor.
    mxVectorProcessing(){};
    
    /// Destructor.
    ~mxVectorProcessing(){};
    
    /// Cross product of input vectors. The resulting vector is ORTHOGONAL to the input vectors. Position of the output vector is V1.
    void CrossProduct(double x1, double y1, double z1, double x2, double y2, double z2, double &output_x, double &output_y, double &output_z)
    {
        //         | i  j  k  |
        //         |          |
        // V x Q = | x1 y1 z1 | = (y1z2-y2z1)i − (x1z2-x2z1)j + (x1y2-x2y1)k.
        //         |          |
        //         | x2 y2 z2 |
        
        output_x = y1*z2-y2*z1;
        output_y = -( x1*z2-x2*z1 );
        output_z = x1*y2-x2*y1;
    };
    
    
    /// Cross product of input vectors. The resulting vector is ORTHOGONAL to the input vectors. Position of the output vector is V1.
    void CrossProduct(mxPosition *V1, mxPosition *V2, mxPosition *Q1, mxPosition *Q2, mxPosition *output1, mxPosition *output2)
    {
        double v1 = V2->C() - V1->C();
        double v2 = V2->R() - V1->R();
        double v3 = V2->S() - V1->S();
        
        double q1 = Q2->C() - Q1->C();
        double q2 = Q2->R() - Q1->R();
        double q3 = Q2->S() - Q1->S();
        
        this->CrossProduct(v1,v2,v3, q1,q2,q3, output2->C(),output2->R(),output2->S());
        
        output1->CopyFrom(*V1);
        output2->C() += output1->C();
        output2->R() += output1->R();
        output2->S() += output1->S();
    };
    
    /// Inner product of vectors.
    double InnerProduct(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        return (x2*x1 + y2*y1 + z2*z1);
    };
    
    /// Inner product of vectors.
    double InnerProduct(mxPosition *V1, mxPosition *V2, mxPosition *Q1, mxPosition *Q2)
    {
        return ( (V2->S()-V1->S())*(Q2->S()-Q1->S()) + (V2->R()-V1->R())*(Q2->R()-Q1->R()) + (V2->C()-V1->C())*(Q2->C()-Q1->C()) );
    };
        
    /// Check if the input position is on the line segment.
    int IsPositionOnLineSegment(mxPosition *position, mxPosition *L1, mxPosition *L2);
    
    /// Magnitude (absolute malue) of input vector.
    double Magnitude(double x, double y, double z)
    {
        return( sqrt(x*x + y*y + z*z) );
    };
    
    /// Magnitude (absolute malue) of input vector.
    double Magnitude(mxPosition *V1, mxPosition *V2)
    {
        return( sqrt( (V2->S()-V1->S())*(V2->S()-V1->S()) + (V2->R()-V1->R())*(V2->R()-V1->R()) + (V2->C()-V1->C())*(V2->C()-V1->C()) ) );
    };
    
    /// Scale magnitude with given factor. Position V1 remains unchanged (output1 = V1).
    void Scale(mxPosition *V1, mxPosition *V2, double scale, mxPosition *output1, mxPosition *output2)
    {
        output1->CopyFrom(*V1);
        output2->S() = V1->S() + (V2->S()-V1->S()) * scale;
        output2->R() = V1->R() + (V2->R()-V1->R()) * scale;
        output2->C() = V1->C() + (V2->C()-V1->C()) * scale;
    };
    
    /// Compute unit vector of the given vector.
    int UnitVector(mxPosition *V1, mxPosition *V2, mxPosition *output1, mxPosition *output2)
    {
        double magnitude = this->Magnitude(V1,V2);
        if(magnitude==0) return 0;
        
        double scale = 1.0/magnitude;
        this->Scale(V1,V2,scale,output1,output2);
        
        return 1;
    };
    
	/// Get orthogonal vector to input vectors. Resulting vectoris a unit vector positioned at V1.
	int OrthogonalVectorToInputVectors(mxPosition *V1, mxPosition *V2, mxPosition *Q1, mxPosition *Q2, mxPosition *output1, mxPosition *output2)
    {
        mxPosition C1,C2;
        this->CrossProduct(V1,V2,Q1,Q2,&C1,&C2);
        return this->UnitVector(&C1,&C2,output1,output2);
    };
};

#endif
