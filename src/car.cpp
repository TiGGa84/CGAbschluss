#include "Car.h"
#include "Application.h"
#include <stdio.h>
#define M_PI 3.14159265

Car::Car()
{
	carMat.identity();
	wheelAngle = 0;
	desiredLane = 0;
	currentPos = 0.0f;
}

Car::~Car()
{
	delete chassis;
	delete frontWheels;
	delete rearWheels;
}

bool Car::loadModels(const char* ChassisFile, const char* FrontWheelFile, const char* RearWheelFile)
{
	chassis = new Model(ChassisFile, false);
	frontWheels = new Model(FrontWheelFile, false);
	rearWheels = new Model(RearWheelFile, false);

	chassis->shader(this->shader());
	frontWheels->shader(this->shader());
	rearWheels->shader(this->shader());
	return true;
}

void Car::steer(int steer)
{
	
	if (desiredLane > -1 && steer < 0) desiredLane--;
	else if (desiredLane < 1 && steer > 0) desiredLane++;
	std::cout << "Desired Lane: " << desiredLane << std::endl << "Steer: " << steer << std::endl;
}

void Car::aim(const Vector& Target)
{
	// TODO: Add your code
}

void Car::update(float frametime, Application& app)
{
	float carOffsetY = 1.0f;
	float wheelSpeed = -50.0f;

	Matrix steerMat;
	steerMat.translation(0, 0, 0);
	float change = 0.0f;
	if (desiredLane+0.00001f < currentPos) {
		change = -0.1f;
		currentPos += change;
	}
	else if (desiredLane-0.0001f > currentPos) {
		change = 0.1f;
		currentPos += change;
	}

	steerMat.translation(change, 0, 0);

	carMat *= steerMat;
	chassis->transform(carMat);

	Vector CarPos = carMat.translation();

	if (wheelAngle <= -360) {
		wheelAngle += 360;
		
	}
	wheelAngle += wheelSpeed * frametime;
	std::cout << wheelAngle << std::endl;
	Matrix chassisMat;
	chassisMat.translation(CarPos);
	Matrix carOffset;
	carOffset.translation(0, carOffsetY, 0);
	Matrix wheelsMat;
	wheelsMat.translation(CarPos);
	Matrix frontOffset;
	frontOffset.translation(0, carOffsetY+0.12474f, -0.57477f);
	Matrix rearOffset;
	rearOffset.translation(0, carOffsetY+0.14376f, 0.50472f);

	Matrix wheelsRotMat;
	wheelsRotMat.rotationX(wheelAngle);
	chassis->transform(chassisMat * carOffset);
	frontWheels->transform(wheelsMat* frontOffset * wheelsRotMat);

	rearWheels->transform(wheelsMat * rearOffset * wheelsRotMat);
	
}

void Car::draw(const BaseCamera& Cam)
{
	chassis->draw(Cam);
	frontWheels->draw(Cam);
	rearWheels->draw(Cam);
}