//
//  Camera.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 22.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>

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
	void zoom(float dz);
	void rotate(float x, float y);
	Vector getVSpherePos(float x, float y);
	Vector rotateAxisAngle(Vector v, Vector n, float a);

	int desiredLane;
	float currentPos;
	Matrix m_ViewMatrix;
	Matrix m_ProjMatrix;
	Vector m_Position;
	Vector m_Target;
	Vector m_Up;
	Vector m_Panning;
	Vector m_Zoom;
	Vector m_Rotation;
	int m_LastMouseX;
	int m_LastMouseY;
	int WindowWidth;
	int WindowHeight;
	GLFWwindow* pWindow;

private:

};
