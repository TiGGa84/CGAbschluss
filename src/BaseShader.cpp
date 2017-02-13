//
//  BaseShader.cpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "BaseShader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>

const BaseShader* BaseShader::ShaderInPipe = NULL;

BaseShader::BaseShader()
{
	ModelTransform.identity();
}

void BaseShader::load(const char* VertexShaderFile, const char* FragmentShaderFile)
{
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::stringstream vertexStream;
	std::stringstream fragmentStream;
	// throw exceptions
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	// Open files
	vertexFile.open(VertexShaderFile);
	fragmentFile.open(FragmentShaderFile);
	// Read file
	vertexStream << vertexFile.rdbuf();
	fragmentStream << fragmentFile.rdbuf();
	// close files
	vertexFile.close();
	fragmentFile.close();

	ShaderProgram = createShaderProgram(vertexStream.str().c_str(), fragmentStream.str().c_str());
}

GLuint BaseShader::createShaderProgram(const char* VScode, const char* FScode)
{
	const unsigned int LogSize = 64 * 1024;
	char ShaderLog[LogSize];
	GLsizei WrittenToLog = 0;
	GLint Success = 0;

	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);

	GLenum Error = glGetError();
	if (Error != 0)	{
		std::cout << "Unable to create shader objects";
		throw std::exception();
	}

	glShaderSource(VS, 1, &VScode, NULL);
	glShaderSource(FS, 1, &FScode, NULL);

	glCompileShader(VS);
	glGetShaderiv(VS, GL_COMPILE_STATUS, &Success);
	if (Success == GL_FALSE) {
		sprintf(ShaderLog, "VS:");
		WrittenToLog += 3;
		GLsizei Written = 0;
		glGetShaderInfoLog(VS, LogSize - WrittenToLog, &Written, &ShaderLog[WrittenToLog]);
		WrittenToLog += Written;
	}

	glCompileShader(FS);
	glGetShaderiv(FS, GL_COMPILE_STATUS, &Success);
	if (Success == GL_FALSE) {
		sprintf(&ShaderLog[WrittenToLog], "FS:");
		WrittenToLog += 3;
		GLsizei Written = 0;
		glGetShaderInfoLog(FS, LogSize - WrittenToLog, &Written, &ShaderLog[WrittenToLog]);
		WrittenToLog += Written;
	}

	if (WrittenToLog > 0) {
		// compilation failed
		std::cout << ShaderLog;
		throw std::exception();
	}

	ShaderProgram = glCreateProgram();
	if (ShaderProgram == 0)	{
		std::cout << "Unable to create shader programm";
		throw std::exception();
	}

	glAttachShader(ShaderProgram, VS);
	glDeleteShader(VS);
	glAttachShader(ShaderProgram, FS);
	glDeleteShader(FS);
	glLinkProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == GL_FALSE)
	{
		GLsizei Written = 0;
		glGetProgramInfoLog(ShaderProgram, LogSize - WrittenToLog, &Written, &ShaderLog[WrittenToLog]);
		WrittenToLog += Written;
	}

	if (WrittenToLog > 0)
	{
		// compilation failed
		std::cout << ShaderLog;
		throw std::exception();
	}
	return ShaderProgram;
}

void BaseShader::activate(const BaseCamera& Cam) const
{
	if (ShaderInPipe != this) {
		glUseProgram(ShaderProgram);
	}
	ShaderInPipe = this;
}


void BaseShader::deactivate() const
{
	glUseProgram(0);
}

GLuint BaseShader::getBlockID(const char* BlockName) const
{
	return glGetUniformBlockIndex(ShaderProgram, BlockName);
}

void BaseShader::setBlock(GLuint ID, GLuint UniformBufferID) const
{
	glBindBufferBase(GL_UNIFORM_BUFFER, ID, UniformBufferID);
}


GLint BaseShader::getParameterID(const char* ParamenterName) const
{
	return glGetUniformLocation(ShaderProgram, ParamenterName);
}

void BaseShader::setParameter(GLint ID, float Param) const
{
	glUniform1f(ID, Param);
}
void BaseShader::setParameter(GLint ID, int Param) const
{
	glUniform1i(ID, Param);
}
void BaseShader::setParameter(GLint ID, const Vector& Param) const
{
	glUniform3f(ID, Param.X, Param.Y, Param.Z);
}
void BaseShader::setParameter(GLint ID, const Color& Param) const
{
	glUniform3f(ID, Param.R, Param.G, Param.B);
}

void BaseShader::setParameter(GLint ID, const Matrix& Param) const
{
	glUniformMatrix4fv(ID, 1, GL_FALSE, Param.m);
}




