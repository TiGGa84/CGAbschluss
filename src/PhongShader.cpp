//
//  PhongShader.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "PhongShader.h"

#include <assert.h>

PhongShader::PhongShader(bool isStatic) :
	DiffuseColor(1, 1, 1),
	SpecularColor(0.5f, 0.5f, 0.5f),
	AmbientColor(0.2f, 0.2f, 0.2f),
	SpecularExp(10.0f),
	EmitColor(0, 0, 0),
	DiffuseTexture(Texture::defaultTex()),
	EmitTexture(Texture::defaultEmitTex()),
	UpdateState(0xFFFFFFFF),
	isStatic(isStatic)
{
	load("vsphong.glsl", "fsphong.glsl");
	assignLocations();
}

void PhongShader::assignLocations()
{
	DiffuseColorLoc = getParameterID("DiffuseColor");
	AmbientColorLoc = getParameterID("AmbientColor");
	SpecularColorLoc = getParameterID("SpecularColor");
	SpecularExpLoc = getParameterID("SpecularExp");
	EmitColorLoc = getParameterID("EmitColor");
	DiffuseTexLoc = getParameterID("DiffuseTexture");
	EmitTexLoc = getParameterID("EmitTexture");
	EyePosLoc = getParameterID("EyePos");
	ModelMatLoc = getParameterID("ModelMat");
	ModelViewProjLoc = getParameterID("ModelViewProjMat");
}
void PhongShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	// update uniforms if necessary
	if (UpdateState & DIFF_COLOR_CHANGED)
		setParameter(DiffuseColorLoc, DiffuseColor);
	if (UpdateState & AMB_COLOR_CHANGED)
		setParameter(AmbientColorLoc, AmbientColor);
	if (UpdateState & SPEC_COLOR_CHANGED)
		setParameter(SpecularColorLoc, SpecularColor);
	if (UpdateState & SPEC_EXP_CHANGED)
		setParameter(SpecularExpLoc, SpecularExp);
	if (UpdateState & EMIT_COLOR_CHANGED)
		setParameter(EmitColorLoc, EmitColor);

	DiffuseTexture->activate(0);
	if (UpdateState & DIFF_TEX_CHANGED)
		setParameter(DiffuseTexLoc, 0);
	EmitTexture->activate(1);
	if (UpdateState & EMIT_TEX_CHANGED)
		setParameter(EmitTexLoc, 1);

	// update matrices
	Matrix view = Cam.getViewMatrix();
	if (isStatic) view.clearTranslation();
	setParameter(ModelViewProjLoc, Cam.getProjectionMatrix() * view * modelTransform());
	setParameter(ModelMatLoc, modelTransform());
	// update Eye Position
	setParameter(EyePosLoc, Cam.position());

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

void PhongShader::emitColor(const Color & c)
{
	EmitColor = c;
	UpdateState |= EMIT_COLOR_CHANGED;
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
	if (!EmitTexture) EmitTexture = Texture::defaultEmitTex();
	UpdateState |= EMIT_TEX_CHANGED;
}
