
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include <iostream>
#include "Application.h"
#include "freeimage.h"

void PrintOpenGLVersion();


int main() {
	FreeImage_Initialise();
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		std::cerr << "ERROR: could not start GLFW3\n" << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const int WindowWidth = 800;
	const int WindowHeight = 600;

	GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Light Rider", NULL, NULL);
	if (!window) {
		std::cerr << "ERROR: can not open window with GLFW3\n" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	// vsync
	glfwSwapInterval(1);

	PrintOpenGLVersion();

	{
		double lasttime = glfwGetTime();
		Application App(window);
		App.start();
		while (!glfwWindowShouldClose(window)) {
			// once per frame
			glfwPollEvents();

			double time = glfwGetTime();
			double frametime = time - lasttime;
			lasttime = time;

			App.update(time, frametime);

			App.draw();
			App.drawHUD();

			glfwSwapBuffers(window);
		}
		App.end();
	}

	glfwTerminate();
	return 0;
}


void PrintOpenGLVersion()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
}

