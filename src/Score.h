#pragma once

#include "BaseModel.h"
#include "HUDElement.h"
#include "HUDShader.h"

#include <vector>

// Stellt eine Zahl bis zu 10 Ziffern dar
class Score : public BaseModel
{
public:
	Score(float x, float y, float lineheight);
	virtual ~Score();

	void setNumber(unsigned int n);
	void setPosition(float x, float y);

	virtual void draw(const BaseCamera& Cam);
protected:
	int number[10];
	float width;
	HUDElement* rect;
};