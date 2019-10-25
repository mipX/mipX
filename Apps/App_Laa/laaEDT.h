#ifndef LAA_EDT_H
#define LAA_EDT_H value

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

class laaEDT {
  public:
    /// Constructor
    laaEDT();

    /// Destructor
    ~laaEDT();

    /// Create Radius Image by performing euclidean distance transform
    int createMaxRadiusImageSITK(mxImage &image, mxImage &out_image);

};

#endif /* ifndef LAA_EDT_H */
