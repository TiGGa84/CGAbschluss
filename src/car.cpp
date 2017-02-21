#include "Car.h"
#include "Application.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define STEERSPEED 6.0f
#define INV_WHEELRADIUS (1.0f / (0.35f))

Car::Car(float speed) :
	desiredLane(0),
	speedPerS(speed),
	wheelAngle(0),
	currentPos(0) {}

Car::~Car()
{
	delete chassis;
	delete frontWheels;
	delete rearWheels;
}

bool Car::loadModels(std::string ChassisFile, std::string FrontWheelFile, std::string RearWheelFile)
{
	chassis = new Model(ChassisFile);
	frontWheels = new Model(FrontWheelFile);
	rearWheels = new Model(RearWheelFile);

	chassis->shader(this->shader());
	frontWheels->shader(this->shader());
	rearWheels->shader(this->shader());
	return true;
}

void Car::steer(int steer)
{
	if (desiredLane > -1 && steer < 0) desiredLane--;
	else if (desiredLane < 1 && steer > 0) desiredLane++;
}

void Car::update(float frametime, Application& app)
{
	// Lenk-Bewegung pro Zeit
	float change = STEERSPEED * frametime;
	float diff = abs(desiredLane - currentPos);

	// An desiredLane angleichen, ohne diff zu überschreiten
	if (desiredLane < currentPos) {
		currentPos -= std::min(change, diff);
	}
	else if (desiredLane > currentPos) {
		currentPos += std::min(change, diff);
	}

	// Winkel nach Geschwindigkeit berechnen
	wheelAngle += (speedPerS * frametime) * INV_WHEELRADIUS * (float)M_PI;
	wheelAngle = fmod(wheelAngle, (float)M_PI * 2.0f);

	Matrix steerMat;
	steerMat.translation(currentPos, 0, 0);

	Matrix frontOffset;
	frontOffset.translation(0, 0.12474f, -0.57477f);

	Matrix rearOffset;
	rearOffset.translation(0, 0.14376f, 0.50472f);

	Matrix wheelsRotMat;
	wheelsRotMat.rotationX(-wheelAngle);

	// Bewegungen und Car-transform anwenden
	chassis->transform(transform() * steerMat);
	frontWheels->transform(transform() * steerMat * frontOffset * wheelsRotMat);
	rearWheels->transform(transform() * steerMat * rearOffset * wheelsRotMat);
}

void Car::draw(const BaseCamera& Cam)
{
	chassis->draw(Cam);
	frontWheels->draw(Cam);
	rearWheels->draw(Cam);
}