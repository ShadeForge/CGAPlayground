#pragma once
#define ERROR_CHECK(errCode) { GLenum err; while ((err = glGetError()) != GL_NO_ERROR) { std::cerr << "OpenGL error: " << errCode << std::endl; } }

#include <GL\glew.h>
#include <vector>
#include "InputListener.h"
#include "AssetManager.h"
#include <GLFW/glfw3.h>

class Game {
public:
	Game() = delete;
	Game(GLchar* title, GLint windowWidth, GLint windowHeight, GLint minorVersion, GLint majorVersion);
	virtual ~Game() {};
	GLvoid initializateWindow();
	GLvoid initializateInput();
	GLvoid loop();
	GLvoid exit();

	//Input
	GLboolean isKeyDown(GLuint key) const;
	GLboolean isKeyUp(GLuint key) const;
	GLboolean isKeyPressed(GLuint key) const;
	GLboolean isButtonDown(GLuint key) const;
	GLboolean isButtonUp(GLuint key) const;
	GLboolean isButtonPressed(GLuint key) const;
	GLuint getMouseX();
	GLuint getMouseY();
	GLboolean isScrolled() const;
	GLint getScrollAmount() const;
	GLvoid addInputListener(InputListener* listener);
	GLvoid removeInputListener(InputListener * listener);
	GLvoid setClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	GLint getWindowWidth();
	GLint getWindowHeight();

protected:
	AssetManager assetManager;

	virtual GLvoid update(GLfloat delta) = 0;
	virtual GLvoid render() = 0;
	virtual GLvoid resize(GLint width, GLint height) = 0;

	//OpenGL & Window
	GLint getFPS() const;
	GLfloat getDeltaTime() const;
	GLint getWindowWidth() const;
	GLint getWindowHeight() const;
	GLint getWindowAspect() const;
	GLfloat getRandom(GLfloat max);

private:
	//OpenGL & Window
	GLchar* title;
	GLint windowWidth;
	GLint windowHeight;
	GLfloat windowAspect;
	GLFWwindow* window;
	GLint minorVersion;
	GLint majorVersion;
	GLfloat clearRed;
	GLfloat clearGreen;
	GLfloat clearBlue;
	GLfloat clearAlpha;

	//Input
	GLuint kbState[GLFW_KEY_LAST + 1];
	GLboolean kbPressed[GLFW_KEY_LAST + 1];
	GLuint mState[GLFW_MOUSE_BUTTON_LAST + 1];
	GLboolean mPressed[GLFW_MOUSE_BUTTON_LAST + 1];
	GLuint mouseX;
	GLuint mouseY;
	GLint scrollAmount;
	GLboolean scrolled;
	std::vector<InputListener*> inputListeners;

	//Runtime
	GLfloat deltaTime;
	GLfloat fpstime;
	GLint frameCounter;
	GLint currentFPS;
	GLboolean isRunning;
	GLfloat lastUpdate;

	static Game* currentGame;
};

