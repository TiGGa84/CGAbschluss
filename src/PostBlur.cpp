#include "PostBlur.h"

#include <iostream>
#include <exception>

PostBlur::PostBlur() : Iterations(5)
{
	Shader.load("vspost.glsl", "fsblur.glsl");
	DirLoc = Shader.getParameterID("horizontal");
}

PostBlur::~PostBlur() {}

void PostBlur::createBuffer(unsigned int width, unsigned int height)
{
	// 2 Ping-pong Framebuffer mit je einer Textur
	// Werden zum iterativen ausführen der Blur-passes genutzt
	glGenFramebuffers(2, FramebufferID);
	glGenTextures(2, ColorbufferID);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID[i]);
		glBindTexture(GL_TEXTURE_2D, ColorbufferID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorbufferID[i], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer not complete!" << std::endl;
			throw std::exception();
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostBlur::setIterations(unsigned int iterations)
{
	Iterations = iterations;
}

void PostBlur::addInputTexID(GLuint textureBuffer)
{
	StartColorbufferID = textureBuffer;
}

GLuint PostBlur::getOutputTexID()
{
	return ColorbufferID[0];
}

void PostBlur::process()
{
	glDisable(GL_DEPTH_TEST);

	Shader.activate(Camera(NULL));

	glActiveTexture(GL_TEXTURE0);

	bool horizontal = true;
	for (GLuint i = 0; i < Iterations * 2; i++)
	{
		// Zwischen Framebuffern und horizontalen / vertikalen Passes tauschen
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID[horizontal]);
		glUniform1i(DirLoc, horizontal);
		glBindTexture(GL_TEXTURE_2D, i == 0 ? StartColorbufferID : ColorbufferID[!horizontal]);
		drawQuad();
		horizontal = !horizontal;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader.deactivate();
}

