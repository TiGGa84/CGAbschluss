#include "RainbowShader.h"

RainbowShader::RainbowShader() :
	time(0)
{
	load("vshud.glsl", "fsrainbow.glsl");

	MatLoc = getParameterID("Mat");
	TimeLoc = getParameterID("GlobalTime");
}

void RainbowShader::setTime(float time)
{
	this->time = time;
}

void RainbowShader::activate(const BaseCamera & Cam) const
{
	BaseShader::activate(Cam);
	setParameter(MatLoc, Cam.getProjectionMatrix() * modelTransform());
	setParameter(TimeLoc, time);
}

