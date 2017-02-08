//
//  Tank.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 16.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#define _USE_MATH_DEFINES

#include <math.h>
#include "Tank.h"
#include "Application.h"


Tank::Tank()
{
    tankMat.identity();
    cannonAngle = 0;
    tankThrottle = 0;
}

Tank::~Tank()
{
    delete chassis;
    delete cannon;
}

bool Tank::loadModels(const char* ChassisFile, const char* CannonFile)
{
    chassis = new Model(ChassisFile, false);
    cannon = new Model(CannonFile, false);
    
    chassis->shader(this->shader());
    cannon->shader(this->shader());
    return true;
}

void Tank::steer(float ForwardBackward, float LeftRight)
{
    // TODO: Add your code
}

void Tank::aim(const Vector& Target)
{
    // TODO: Add your code
}

void Tank::update(float frametime, Application& app)
{
    float steering, throttle;
    app.getInput(steering, throttle);
    
    tankThrottle = tankThrottle + ((throttle - tankThrottle) * 0.05);
    
    Matrix steerMat;
    steerMat.rotationY(steering * frametime * -1);
    
    Matrix throttleMat;
    throttleMat.translation(tankThrottle * frametime, 0, 0);
    
    tankMat *= throttleMat * steerMat;
    
    Vector i = app.getMouseInput();
    
    Vector tankPos = tankMat.translation();
    
    float angle = std::atan2(i.X - tankPos.X, i.Z - tankPos.Z);
    //cannonAngle = cannonAngle + ((angle - cannonAngle) * 0.03);
    cannonAngle = angle;
    
    Matrix cannonMat;
    cannonMat.translation(tankPos);
    
    Matrix cannonRotMat;
    cannonRotMat.rotationY(cannonAngle - M_PI / 2 + M_PI);
    
    chassis->transform(tankMat);
    cannon->transform(cannonMat * cannonRotMat);
}

void Tank::draw(const BaseCamera& Cam)
{
    chassis->draw(Cam);
    cannon->draw(Cam);
}
