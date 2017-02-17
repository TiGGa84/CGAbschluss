
#include "ShaderLightMapper.h"
#include "BaseShader.h"

#define _USE_MATH_DEFINES
#include <math.h>

ShaderLightMapper* ShaderLightMapper::pMapper = NULL;

float toRad(float deg)
{
	return deg * (float)M_PI / 180.0f;
}


ShaderLightMapper& ShaderLightMapper::instance()
{
	if (pMapper == NULL) pMapper = new ShaderLightMapper();
	return *pMapper;
}

ShaderLightMapper::ShaderLightMapper()
{
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderLightBlock), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
}

ShaderLightMapper::~ShaderLightMapper()
{
	clear();
}

void ShaderLightMapper::addLight(BaseLight* Light)
{
	if (Lights.size() >= MAXLIGHTCOUNT)
		return;

	Lights.push_back(Light);
}


void ShaderLightMapper::activate()
{
	ShaderLights.LightCount = (int)Lights.size();

	for (int i = 0; i < ShaderLights.LightCount; i++)
	{
		BaseLight* L = Lights[i];
		ShaderLight* SL = &ShaderLights.lights[i];

		SL->Color = L->color();
		SL->Attenuation = L->attenuation();
		SL->Position = L->position();
		SL->Type = L->type();

		switch (L->type())
		{
		case BaseLight::DIRECTIONAL:
		{
			DirectionalLight* dl = dynamic_cast<DirectionalLight*>(L);
			SL->Direction = dl->direction();
			SL->Direction.normalize();
			break;
		}
		case BaseLight::SPOT:
		{
			SpotLight* sl = dynamic_cast<SpotLight*>(L);
			SL->Direction = sl->direction();
			SL->SpotRadius = Vector(toRad(sl->innerRadius()), toRad(sl->outerRadius()), 0);
			SL->Direction.normalize();
			break;
		}
		case BaseLight::POINT:
			break;
		}
	}

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShaderLightBlock), &ShaderLights);
}

void ShaderLightMapper::clear()
{
	for (BaseLight* pLight : Lights)
		delete pLight;

	Lights.clear();
}