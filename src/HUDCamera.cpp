#include "HUDCamera.h"

HUDCamera::HUDCamera(GLFWwindow* pWindow)
{
	glfwGetWindowSize(pWindow, &WindowWidth, &WindowHeight);

	ViewMatrix.identity();
	ProjMatrix.orthographic((float)WindowWidth / (float)WindowHeight, 1, -1, 1);
}

const Matrix& HUDCamera::getViewMatrix() const
{
	return ViewMatrix;
}

const Matrix& HUDCamera::getProjectionMatrix() const
{
	return ProjMatrix;
}