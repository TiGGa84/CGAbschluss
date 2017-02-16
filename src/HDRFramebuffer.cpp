#include "HDRFramebuffer.h"

#include <iostream>
#include <exception>

HDRFramebuffer::HDRFramebuffer() {}

HDRFramebuffer::~HDRFramebuffer() {}

void HDRFramebuffer::createBuffer(unsigned int width, unsigned int height)
{
	// 16 Bit float Framebuffer zum rendern
	glGenFramebuffers(1, &HDRFramebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, HDRFramebufferID);
	// 2 Texturen für normales Bild und Bloom
	glGenTextures(2, ColorbufferID);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, ColorbufferID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// "clamp" damit effekte gut aussehen
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// Beide Texturen an Framebuffer binden
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ColorbufferID[i], 0);
	}
	// depth buffer zum rendern (keine Textur, da kein samplen notwendig)
	glGenRenderbuffers(1, &DepthbufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthbufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// An Framebuffer binden
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthbufferID);
	// wir wollen beide Texturen als Ausgabe haben
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!" << std::endl;
		throw std::exception();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDRFramebuffer::activate() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, HDRFramebufferID);
}

void HDRFramebuffer::deactivate() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint HDRFramebuffer::getOutputColorTexID()
{
	return ColorbufferID[0];
}

GLuint HDRFramebuffer::getOutputBloomTexID()
{
	return ColorbufferID[1];
}
