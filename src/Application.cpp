//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "Model.h"
#include "ShaderLightMapper.h"
#include "HUDElement.h"
#include "HUDShader.h"

#define ASSET_DIRECTORY "../../assets/"

#define CARSPEED 5.0f

using namespace std;


Application::Application(GLFWwindow* pWin, GamestateManager* gm) :
	pWindow(pWin),
	Cam(pWin),
	HUDCam(pWin),
	gm(gm)
{
	leftKeyPressedOnce = false;
	rightKeyPressedOnce = false;
	Cam.setPosition(Vector(0.0f, 2.1f, 4.7f));

	int w = 0, h = 0;
	glfwGetFramebufferSize(pWin, &w, &h);

	HDRBuffer.createBuffer(w, h);
	Blur.createBuffer(w, h);
	Blur.setIterations(2);

	Blur.addInputTexID(HDRBuffer.getOutputBloomTexID());

	Tonemap.addInputColorTexID(HDRBuffer.getOutputColorTexID());
	Tonemap.addInputBloomTexID(Blur.getOutputTexID());

	// globales licht für highlights
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0.5f, -1.0f, 1.0f));
	dl->color(Color(0.1f, 0.1f, 0.2f));
	ShaderLightMapper::instance().addLight(dl);

}

void Application::initModels() {
	BaseModel* pModel;
	Matrix m;

	pModel = new Model(ASSET_DIRECTORY"street.dae");
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);

	track = new Track(CARSPEED, 40);
	track->shader(new PhongShader(), true);
	track->loadModel(ASSET_DIRECTORY"block.dae");
	m.translation(0, 0, 4.0f);
	track->transform(m);
	Models.push_back(track);

	scenery = new Scenery(CARSPEED, 200);
	scenery->shader(new PhongShader(), true);
	scenery->loadModel(ASSET_DIRECTORY"bgmountains.dae", 40);
	//scenery->loadModel(ASSET_DIRECTORY"bgtunnel.dae", 100);
	//scenery->loadModel(ASSET_DIRECTORY"bgspiral.dae", 240);
	//scenery->loadModel(ASSET_DIRECTORY"bgcity.dae", 240);
	m.translation(0, -0.1f, 4.0f);
	scenery->transform(m);
	Models.push_back(scenery);

	car = new Car(CARSPEED);
	car->shader(new PhongShader(), true);
	car->loadModels(ASSET_DIRECTORY "NotDelorean.dae", ASSET_DIRECTORY "Vorderachse.dae", ASSET_DIRECTORY "Hinterachse.dae");
	Models.push_back(car);

	HUDElement* speedometer = new HUDElement(0.934f, 0.0f, 0.4f, 0.4f, Texture::LoadShared(ASSET_DIRECTORY "TachoMitFesterNadel.png"));
	speedometer->shader(new HUDShader(), true);
	speedometer->setTextureScale(1.0f, 1.0f);
	HUDModels.push_back(speedometer);

	score = new Score(1.0f, 0.015f, 0.04f);
	HUDModels.push_back(score);
}

void Application::start()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Application::update(double time, double frametime)
{
	getInput();
	Cam.update(frametime);
	track->update(frametime);
	scenery->update(frametime);
	score->setNumber((unsigned int)(time * CARSPEED));
	car->update(frametime, *this);
}

void Application::draw()
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
void Application::drawHUD()
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
void Application::end()
{
	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		delete *it;

	Models.clear();

	for (ModelList::iterator it = HUDModels.begin(); it != HUDModels.end(); ++it)
		delete *it;

	HUDModels.clear();
}

void Application::getInput() {

	bool leftDown = glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS;
	bool rightDown = glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS;

	if (leftDown && !leftKeyPressedOnce) {
		car->steer(-1);
		Cam.switchLane(-1);
		leftKeyPressedOnce = true;
	}
	if (rightDown && !rightKeyPressedOnce) {
		car->steer(1);
		Cam.switchLane(1);
		rightKeyPressedOnce = true;
	}
	if (!leftDown) {
		leftKeyPressedOnce = false;
	}
	if (!rightDown) {
		rightKeyPressedOnce = false;
	}
}
