#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "vector.h"
#include "color.h"

#include <vector>

class BaseLight
{
public:
	enum LightType
	{
		POINT = 0,
		DIRECTIONAL,
		SPOT
	};

	BaseLight(const Vector& p = Vector(0, 0, 0), const Color& c = Color(1, 1, 1)) : Position(p), Col(c), Attenuation(1, 0, 0) {}
	virtual ~BaseLight() {}

	const Vector& position() const { return Position; }
	void position(const Vector& p) { Position = p; }

	const Vector& attenuation() const { return Attenuation; }
	void attenuation(const Vector& a) { Attenuation = a; }

	const Color& color() const { return Col; }
	void color(const Color& c) { Col = c; }

	virtual LightType type() const = 0;

protected:
	Vector Position;
	Vector Attenuation;
	Color Col;
};

class PointLight : public BaseLight
{
public:
	PointLight(const Vector& p = Vector(0, 0, 0), const Color& c = Color(1, 1, 1)) : BaseLight(p, c) {}
	virtual ~PointLight() {}

	virtual LightType type() const { return POINT; }
};

class DirectionalLight : public BaseLight
{
public:
	DirectionalLight(const Vector& d = Vector(-1, -1, -1), const Color& c = Color(1, 1, 1)) : BaseLight(Vector(0, 0, 0), c), Direction(d) {}
	virtual ~DirectionalLight() {}

	virtual LightType type() const { return DIRECTIONAL; }

	const Vector& direction() const { return Direction; }
	void direction(const Vector& d) { Direction = d; }

protected:
	Vector Direction;
};

class SpotLight : public BaseLight
{
public:
	SpotLight(const Vector& p = Vector(0, 0, 0), const Vector& d = Vector(-1, -1, -1), float InnerRadius = 30.0, float OuterRadius = 40.0f, const Color& c = ::Color(1, 1, 1)) : BaseLight(p, c), Direction(d), InnerRadius(InnerRadius), OuterRadius(OuterRadius) {}
	virtual ~SpotLight() {}

	virtual LightType type() const { return SPOT; }

	float innerRadius() const { return InnerRadius; }
	void innerRadius(float r) { InnerRadius = r; }

	float outerRadius() const { return OuterRadius; }
	void outerRadius(float r) { OuterRadius = r; }

	const Vector& direction() const { return Direction; }
	void direction(const Vector& d) { Direction = d; }

protected:
	float InnerRadius;
	float OuterRadius;
	Vector Direction;
};


#define MAXLIGHTCOUNT 5

class ShaderLightMapper
{
public:
	typedef std::vector<BaseLight*> LightList;

	void addLight(BaseLight* Light);
	void clear();

	void activate();
	static ShaderLightMapper& instance();
	GLuint uniformBlockID() { return UBO; }

protected:
	struct ShaderLight
	{
		int Type; Vector padding5;
		Color Color; float padding0;
		Vector Position; float padding1;
		Vector Direction; float padding2;
		Vector Attenuation; float padding3;
		Vector SpotRadius; float padding4;
	};

	struct ShaderLightBlock
	{
		ShaderLight lights[MAXLIGHTCOUNT];
		int LightCount;
	};
protected:
	ShaderLightMapper();
	ShaderLightMapper(const ShaderLightMapper& m) {}
	~ShaderLightMapper();
	LightList Lights;
	ShaderLightBlock ShaderLights;
	GLuint UBO;
	static ShaderLightMapper* pMapper;
};

