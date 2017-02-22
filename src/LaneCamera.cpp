
#include "LaneCamera.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define CAM_PAN_SPEED 15.0f
#define CAM_PAN_DIRECTION 0.7f

LaneCamera::LaneCamera(GLFWwindow* pWin) :
	m_Position(0.0f, 5.0f, 5.0f),
	m_Target(0.0f, 0.0f, 0.0f),
	m_Up(0.0f, 1.0f, 0.0f),
	m_Panning(0, 0, 0)
{
	int w = 0, h = 0;
	glfwGetWindowSize(pWin, &w, &h);

	m_ViewMatrix.identity();
	m_ProjMatrix.perspective((float)M_PI * 70.0f / 180.0f, (float)w / (float)h, 0.045f, 1000.0f);

	reset();
}

Vector LaneCamera::position() const
{
	return m_Position + m_Panning;
}
Vector LaneCamera::target() const
{
	return m_Target + m_Panning;
}
Vector LaneCamera::up() const
{
	return m_Up;
}

void LaneCamera::setPosition(const Vector& Pos)
{
	m_Position = Pos;
	m_Panning = Vector(0, 0, 0);


}
void LaneCamera::setTarget(const Vector& Target)
{
	m_Target = Target;
	m_Panning = Vector(0, 0, 0);
}
void LaneCamera::setUp(const Vector& Up)
{
	m_Up = Up;
}

void LaneCamera::pan(float dx, float dy)
{
	// calculate panning-plane
	Vector aDir = m_Target - m_Position;
	aDir.normalize();
	Vector aRight = aDir.cross(m_Up);
	aRight.normalize();
	Vector aUp = aDir.cross(aRight);
	m_Panning = aRight * dx + aUp * dy;
}

const Matrix& LaneCamera::getViewMatrix() const
{
	return m_ViewMatrix;
}

const Matrix& LaneCamera::getProjectionMatrix() const
{
	return m_ProjMatrix;
}

void LaneCamera::update(float frametime)
{
	Vector Pos = position();
	Vector Target = target();

	float change = 0.0f;
	float diff = abs(desiredLane - currentPos);
	change = diff / 10.0f * frametime * CAM_PAN_SPEED;

	if (desiredLane < currentPos) {
		currentPos -= change;
	}
	else if (desiredLane > currentPos) {
		currentPos += change;
	}

	pan(currentPos * CAM_PAN_DIRECTION, 0);

	m_ViewMatrix.lookAt(Target, m_Up, Pos);
}

void LaneCamera::setLane(int lane) {
	desiredLane = lane;
}
int LaneCamera::getLane() {
	return desiredLane;
}

void LaneCamera::switchLane(int direction) {
	if (desiredLane > -1 && direction < 0) desiredLane--;
	else if (desiredLane < 1 && direction > 0) desiredLane++;
}

void LaneCamera::reset() {
	currentPos = 0;
	desiredLane = 0;
}