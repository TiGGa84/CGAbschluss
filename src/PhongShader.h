//
//  PhongShader.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef PhongShader_hpp
#define PhongShader_hpp

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "color.h"
#include "vector.h"
#include "matrix.h"
#include "camera.h"
#include "baseshader.h"
#include "texture.h"

class PhongShader : public BaseShader
{
public:
	PhongShader();
	// setter
	void diffuseColor(const Color& c);
	void ambientColor(const Color& c);
	void specularColor(const Color& c);
	void specularExp(float exp);
	void emitColor(const Color& c);
	void diffuseTexture(const Texture* pTex);
	void emitTexture(const Texture* pTex);
	//getter
	const Color& diffuseColor() const { return DiffuseColor; }
	const Color& ambientColor() const { return AmbientColor; }
	const Color& specularColor() const { return SpecularColor; }
	float specularExp() const { return SpecularExp; }
	const Color& emitColor() const { return EmitColor; };
	const Texture* diffuseTexture() const { return DiffuseTexture; }
	const Texture* emitTexture() const { return EmitTexture; }

	virtual void activate(const BaseCamera& Cam) const;
private:
	void assignLocations();

	Color DiffuseColor;
	Color SpecularColor;
	Color AmbientColor;
	float SpecularExp;
	Color EmitColor;
	const Texture* DiffuseTexture;
	const Texture* EmitTexture;

	GLint DiffuseColorLoc;
	GLint SpecularColorLoc;
	GLint AmbientColorLoc;
	GLint SpecularExpLoc;
	GLint EmitColorLoc;
	GLint ModelMatLoc;
	GLint ModelViewProjLoc;
	GLint EyePosLoc;
	GLint DiffuseTexLoc;
	GLint EmitTexLoc;

	mutable unsigned int UpdateState;

	enum UPDATESTATES
	{
		DIFF_COLOR_CHANGED = 1 << 0,
		AMB_COLOR_CHANGED = 1 << 1,
		SPEC_COLOR_CHANGED = 1 << 2,
		SPEC_EXP_CHANGED = 1 << 3,
		EMIT_COLOR_CHANGED = 1 << 4,
		DIFF_TEX_CHANGED = 1 << 5,
		EMIT_TEX_CHANGED = 1 << 6
	};

};

#endif /* PhongShader_hpp */
