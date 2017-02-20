#pragma once

#include "HUDCamera.h"
#include "BaseModel.h"
#include "GamestateManager.h"
#include "RainbowShader.h"
#include "irrKlang.h"

#include <list>

#pragma comment(lib, "irrKlang.lib")

class MainMenu
{
public:
	typedef std::list<BaseModel*> ModelList;
	MainMenu(GLFWwindow* pWin, GamestateManager* gm);
	void start();
	void update(double time, double frametime);
	void draw();
	void end();

	void initModels();

	void getInput();

protected:
	irrklang::ISoundEngine* se;
	GLFWwindow* pWindow;
	GamestateManager* gm;

	HUDCamera HUDCam;

	ModelList HUDModels;

	RainbowShader* rainbow;
};