#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "BaseShader.h"

#include <vector>

// Abstrakte Klasse für Postprocessing Effekte
// malt Geometire über gesammten Viewport
class PostEffect
{
public:
	PostEffect();
	virtual ~PostEffect();

	virtual void process() = 0;
protected:
	virtual void drawQuad() const;

	static GLuint QuadVAOID;
	static GLuint QuadVBOID;

	BaseShader Shader;
};