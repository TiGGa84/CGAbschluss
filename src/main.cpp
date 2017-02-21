
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include <iostream>
#include "Application.h"
#include "freeimage.h"
#include "GamestateManager.h"
#include "MainMenu.h"
#include "SoundManager.h"

#define SKIP_MENU1

void PrintOpenGLVersion();

int main() {
	GamestateManager* gm = new GamestateManager();
	SoundManager* sm = new SoundManager();
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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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

#ifdef SKIP_MENU
	gm->setGameState(2);
#endif


	MainMenu Menu(window, gm);
	Application App(window, gm);

	double lasttime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		// once per frame
		glfwPollEvents();

		double time = glfwGetTime();
		double frametime = time - lasttime;
		lasttime = time;

		switch (gm->getGameState()) {
		case 0:
			//Menue laden
			Menu.start();
			sm->setBaseVolume(0.4f);
			sm->playMenuMusic();
			gm->setGameState(1);
			break;
		case 1:
			//Menue anzeigen und auf Eingabe warten
			Menu.update(time, frametime);
			Menu.draw();
			break;
		case 2:
			//Spiel starten
			glfwSetTime(0);
			App.start();
			sm->stopAllSounds();
			sm->playDrivingMusic();
			gm->setGameState(4);
			break;
		case 3:
			//Spiel neu starten
			glfwSetTime(0);
			App.start();
			gm->setGameState(4);
			break;
		case 4:
			//Spiel ablaufen lassen
			App.update(time, frametime);
			App.draw();
			App.drawHUD();
			break;
		case 5:
			//Nach Kollision Dialogfenster anzeigen und auf Eingabe warten
			App.update(time, frametime);
			App.draw();
			App.drawHUD();
			App.drawDialog();
			break;
		case 6:
			//Spielwelt entladen
			sm->stopAllSounds();
			gm->setGameState(0);
			break;
		default:
			break;
		}

		glfwSwapBuffers(window);
	}

	delete gm;
	delete sm;
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

