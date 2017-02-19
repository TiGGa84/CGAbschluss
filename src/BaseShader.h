//
//  BaseShader.hpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef BaseShader_hpp
#define BaseShader_hpp

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "color.h"
#include "vector.h"
#include "matrix.h"
#include "camera.h"

#include <string>

class BaseShader
{
public:
	BaseShader();
	virtual ~BaseShader() {}
	virtual void modelTransform(const Matrix& m) { ModelTransform = m; }
	virtual const Matrix& modelTransform() const { return ModelTransform; }

	virtual void activate(const BaseCamera& Cam) const;
	virtual void deactivate() const;

	void load(std::string VertexShaderFile, std::string FragmentShaderFile);
	GLint getParameterID(const char* ParamenterName) const;
	GLuint getBlockID(const char* BlockName) const;

	void setBlock(GLuint ID, GLuint UniformBufferID) const;
	void setParameter(GLint ID, float Param) const;
	void setParameter(GLint ID, int Param) const;
	void setParameter(GLint ID, const Vector& Param) const;
	void setParameter(GLint ID, const Color& Param) const;
	void setParameter(GLint ID, const Matrix& Param) const;

	GLuint openGLProgramID() { return ShaderProgram; }
protected:
	GLuint createShaderProgram(const char* VScode, const char* FScode);
	Matrix ModelTransform;
	GLuint ShaderProgram;

	static const BaseShader* ShaderInPipe;
};

#endif /* BaseShader_hpp */
