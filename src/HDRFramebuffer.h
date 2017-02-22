#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>

// 16 Bit float Buffer zurm Rendern der Szene
// Nach Vorlage https://learnopengl.com/#!Advanced-Lighting/HDR
class HDRFramebuffer
{
public:
	HDRFramebuffer();
	virtual ~HDRFramebuffer();

	void createBuffer(unsigned int width, unsigned int height);

	void activate() const;
	void deactivate() const;

	GLuint getOutputColorTexID();
	GLuint getOutputBloomTexID();
private:
	GLuint HDRFramebufferID;
	GLuint ColorbufferID[2];
	GLuint DepthbufferID;
};