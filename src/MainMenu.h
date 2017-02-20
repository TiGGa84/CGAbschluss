#pragma once

#include "Camera.h"
#include "HUDCamera.h"
#include "PhongShader.h"
#include "ConstantShader.h"
#include "BaseModel.h"
#include "HDRFramebuffer.h"
#include "PostTonemap.h"
#include "PostBlur.h"
#include "GamestateManager.h"

#include <list>

class MainMenu
{
public:
	typedef std::list<BaseModel*> ModelList;
	MainMenu(GLFWwindow* pWin, GamestateManager* gm);
	void start();
	void update(double time, double frametime);
	void draw();
	void drawHUD();
	void end();

	void initModels();

	void getInput();

protected:
	GamestateManager* gm;
	Camera Cam;
	HUDCamera HUDCam;

	GLFWwindow* pWindow;

	ModelList Models;
	ModelList HUDModels;

	HDRFramebuffer HDRBuffer;
	PostTonemap Tonemap;
	PostBlur Blur;
};