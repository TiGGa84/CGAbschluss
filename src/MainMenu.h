#pragma once

#include "HUDCamera.h"
#include "BaseModel.h"
#include "GamestateManager.h"
#include "RainbowShader.h"

#include <list>

class MainMenu
{
public:
	typedef std::list<BaseModel*> ModelList;
	MainMenu(GLFWwindow* pWin, GamestateManager* gm);
	~MainMenu();
	void start();
	void update(double time, double frametime);
	void draw();
	void end();

	void getInput();

protected:
	GLFWwindow* pWindow;
	GamestateManager* gm;

	HUDCamera HUDCam;

	ModelList HUDModels;

	RainbowShader* rainbow;
};