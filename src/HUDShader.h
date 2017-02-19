#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "BaseShader.h"
#include "Texture.h"

// Shader für HUD Elemente
// Wendet nur Model-Matrix und Projection-Matrix an
// Ohne View-Matrix bleibt das Koordinatensystem fest vor der Kamera
class HUDShader : public BaseShader
{
public:
	HUDShader();

	void Tex(const Texture* pTex);
	// Manipulation von UV-Koordinaten
	void TexScale(Vector v);
	void TexOffset(Vector v);

	virtual void activate(const BaseCamera& Cam) const;
private:
	const Texture* pTexture;
	Vector offset;
	Vector scale;

	GLint MatLoc;
	GLint TexScaleLoc;
	GLint TexOffsetLoc;
	GLint TexLoc;
};
