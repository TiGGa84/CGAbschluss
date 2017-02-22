#include "PostTonemap.h"

PostTonemap::PostTonemap() : offset(0)
{
	Shader.load("vspost.glsl", "fstonemap.glsl");
	OffsetLoc = Shader.getParameterID("offset");
	ColorTexLoc = Shader.getParameterID("colorTex");
	BloomTexLoc = Shader.getParameterID("bloomTex");
}

PostTonemap::~PostTonemap() {}

void PostTonemap::setOffset(float o) { offset = o; }

float PostTonemap::getOffst() { return offset; }

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
	Shader.setParameter(OffsetLoc, offset);
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

