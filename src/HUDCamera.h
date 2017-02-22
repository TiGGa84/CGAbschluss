#pragma once

#include "BaseCamera.h"

// Orthografische Camera um den HUD zu rendern
class HUDCamera : public BaseCamera
{
public:
	HUDCamera(GLFWwindow* pWindow);
	virtual ~HUDCamera() {};

	virtual const Matrix& getViewMatrix() const;
	virtual const Matrix& getProjectionMatrix() const;
protected:
	Matrix ViewMatrix;
	Matrix ProjMatrix;
	int WindowWidth;
	int WindowHeight;
};

