#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "vector.h"
#include "matrix.h"

class BaseCamera
{
public:
	virtual void update() {}
	virtual const Matrix& getViewMatrix() const = 0;
	virtual const Matrix& getProjectionMatrix() const = 0;
	virtual Vector position() const { return Vector(0, 0, 0); }
	virtual ~BaseCamera() {};
};
