#pragma once

#include "BaseModel.h"
#include "ShaderLightMapper.h"

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

	Matrix frontOffset;
	Matrix rearOffset;

	SpotLight* frontLLight;
	SpotLight* frontRLight;
	SpotLight* backLLight;
	SpotLight* backRLight;

	Vector frontLOffset;
	Vector frontROffset;
	Vector backLOffset;
	Vector backROffset;
};