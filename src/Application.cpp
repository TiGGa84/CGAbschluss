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

#define ASSET_DIRECTORY "../../assets/"


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin)
{
	Cam.setPosition(Vector(0.0f, 2.0f, -5.0f));

	int w = 0, h = 0;
	glfwGetFramebufferSize(pWin, &w, &h);

	HDRBuffer.createBuffer(w, h);
	Blur.createBuffer(w, h);

	Blur.addInputTexID(HDRBuffer.getOutputBloomTexID());
	
	Tonemap.addInputColorTexID(HDRBuffer.getOutputColorTexID());
	Tonemap.addInputBloomTexID(Blur.getOutputTexID());

	BaseModel* pModel;
	ConstantShader* pConstShader;

	// create LineGrid model with constant color shader
	pModel = new LinePlaneModel(10, 10, 10, 10);
	pConstShader = new ConstantShader();
	pConstShader->color(Color(1, 1, 1));
	pModel->shader(pConstShader, true);
	//Models.push_back(pModel);
	
	pModel = new Model(ASSET_DIRECTORY"grid.dae");
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);
	
	Matrix m;

	pModel = new Model(ASSET_DIRECTORY "plane.dae");
	pModel->shader(new PhongShader(), true);
	m.translation(0, 1, 0);
	pModel->transform(m);
	Models.push_back(pModel);

	
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Application::update(double time, double frametime)
{

	Cam.update();

}

void Application::draw()
{

	ShaderLightMapper::instance().activate();
	HDRBuffer.activate();

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
	{
		(*it)->draw(Cam);
	}

	HDRBuffer.deactivate();

	Blur.process();
	Tonemap.process();

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

	//bool leftDown = glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS;
	//bool rightDown = glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS;

}
