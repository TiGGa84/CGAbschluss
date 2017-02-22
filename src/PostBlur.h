#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "PostEffect.h"
#include "BaseShader.h"

// Zwei-pass Gauss Blur, 5x5 Kernel größe
// Wird iterativ angewannt
// Nach Vorlage https://learnopengl.com/#!Advanced-Lighting/Bloom
class PostBlur : public PostEffect
{
public:
	PostBlur();
	virtual ~PostBlur();

	void createBuffer(unsigned int width, unsigned int height);
	void setIterations(unsigned int iterations);

	void addInputTexID(GLuint textureBuffer);
	GLuint getOutputTexID();

	virtual void process();
protected:
	unsigned int Iterations;
	GLint DirLoc;
	GLuint StartColorbufferID;
	GLuint FramebufferID[2];
	GLuint ColorbufferID[2];
};
