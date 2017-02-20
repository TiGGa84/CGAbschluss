#include "HUDElement.h"
#include "HUDShader.h"



HUDElement::HUDElement(float x, float y, float sizeX, float sizeY, const Texture* pTex) :
	pTexture(pTex),
	scale(Vector(1, 1, 1)),
	offset(Vector(0, 0, 0))
{
	setPosition(x, y);

	if (sizeY == 0) {
		float factor = sizeX / pTex->width();
		sizeY = factor * pTex->height();
	}

	// Viereck mit UV Koordinaten
	// Draw als Strip ohne Indexbuffer
	VB.begin();

	VB.addTexcoord0(0, 0);
	VB.addVertex(0, sizeY, 0);

	VB.addTexcoord0(0, 1);
	VB.addVertex(0, 0, 0);

	VB.addTexcoord0(1, 0);
	VB.addVertex(sizeX, sizeY, 0);

	VB.addTexcoord0(1, 1);
	VB.addVertex(sizeX, 0, 0);

	VB.end();

}

void HUDElement::draw(const BaseCamera & Cam)
{
	HUDShader* shader = dynamic_cast<HUDShader*>(pShader);
	if (shader) {
		shader->Tex(pTexture);
		shader->TexOffset(offset);
		shader->TexScale(scale);
	}

	BaseModel::draw(Cam);
	VB.activate();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, VB.vertexCount());
	VB.deactivate();
}

void HUDElement::setPosition(float x, float y)
{
	Matrix m;
	m.translation(x, y, 0);
	transform(m);
}

void HUDElement::setTexture(const Texture * pTex)
{
	pTexture = pTex;
}

void HUDElement::setTextureScale(float x, float y)
{
	scale.X = x;
	scale.Y = y;
}

void HUDElement::setTextureOffset(float x, float y)
{
	offset.X = x;
	offset.Y = y;
}


