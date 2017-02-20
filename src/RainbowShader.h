#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "BaseShader.h"
#include "Texture.h"

class RainbowShader : public BaseShader
{
public:
	RainbowShader();

	void setTime(float time);

	virtual void activate(const BaseCamera& Cam) const;
private:
	float time;
	GLint MatLoc;
	GLint TimeLoc;
};
