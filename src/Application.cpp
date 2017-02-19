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

#include "lineplanemodel.h"
#include "model.h"
#include "ShaderLightMapper.h"
#include "Score.h"
#include "HUDElement.h"
#include "HUDShader.h"

#define ASSET_DIRECTORY "../../assets/"


Application::Application(GLFWwindow* pWin) :
	pWindow(pWin),
	Cam(pWin),
	HUDCam(pWin)
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

	BaseModel* pModel;
	
	pModel = new Model(ASSET_DIRECTORY"grid.dae");
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);
	
	Matrix m;

	float speed = 3;

	Model* block = new Model(ASSET_DIRECTORY"block.dae");
	block->shader(new PhongShader(), true);

	track = new Track(block, speed, 30);
	m.translation(0, 1, 0);
	track->transform(m);
	Models.push_back(track);

	pModel = new Model(ASSET_DIRECTORY "plane.dae");
	pModel->shader(new PhongShader(), true);
	m.translation(0, 1, 0);
	pModel->transform(m);
	Models.push_back(pModel);
	
	car = new Car();
	car->shader(new PhongShader(), true);
	car->loadModels(ASSET_DIRECTORY "NotDelorean.dae", ASSET_DIRECTORY "Vorderachse.dae", ASSET_DIRECTORY "Hinterachse.dae");
	m.translation(0, 1, 0);
	car->transform(m);
	Models.push_back(car);
	/*
	pModel = new Model(ASSET_DIRECTORY "NotDelorean.dae");
	pModel->shader(new PhongShader(), true);
	m.translation(0, 1, 0);
	pModel->transform(m);
	Models.push_back(pModel);

	pModel = new Model(ASSET_DIRECTORY "Vorderachse.dae");
	pModel->shader(new PhongShader(), true);
	m.translation(0, 1, 0);
	pModel->transform(m);
	Models.push_back(pModel);

	pModel = new Model(ASSET_DIRECTORY "Hinterachse.dae");
	pModel->shader(new PhongShader(), true);
	m.translation(0, 1, 0);
	pModel->transform(m);
	Models.push_back(pModel);
	*/

	score = new Score(1.0f, 0.1f, 0.05f);
	HUDModels.push_back(score);
	
	// directional lights
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(1, -1, 1));
	dl->color(Color(0.1, 0.1, 0.1));
	ShaderLightMapper::instance().addLight(dl);
	
	// point lights
	PointLight* pl = new PointLight();
	pl->position(Vector(0, 3, -10));
	pl->color(Color(3.0f, 2.0f, 0.0f));
	ShaderLightMapper::instance().addLight(pl);

	// spot lights
	SpotLight* sl = new SpotLight();
	sl->position(Vector(0, 2, 0));
	sl->color(Color(0.0f, 1.0f, 2.0f));
	sl->direction(Vector(4, -4, 0));
	sl->innerRadius(45);
	sl->outerRadius(50);
	ShaderLightMapper::instance().addLight(sl);

	sl = new SpotLight();
	sl->position(Vector(0, 3, 10));
	sl->color(Color(0.0f, 1.0f, 0.0f));
	sl->direction(Vector(0, -4, 0));
	sl->innerRadius(50);
	sl->outerRadius(60);
	ShaderLightMapper::instance().addLight(sl);
	
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
	Cam.update();
	track->update(frametime);
	score->setNumber((unsigned int)time);
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
}

void Application::getInput() {

	bool leftDown = glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS;
	bool rightDown = glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS;

	if (leftDown) car->steer(-1);
	else if (rightDown) car->steer(1);
}
