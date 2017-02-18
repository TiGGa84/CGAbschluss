//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include "Camera.h"
#include "PhongShader.h"
#include "ConstantShader.h"
#include "BaseModel.h"
#include "Track.h"
#include "HDRFramebuffer.h"
#include "PostTonemap.h"
#include "PostBlur.h"

#include <list>

class Application
{
public:
	typedef std::list<BaseModel*> ModelList;
	Application(GLFWwindow* pWin);
	void start();
	void update(double time, double frametime);
	void draw();
	void end();

	void getInput();

protected:
	Camera Cam;
	ModelList Models;
	GLFWwindow* pWindow;

	Track* track;

	HDRFramebuffer HDRBuffer;
	PostTonemap Tonemap;
	PostBlur Blur;
};

#endif /* Application_hpp */
