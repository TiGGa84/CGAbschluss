//
//  LineBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "LineBoxModel.h"

LineBoxModel::LineBoxModel( Vector a, Vector b  )
{
    VB.begin();
    VB.addVertex(a);
    VB.addVertex(b);
    VB.end();
}

void LineBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);
  
    VB.activate();
    
    glDrawArrays(GL_LINES, 0, VB.vertexCount());
    
    VB.deactivate();
}
