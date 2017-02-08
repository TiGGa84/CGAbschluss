//
//  Tank.hpp
//  CGXcode
//
//  Created by Philipp Lensing on 16.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Tank_hpp
#define Tank_hpp

#include <stdio.h>
#include "model.h"

class Application;

class Tank : public BaseModel
{
public:
    Tank();
    virtual ~Tank();
    bool loadModels(const char* ChassisFile, const char* CannonFile);
    void steer(float ForwardBackward, float LeftRight);
    void aim(const Vector& Target);
    void update(float dtime, Application& app);
    virtual void draw(const BaseCamera& Cam);
protected:
    BaseModel* chassis;
    BaseModel* cannon;
    
    Matrix tankMat;
    
    float cannonAngle;
    float tankThrottle;
};

#endif /* Tank_hpp */
