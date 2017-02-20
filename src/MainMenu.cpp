//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "MainMenu.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "HUDElement.h"
#include "HUDShader.h"



#include <cassert>


#define ASSET_DIRECTORY "../../assets/"

using namespace std;



MainMenu::MainMenu(GLFWwindow* pWin, GamestateManager* gm) :
	pWindow(pWin),
	HUDCam(pWin),
	gm(gm) {

}

void MainMenu::initModels() {

	int w = 0, h = 0;
	glfwGetFramebufferSize(pWindow, &w, &h);

	rainbow = new RainbowShader();
	HUDElement* bg = new HUDElement(0, 0, (float)w / (float)h, 1);
	bg->shader(rainbow, true);
	HUDModels.push_back(bg);

	HUDElement* titel = new HUDElement(0.17f, 0.66f, 1.0f, 0.0f, Texture::LoadShared(ASSET_DIRECTORY "LightRiderTitle.png"));
	titel->shader(new HUDShader(), true);
	HUDModels.push_back(titel);

	HUDElement* pressKeyText = new HUDElement(0.4f, 0.5f, 0.55f, 0.0f, Texture::LoadShared(ASSET_DIRECTORY "PressKeyToStart.png"));
	pressKeyText->shader(new HUDShader(), true);
	HUDModels.push_back(pressKeyText);

	HUDElement* help = new HUDElement(0.4f, 0.2f, 0.55f, 0.0f, Texture::LoadShared(ASSET_DIRECTORY "Help.png"));
	help->shader(new HUDShader(), true);
	HUDModels.push_back(help);
}

void MainMenu::start()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void MainMenu::update(double time, double frametime)
{
	getInput();
	rainbow->setTime((float)time);
}

void MainMenu::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (ModelList::iterator it = HUDModels.begin(); it != HUDModels.end(); ++it)
	{
		(*it)->draw(HUDCam);
	}

	GLenum Error = glGetError();
	assert(Error == 0);
}
void MainMenu::end()
{
	for (ModelList::iterator it = HUDModels.begin(); it != HUDModels.end(); ++it)
		delete *it;

	HUDModels.clear();
}

void MainMenu::getInput() {

	if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
		gm->setGameState(2);
		end();
	}
		
}
