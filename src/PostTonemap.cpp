#include "PostTonemap.h"

PostTonemap::PostTonemap()
{
	Shader.load("vspost.glsl", "fstonemap.glsl");
	ColorTexLoc = Shader.getParameterID("colorTex");
	BloomTexLoc = Shader.getParameterID("bloomTex");
}

PostTonemap::~PostTonemap(){}

void PostTonemap::addInputColorTexID(GLuint textureBuffer)
{
	ColorbufferID[0] = textureBuffer;
}

void PostTonemap::addInputBloomTexID(GLuint textureBuffer)
{
	ColorbufferID[1] = textureBuffer;
}

void PostTonemap::process()
{
	// Post Process Shader ausführen
	Shader.activate(Camera(NULL));
	// Input Texturen
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ColorbufferID[0]);
	Shader.setParameter(ColorTexLoc, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ColorbufferID[1]);
	Shader.setParameter(BloomTexLoc, 1);
	// draw
	drawQuad();

	Shader.deactivate();
}

