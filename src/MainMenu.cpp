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

#include "ShaderLightMapper.h"

#include "HUDElement.h"
#include "HUDShader.h"
#include <stdio.h>

#define ASSET_DIRECTORY "../../assets/"

#define CARSPEED 3.0f


MainMenu::MainMenu(GLFWwindow* pWin, GamestateManager* gm) :
	pWindow(pWin),
	Cam(pWin),
	HUDCam(pWin),
	gm(gm)
{
	Cam.setPosition(Vector(0.0f, 2.0f, 5.0f));

	int w = 0, h = 0;
	glfwGetFramebufferSize(pWin, &w, &h);

	HDRBuffer.createBuffer(w, h);
	Blur.createBuffer(w, h);
	Blur.setIterations(3);

	Blur.addInputTexID(HDRBuffer.getOutputBloomTexID());
	
	Tonemap.addInputColorTexID(HDRBuffer.getOutputColorTexID());
	Tonemap.addInputBloomTexID(Blur.getOutputTexID());

	
	
}

void MainMenu::initModels() {
	HUDElement* titel = new HUDElement(0.17f, 0.66f, 1.0f, 0.0f, Texture::LoadShared(ASSET_DIRECTORY "LightRiderTitle.png"));
	titel->shader(new HUDShader(), true);
	titel->setTextureScale(1.0f, 1.0f);
	HUDModels.push_back(titel);

	HUDElement* pressKeyText = new HUDElement(0.4f, 0.5f, 0.55f, 0.0f, Texture::LoadShared(ASSET_DIRECTORY "PressKeyToStart.png"));
	pressKeyText->shader(new HUDShader(), true);
	pressKeyText->setTextureScale(1.0f, 1.0f);
	HUDModels.push_back(pressKeyText);

	HUDElement* help = new HUDElement(0.4f, 0.2f, 0.55f, 0.0f, Texture::LoadShared(ASSET_DIRECTORY "Help.png"));
	help->shader(new HUDShader(), true);
	help->setTextureScale(1.0f, 1.0f);
	HUDModels.push_back(help);
}

void MainMenu::start()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void MainMenu::update(double time, double frametime)
{
	getInput();
	
}

void MainMenu::draw()
{

	ShaderLightMapper::instance().activate();
	HDRBuffer.activate();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
	{
		(*it)->draw(Cam);
	}

	HDRBuffer.deactivate();

	glDisable(GL_DEPTH_TEST);

	Blur.process();
	Tonemap.process();

	GLenum Error = glGetError();
	assert(Error == 0);
}
void MainMenu::drawHUD()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

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

	Models.clear();

	//for (ModelList::iterator it = HUDModels.begin(); it != HUDModels.end(); ++it)
		//delete *it;

	HUDModels.clear();
}

void MainMenu::getInput() {

	if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
		gm->setGameState(2);
		end();
	}
		
}
