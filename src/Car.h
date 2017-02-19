#pragma once

#include "BaseModel.h"

class Application;

class Car : public BaseModel {
public:
	Car();
	virtual ~Car();
	bool loadModels(const char* ChassisFile, const char* FrontWheelFile, const char* RearWheelFile);
	void steer(int steer);
	void aim(const Vector& Target);
	void update(float dtime, Application& app);
	virtual void draw(const BaseCamera& Cam);

protected:
	BaseModel* chassis;
	BaseModel* frontWheels;
	BaseModel* rearWheels;

	Matrix carMat;

	float wheelAngle;
	int currentLane;
	int desiredLane;
	float currentPos;
};