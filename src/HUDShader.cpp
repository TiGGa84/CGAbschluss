#include "HUDShader.h"



HUDShader::HUDShader() :
	pTexture(Texture::defaultTex()),
	offset(Vector(0, 0, 0)),
	scale(Vector(1, 1, 1))
{
	load("vshud.glsl", "fshud.glsl");

	MatLoc = getParameterID("Mat");
	TexScaleLoc = getParameterID("TexScale");
	TexOffsetLoc = getParameterID("TexOffset");
	TexLoc = getParameterID("Tex");
}

void HUDShader::Tex(const Texture * pTex)
{
	pTexture = pTex;
	if (!pTexture) pTexture = Texture::defaultTex();
}

void HUDShader::TexScale(Vector v)
{
	scale = v;
}

void HUDShader::TexOffset(Vector v)
{
	offset = v;
}

void HUDShader::activate(const BaseCamera & Cam) const
{
	BaseShader::activate(Cam);

	setParameter(MatLoc, Cam.getProjectionMatrix() * modelTransform());
	glUniform2f(TexScaleLoc, scale.X, scale.Y);
	glUniform2f(TexOffsetLoc, offset.X, offset.Y);
	pTexture->activate(0);
	setParameter(TexLoc, 0);
}

