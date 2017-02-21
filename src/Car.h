#pragma once

#include "BaseModel.h"

#include <string>

class Application;

class Car : public BaseModel {
public:
	Car(float speed);
	virtual ~Car();
	bool loadModels(std::string ChassisFile, std::string FrontWheelFile, std::string RearWheelFile);
	void steer(int steer);
	void update(float dtime, Application& app);
	virtual void draw(const BaseCamera& Cam);

protected:
	BaseModel* chassis;
	BaseModel* frontWheels;
	BaseModel* rearWheels;

	int desiredLane;
	float speedPerS;
	float wheelAngle;
	float currentPos;
};