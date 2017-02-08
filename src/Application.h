//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "tank.h"

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void start();
    void update(double time, double frametime);
    void draw();
    void end();
    
    void getInput(float &steering, float& throttle);
    Vector getMouseInput();
    
protected:
    Vector calc3DRay( float x, float y, Vector& Pos);
    Camera Cam;
    ModelList Models;
    GLFWwindow* pWindow;
    Tank* pTank;
    Model* pTankTop;
    Model* pTankBot;

};

#endif /* Application_hpp */
