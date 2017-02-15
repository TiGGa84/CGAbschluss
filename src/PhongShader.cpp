//
//  PhongShader.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "PhongShader.h"

#include <assert.h>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

PhongShader::PhongShader() :
	DiffuseColor(0.8f, 0.8f, 0.8f),
	SpecularColor(0.5f, 0.5f, 0.5f),
	AmbientColor(0.2f, 0.2f, 0.2f),
	SpecularExp(20.0f),
	LightPos(20.0f, 20.0f, 20.0f),
	LightColor(1, 1, 1),
	DiffuseTexture(Texture::defaultTex()),
	UpdateState(0xFFFFFFFF)
{
	load(ASSET_DIRECTORY"vsphong.glsl", ASSET_DIRECTORY"fsphong.glsl");
	assignLocations();
}

void PhongShader::assignLocations()
{
	DiffuseColorLoc = glGetUniformLocation(ShaderProgram, "DiffuseColor");
	AmbientColorLoc = glGetUniformLocation(ShaderProgram, "AmbientColor");
	SpecularColorLoc = glGetUniformLocation(ShaderProgram, "SpecularColor");
	SpecularExpLoc = glGetUniformLocation(ShaderProgram, "SpecularExp");
	DiffuseTexLoc = glGetUniformLocation(ShaderProgram, "DiffuseTexture");
	EmitTexLoc = glGetUniformLocation(ShaderProgram, "EmitTexture");
	LightPosLoc = glGetUniformLocation(ShaderProgram, "LightPos");
	LightColorLoc = glGetUniformLocation(ShaderProgram, "LightColor");
	EyePosLoc = glGetUniformLocation(ShaderProgram, "EyePos");
	ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
	ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
}
void PhongShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	// update uniforms if necessary
	if (UpdateState & DIFF_COLOR_CHANGED)
		glUniform3f(DiffuseColorLoc, DiffuseColor.R, DiffuseColor.G, DiffuseColor.B);
	if (UpdateState & AMB_COLOR_CHANGED)
		glUniform3f(AmbientColorLoc, AmbientColor.R, AmbientColor.G, AmbientColor.B);
	if (UpdateState & SPEC_COLOR_CHANGED)
		glUniform3f(SpecularColorLoc, SpecularColor.R, SpecularColor.G, SpecularColor.B);
	if (UpdateState & SPEC_EXP_CHANGED)
		glUniform1f(SpecularExpLoc, SpecularExp);

	DiffuseTexture->activate(0);
	if (UpdateState & DIFF_TEX_CHANGED && DiffuseTexture)
		glUniform1i(DiffuseTexLoc, 0);
	EmitTexture->activate(1);
	if (UpdateState & EMIT_TEX_CHANGED)
		glUniform1i(EmitTexLoc, 1);

	if (UpdateState & LIGHT_COLOR_CHANGED)
		glUniform3f(LightColorLoc, LightColor.R, LightColor.G, LightColor.B);
	if (UpdateState & LIGHT_POS_CHANGED)
		glUniform3f(LightPosLoc, LightPos.X, LightPos.Y, LightPos.Z);

	// always update matrices
	Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
	glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);

	Vector EyePos = Cam.position();
	glUniform3f(EyePosLoc, EyePos.X, EyePos.Y, EyePos.Z);

	UpdateState = 0x0;
}
void PhongShader::diffuseColor(const Color& c)
{
	DiffuseColor = c;
	UpdateState |= DIFF_COLOR_CHANGED;
}
void PhongShader::ambientColor(const Color& c)
{
	AmbientColor = c;
	UpdateState |= AMB_COLOR_CHANGED;
}
void PhongShader::specularColor(const Color& c)
{
	SpecularColor = c;
	UpdateState |= SPEC_COLOR_CHANGED;
}
void PhongShader::specularExp(float exp)
{
	SpecularExp = exp;
	UpdateState |= SPEC_EXP_CHANGED;
}
void PhongShader::lightPos(const Vector& pos)
{
	LightPos = pos;
	UpdateState |= LIGHT_POS_CHANGED;
}
void PhongShader::lightColor(const Color& c)
{
	LightColor = c;
	UpdateState |= LIGHT_COLOR_CHANGED;
}

void PhongShader::diffuseTexture(const Texture* pTex)
{
	DiffuseTexture = pTex;
	if (!DiffuseTexture) DiffuseTexture = Texture::defaultTex();
	UpdateState |= DIFF_TEX_CHANGED;
}

void PhongShader::emitTexture(const Texture * pTex)
{
	EmitTexture = pTex;
	if (!EmitTexture) EmitTexture = Texture::defaultTex();
	UpdateState |= EMIT_TEX_CHANGED;
}
