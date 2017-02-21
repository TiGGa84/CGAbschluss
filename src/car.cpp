#include "Car.h"
#include "Application.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define STEERSPEED 6.0f
#define INV_WHEELRADIUS (1.0f / (0.35f))

Car::Car(float speed) :
	speedPerS(speed),
	Box(-0.4f, 0.0f, -0.9f, 0.4f, 1.0f, 0.7f)
{
	reset();
	frontOffset.translation(0, 0.12474f, -0.57477f);
	rearOffset.translation(0, 0.14376f, 0.50472f);

	frontLOffset = Vector(-0.23f, 0.28f, -0.95f);
	frontROffset = Vector(0.23f, 0.28f, -0.95f);
	backLOffset = Vector(-0.135f, 0.34f, 0.9f);
	backROffset = Vector(0.135f, 0.34f, 0.9f);

	Color frontColor(3.0f, 3.0f, 2.95f);
	Vector frontDir(0, -0.1f, -1.0f);
	float frontInner = 15.0f;
	float frontOuter = 30.0f;

	Color backColor(0.1f, 0.0f, 0.0f);
	Vector backDir(0, -0.4f, 1.0f);
	float backInner = 40.0f;
	float backOuter = 60.0f;

	frontLLight = new SpotLight(frontLOffset, frontDir, frontInner, frontOuter, frontColor);
	frontRLight = new SpotLight(frontROffset, frontDir, frontInner, frontOuter, frontColor);
	backLLight = new SpotLight(backLOffset, backDir, backInner, backOuter, backColor);
	backRLight = new SpotLight(backROffset, backDir, backInner, backOuter, backColor);

	ShaderLightMapper::instance().addLight(frontLLight);
	ShaderLightMapper::instance().addLight(frontRLight);
	ShaderLightMapper::instance().addLight(backLLight);
	ShaderLightMapper::instance().addLight(backRLight);
}

Car::~Car()
{
	delete chassis;
	delete frontWheels;
	delete rearWheels;
}

void Car::reset()
{
	desiredLane = 0;
	wheelAngle = 0;
	currentPos = 0;
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

	Matrix wheelsRotMat;
	wheelsRotMat.rotationX(-wheelAngle);

	// Bewegungen und Car-transform anwenden
	chassis->transform(transform() * steerMat);
	frontWheels->transform(transform() * steerMat * frontOffset * wheelsRotMat);
	rearWheels->transform(transform() * steerMat * rearOffset * wheelsRotMat);

	frontLLight->position(steerMat * frontLOffset);
	frontRLight->position(steerMat * frontROffset);
	backLLight->position(steerMat * backROffset);
	backRLight->position(steerMat * backLOffset);
}

void Car::draw(const BaseCamera& Cam)
{
	chassis->draw(Cam);
	frontWheels->draw(Cam);
	rearWheels->draw(Cam);
}

const AABB& Car::boundingBox() const
{
	return Box * chassis->transform();
}
