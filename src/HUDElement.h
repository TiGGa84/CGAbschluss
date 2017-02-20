#pragma once

#include "BaseModel.h"
#include "VertexBuffer.h"
#include "Texture.h"

class HUDElement : public BaseModel
{
public:
	HUDElement(float x, float y, float sizeX, float sizeY = 0, const Texture* pTex = Texture::defaultTex());

	virtual void draw(const BaseCamera& Cam);
	void setPosition(float x, float y);
	void setTexture(const Texture* pTex);
	void setTextureScale(float x, float y);
	void setTextureOffset(float x, float y);
protected:
	VertexBuffer VB;
	const Texture* pTexture;
	Vector scale;
	Vector offset;
};

