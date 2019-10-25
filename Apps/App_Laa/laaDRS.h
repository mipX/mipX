#ifndef LAA_DRS_H
#define LAA_DRS_H value
#endif /* ifndef LAA_DRS_H */

#include "mxBIP.h"
#include "mxBasicImage.h"
#include "mxGIP.h"
#include "mxGeometry.h"
#include "mxList.h"
#include "mxIndex.h"
#include "mxPoint.h"
#include "mxString.h"
#include "mxSkeletonization.h"
#include "mxVoxel.h"
#include "vmxImage.h"

class laaSegmentation {
  public:

    /// Constructor
    laaSegmentation();

    /// Destructor
    ~laaSegmentation();

    /// Perform the LAA segmentation
    /// TODO: objasniti argumente 
    int runSegmentation(mxImage &in_image, 
                        mxImage &out_image,
                        int threshold,
                        int seed_slice,
                        int seed_row,
                        int seed_column);
    
};


