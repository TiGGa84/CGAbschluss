#pragma once

#include "BaseCamera.h"
#include "vector.h"
#include "Matrix.h"

class LaneCamera : public BaseCamera
{
public:
	LaneCamera(GLFWwindow* pWin);
	virtual ~LaneCamera() {};

	virtual Vector position() const;
	Vector target() const;
	Vector up() const;

	void setPosition(const Vector& Pos);
	void setTarget(const Vector& Target);
	void setUp(const Vector& Up);

	virtual void update(float frametime);
	virtual const Matrix& getViewMatrix() const;
	virtual const Matrix& getProjectionMatrix() const;

	void setLane(int lane);
	int getLane();
	void switchLane(int direction);
	void reset();

protected:

	void pan(float dx, float dy);

	int desiredLane;
	float currentPos;
	Matrix m_ViewMatrix;
	Matrix m_ProjMatrix;
	Vector m_Position;
	Vector m_Target;
	Vector m_Up;
	Vector m_Panning;;

private:

};
