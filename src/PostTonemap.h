#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "PostEffect.h"

// Tonemapping um HDR float Buffer -> LDR Bildschirmbuffer zu mappen
// Mischt zusätzlich zwei Texturbuffer zusammen
class PostTonemap : public PostEffect
{
public:
	PostTonemap();
	virtual ~PostTonemap();

	void addInputColorTexID(GLuint textureBuffer);
	void addInputBloomTexID(GLuint textureBuffer);

	virtual void process();
protected:
	GLuint ColorbufferID[2];

	GLint ColorTexLoc;
	GLint BloomTexLoc;
};