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

#define DEBUG_CAM1
#define GODMODE1

Application::Application(GLFWwindow* pWin, GamestateManager* gm) :
	pWindow(pWin),
	LaneCam(pWin),
	HUDCam(pWin),
	Cam(pWin),
	gm(gm)
{
	int w = 0, h = 0;
	glfwGetFramebufferSize(pWin, &w, &h);

	// Renderbuffer
	HDRBuffer.createBuffer(w, h);
	Blur.createBuffer(w, h);
	Blur.setIterations(2);
	// HDR-Bloombuffer -> Blur
	Blur.addInputTexID(HDRBuffer.getOutputBloomTexID());
	// HDR-Colorbuffer + Blur
	Tonemap.addInputColorTexID(HDRBuffer.getOutputColorTexID());
	Tonemap.addInputBloomTexID(Blur.getOutputTexID());

	// globales licht für highlights
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0.5f, -1.0f, 1.0f));
	dl->color(Color(0.1f, 0.1f, 0.2f));
	ShaderLightMapper::instance().addLight(dl);

	BaseModel* pModel;
	Matrix m;

	pModel = new Model(ASSET_DIRECTORY"street.dae");
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);

	track = new Track(CARSPEED, 40);
	track->shader(new PhongShader(), true);
	track->loadModel(ASSET_DIRECTORY"block.dae");
	m.translation(0, 0, 3.0f);
	track->transform(m);
	Models.push_back(track);

	scenery = new Scenery(CARSPEED, 200);
	scenery->shader(new PhongShader(), true);
	scenery->loadModel(ASSET_DIRECTORY"bgmountains.dae", 40);
	scenery->loadModel(ASSET_DIRECTORY"bgspiral.dae", 120);
	scenery->loadModel(ASSET_DIRECTORY"bgcity.dae", 120);
	scenery->loadModel(ASSET_DIRECTORY"bgtunnel.dae", 100);
	scenery->loadModel(ASSET_DIRECTORY"bgspiral2.dae", 140);
	m.translation(0, -0.1f, 2.0f);
	scenery->transform(m);
	Models.push_back(scenery);

	car = new Car(CARSPEED);
	car->shader(new PhongShader(), true);
	car->loadModels(ASSET_DIRECTORY "NotDelorean.dae", ASSET_DIRECTORY "Vorderachse.dae", ASSET_DIRECTORY "Hinterachse.dae");
	Models.push_back(car);

	HUDElement* speedometer = new HUDElement(0.934f, 0.0f, 0.4f, 0.4f, Texture::LoadShared(ASSET_DIRECTORY "TachoMitFesterNadel.png"));
	speedometer->shader(new HUDShader(), true);
	HUDModels.push_back(speedometer);

	score = new Score(1.0f, 0.015f, 0.04f);
	HUDModels.push_back(score);

	//Dialog Fenster wird bereits geladen aber noch nicht gemalt
	dialog = new HUDElement(0.417f, 0.38f, 0.5f, 0.0f, Texture::LoadShared(ASSET_DIRECTORY "Dialog.png"));
	dialog->shader(new HUDShader(), true);

	dialogScore = new Score(0.55f, 0.548f, 0.04f);

	skybox = new Model(ASSET_DIRECTORY"skybox.dae");
	skybox->shader(new PhongShader(true), true);
}

Application::~Application()
{
	Models.clear();
	HUDModels.clear();
	delete track;
	delete scenery;
	delete score;
	delete car;
	delete dialog;
	delete dialogScore;
}

void Application::start()
{
	crashed = false;
	gamescore = 0;
	leftKeyPressedOnce = false;
	rightKeyPressedOnce = false;

	Tonemap.setOffset(0);
	score->setNumber(0);
	dialogScore->setNumber(0);
	track->reset();
	scenery->reset();
	car->reset();

	LaneCam.setPosition(Vector(0.0f, 1.7f, 3.0f));
	LaneCam.setTarget(Vector(0.0f, 0.4f, 0.0f));
	LaneCam.reset();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Application::update(double time, double frametime)
{
	getInput();
	// Spiel aktiv
	if (gm->getGameState() == 4) {
		gamescore = (unsigned int)(time * CARSPEED);
		score->setNumber(gamescore);

		track->update((float)time);
		scenery->update((float)time);
		car->update((float)frametime, *this);

#ifndef GODMODE
		auto box = car->boundingBox();
		if (track->testIntersesction(box)) {
			gm->setGameState(5);
		}
#endif
	}

	// Crash
	else if (gm->getGameState() == 5) {

		if (Tonemap.getOffst() < 0.01) Tonemap.setOffset(Tonemap.getOffst() + 0.001);
		if (!crashed) {
			crashed = true;
			LaneCam.setLane(0);
			dialogScore->setNumber(gamescore);
		}
	}

#ifdef DEBUG_CAM
	Cam.update();
#else
	LaneCam.update((float)frametime);
#endif
}

void Application::draw()
{
	ShaderLightMapper::instance().activate();
	HDRBuffer.activate();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef DEBUG_CAM
	skybox->draw(Cam);
#else
	skybox->draw(LaneCam);
#endif

	glEnable(GL_DEPTH_TEST);


	for (auto m : Models)
	{
#ifdef DEBUG_CAM
		m->draw(Cam);
#else
		m->draw(LaneCam);
#endif
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

	for (auto m : HUDModels) m->draw(HUDCam);

	GLenum Error = glGetError();
	assert(Error == 0);
}

void Application::drawDialog()
{
	dialog->draw(HUDCam);
	dialogScore->draw(HUDCam);

	GLenum Error = glGetError();
	assert(Error == 0);
}

void Application::getInput() {

	bool leftDown = glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS;
	bool rightDown = glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS;
	bool spaceDown = glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS;
	bool escapeDown = glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS;

	// Wenn Spiel aktiv
	if (gm->getGameState() == 4) {
		if (leftDown && !leftKeyPressedOnce) {
			car->steer(-1);
			LaneCam.switchLane(-1);
			leftKeyPressedOnce = true;
		}
		if (rightDown && !rightKeyPressedOnce) {
			car->steer(1);
			LaneCam.switchLane(1);
			rightKeyPressedOnce = true;
		}
		if (!leftDown) {
			leftKeyPressedOnce = false;
		}
		if (!rightDown) {
			rightKeyPressedOnce = false;
		}
	}

	// Neustart
	if (gm->getGameState() == 5 && spaceDown) {
		gm->setGameState(3);
	}

	// Beenden
	if (gm->getGameState() == 5 && escapeDown) {
		gm->setGameState(6);
	}
}
