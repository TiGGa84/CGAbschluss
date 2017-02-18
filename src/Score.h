#pragma once

#include <stdio.h>
#include <list>

#include "BaseModel.h"

class Score
{
public:
	Score(std::list<BaseModel*>* models, float x, float y, float scaleX, float scaleY, unsigned int num);
	virtual ~Score() {}
private:
	int number[10];
};