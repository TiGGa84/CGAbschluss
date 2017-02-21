#pragma once

#include "BaseModel.h"
#include "ShaderLightMapper.h"
#include "Aabb.h"

#include <string>

class Application;

class Car : public BaseModel {
public:
	Car(float speed);
	virtual ~Car();

	void reset();
	bool loadModels(std::string ChassisFile, std::string FrontWheelFile, std::string RearWheelFile);
	void steer(int steer);
	void update(float frametime, Application& app);
	virtual void draw(const BaseCamera& Cam);
	const AABB& boundingBox() const;

protected:

	float speedPerS;
	int desiredLane;
	float wheelAngle;
	float currentPos;

	AABB Box;

	BaseModel* chassis;
	BaseModel* frontWheels;
	BaseModel* rearWheels;

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