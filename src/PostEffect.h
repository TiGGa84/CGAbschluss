
#ifndef PostEffect_hpp
#define PostEffect_hpp

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "BaseShader.h"

#include <vector>

// Abstrakte Klasse für Postprocessing Effekte
class PostEffect
{
public:
	PostEffect();
	virtual ~PostEffect();

	virtual void process() = 0;
protected:
	virtual void drawQuad() const;

	static GLuint QuadVAOID;
	static GLuint QuadVBOID;

	BaseShader Shader;
};

#endif /* PostEffect_hpp */