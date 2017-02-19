#include "Car.h"
#include "Application.h"
#include <stdio.h>
#define M_PI 3.14159265

Car::Car()
{
	carMat.identity();
	wheelAngle = 0;
	currentLane = 0;
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
	//Matrix steerMat;
	//steerMat.rotationY(steering * frametime * -1);

	Matrix throttleMat;
	throttleMat.translation(0, 0, 0);
	float change = 0.0f;
	if (desiredLane+0.00001f < currentPos) {
		change = -0.1f;
		currentPos += change;
		//currentLane--;
	}
	else if (desiredLane-0.0001f > currentPos) {
		change = 0.1f;
		currentPos += change;
		//currentLane++;
	}

	throttleMat.translation(change, 0, 0);
		
	
	

	carMat *= throttleMat;
	chassis->transform(carMat);

	//cannon->transform(CarMat);

	//Vector i = app.getMouseInput();

	Vector CarPos = carMat.translation();

	//float angle = std::atan2(i.X - CarPos.X, i.Z - CarPos.Z);
	if (wheelAngle < 360) {
		wheelAngle += 0.1f * frametime;

	}
	
	Matrix wheelsMat;
	wheelsMat.translation(CarPos);

	Matrix wheelsRotMat;
	wheelsRotMat.rotationX(wheelAngle);
	frontWheels->transform(wheelsMat * wheelsRotMat);

	rearWheels->transform(wheelsMat * wheelsRotMat);
	
}

void Car::draw(const BaseCamera& Cam)
{
	chassis->draw(Cam);
	frontWheels->draw(Cam);
	rearWheels->draw(Cam);
}