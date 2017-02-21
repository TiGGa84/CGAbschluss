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
#include "LaneCamera.h"
#include "HUDCamera.h"
#include "PhongShader.h"
#include "BaseModel.h"
#include "Track.h"
#include "Scenery.h"
#include "Score.h"
#include "HDRFramebuffer.h"
#include "PostTonemap.h"
#include "PostBlur.h"
#include "Car.h"
#include "GamestateManager.h"

#include <list>

class Application
{
public:
	typedef std::list<BaseModel*> ModelList;
	Application(GLFWwindow* pWin, GamestateManager* gm);
	void start();
	void update(double time, double frametime);
	void draw();
	void drawHUD();
	void end();

	void initModels();
	void initDialog();

	void getInput();

protected:
	GamestateManager* gm;
	LaneCamera LaneCam;
	HUDCamera HUDCam;
	Camera Cam;

	GLFWwindow* pWindow;

	ModelList Models;
	ModelList HUDModels;
	
	Track* track;
	Scenery* scenery;
	Score* score;
	Car* car;
	HUDElement* dialog;
	Score* dialogScore;

	HDRFramebuffer HDRBuffer;
	PostTonemap Tonemap;
	PostBlur Blur;

	bool leftKeyPressedOnce;
	bool rightKeyPressedOnce;
	unsigned int gamescore;
	bool crashed;
};

#endif /* Application_hpp */
