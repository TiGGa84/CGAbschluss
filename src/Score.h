#pragma once

#include "BaseModel.h"
#include "HUDElement.h"
#include "HUDShader.h"

#include <vector>

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
	HUDShader* hShader;
};