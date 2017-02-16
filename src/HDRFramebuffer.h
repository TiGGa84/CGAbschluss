
#ifndef HDRFramebuffer_hpp
#define HDRFramebuffer_hpp

#include <GL/glew.h>
#include <glfw/glfw3.h>

class HDRFramebuffer
{
public:
	HDRFramebuffer();
	virtual ~HDRFramebuffer();

	void createBuffer(unsigned int width, unsigned int height);

	void activate() const;
	void deactivate() const;

	GLuint getOutputColorTexID();
	GLuint getOutputBloomTexID();
private:
	GLuint HDRFramebufferID;
	GLuint ColorbufferID[2];
	GLuint DepthbufferID;
};

#endif /* HDRFramebuffer_hpp */