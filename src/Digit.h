#pragma once

#include <stdio.h>
#include "basemodel.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class Digit : public BaseModel
{
public:
	Digit(float x, float y, float scaleX, float scaleY, int num);
	virtual ~Digit() {}
	virtual void draw(const BaseCamera& Cam);
protected:
	VertexBuffer VB;
	IndexBuffer IB;
};