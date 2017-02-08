//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "scene.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin)
{
    BaseModel* pModel;
    ConstantShader* pConstShader;
    PhongShader* pPhongShader;
    
    // create LineGrid model with constant color shader
    pModel = new LinePlaneModel(10, 10, 10, 10);
    pConstShader = new ConstantShader();
	pConstShader->color( Color(1,1,1));
    pModel->shader(pConstShader, true);
    Models.push_back( pModel );

    // Exercise 1
    /*
    pTankTop = new Model(ASSET_DIRECTORY "tank_top.dae", false);
    pTankTop->shader(new PhongShader(), true);
    Models.push_back(pTankTop);
    pTankBot = new Model(ASSET_DIRECTORY "tank_bottom.dae", false);
    pTankBot->shader(new PhongShader(), true);
    Models.push_back(pTankBot);
    */
    
    // Exercise 2
    ///*
    pPhongShader = new PhongShader();
    pTank = new Tank();
    pTank->shader(pPhongShader, true);
    pTank->loadModels(ASSET_DIRECTORY "tank_bottom.dae", ASSET_DIRECTORY "tank_top.dae");
    Models.push_back(pTank);
    //*/
    
    // Exercise 3
    /*
    Scene* pScene = new Scene();
    pScene->shader(new PhongShader(), true);
    pScene->addSceneFile(ASSET_DIRECTORY "scene.osh");
    Models.push_back(pScene);
    */
    
    //testModel = new TriangleSphereModel(.1, 10, 10);
    //testModel->shader(new PhongShader(), true);
    //Models.push_back(testModel);
    
}
void Application::start()
{
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Application::update(double time, double frametime)
{
    // Exercise 1
    /*
    Matrix TM;
    TM.translation(0, 0, 4);
    
    Matrix RM;
    RM.rotationY(time * ((M_PI*2)/6) * -1);
    
    Matrix IRM = RM;
    IRM.invert();
    
    Matrix bottomRM;
    bottomRM.rotationY(M_PI);
    
    Matrix topRM;
    topRM.rotationY(-(M_PI/2));
    
    pTankBot->transform(RM * TM * bottomRM);
    
    pTankTop->transform(RM * TM * IRM * topRM);
    */
    
    // Exercise 2
    pTank->update(frametime, *this);
    
    Cam.update();
    
}

// Pos:Ray Origin
// return:Ray Direction
Vector Application::calc3DRay( float x, float y, Vector& Pos)
{
    int width, height;
    glfwGetWindowSize(pWindow, &width, &height);

    x = (x / (width / 2.0f)) - 1.0f;
    y = (y / (height / 2.0f)) - 1.0f;
    
    Vector vpVec(x,-y,0);
    
    Matrix invProjMat = Cam.getProjectionMatrix();
    invProjMat.invert();
    
    Vector camVec = invProjMat.transformVec4x4(vpVec);
    camVec.normalize();

    Matrix invViewMat = Cam.getViewMatrix();
    invViewMat.invert();
    
    Pos = invViewMat.translation();
    
    Vector worldVec = invViewMat.transformVec3x3(camVec);
    
    return worldVec;
}

void Application::draw()
{
    // 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. setup shaders and draw models
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
    {
        (*it)->draw(Cam);
    }
    
    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}
void Application::end()
{
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
        delete *it;
    
    Models.clear();
}

void Application::getInput(float &steering, float &throttle) {
    steering = 0;
    throttle = 0;
    
    bool leftDown = glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS;
    bool rightDown = glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS;
    
    bool upDown = glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS;
    bool downDown = glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS;
    
    bool shiftDown = glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    
    if(leftDown) steering = -1;
    else if (rightDown) steering = 1;
    
    if(upDown) throttle = 1;
    else if (downDown) throttle = -1;
    
    if(shiftDown) throttle *= 2.0f;
}

Vector Application::getMouseInput() {
    double xpos, ypos;
    glfwGetCursorPos(pWindow, &xpos, &ypos);
    
    Vector origin;
    Vector dir = calc3DRay(xpos, ypos, origin);
    Vector center(0,0,0);
    Vector normal(0,1,0);
    return origin.planeIntersection(dir, center, normal);
}
