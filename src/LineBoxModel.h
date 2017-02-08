//
//  LineBoxModel.hpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef LineCubeModel_hpp
#define LineCubeModel_hpp

#include <stdio.h>
#include "basemodel.h"
#include "vertexbuffer.h"

class LineBoxModel : public BaseModel
{
public:
    LineBoxModel( Vector a, Vector b );
    virtual ~LineBoxModel() {}
    virtual void draw(const BaseCamera& Cam);
protected:
    VertexBuffer VB;
    
};

#endif /* LineCubeModel_hpp */
