#include "Game.h"
#include <iostream>
#include <ctime>
#include <GLFW/glfw3.h>

using namespace std;

Game* Game::currentGame;

Game::Game(GLchar* title, GLint windowWidth, GLint windowHeight, GLint minorVersion, GLint majorVersion) {
	this->title = title;
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;
	this->minorVersion = minorVersion;
	this->majorVersion = majorVersion;
	this->windowAspect = windowWidth / windowHeight;
	this->isRunning = false;
	this->lastUpdate = 0;
	this->deltaTime = 0;
	this->clearRed = 0;
	this->clearGreen = 0;
	this->clearBlue = 0;
	this->clearAlpha = 0;
	initializateWindow();
	initializateInput();
	srand((unsigned int)time(NULL));
	currentGame = this;
}

GLvoid Game::initializateWindow() {

	if (!glfwInit()) {
		std::cerr << "Can't initialize GLFW" << std::endl;
		return;
	}

	window = glfwCreateWindow(windowWidth, windowHeight, title, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (!window) {
		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwSetErrorCallback([](GLint error, const GLchar* description) {
		cout << "Errorcode: " << error << endl << "Description: " << description << endl;
	});

	glfwSetWindowSizeCallback(window, [](GLFWwindow * window, GLint width, GLint height) {
		currentGame->windowWidth = width;
		currentGame->windowHeight = height;
		currentGame->windowAspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
		glfwSetWindowSize(window, width, height);
		currentGame->resize(width, height);
	});
	glewExperimental = GL_TRUE;
	auto err = glewInit();
	if (err != GLEW_OK) {
		cout << "Error: " << glewGetErrorString(err) << endl;
	}

	glViewport(0, 0, windowWidth, windowHeight);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLvoid Game::initializateInput() {

	glfwSetKeyCallback(window, [](GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mods) {
		if (key != -1) {
			currentGame->kbState[key] = action;
			if (action == GLFW_RELEASE) {
				currentGame->kbPressed[key] = true;
				for each(InputListener* listener in currentGame->inputListeners) {
					listener->keyup(key);
				}
			} else {
				for each(InputListener* listener in currentGame->inputListeners) {
					listener->keydown(key);
				}
			}
		}
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, GLdouble x, GLdouble y) {
		currentGame->mouseX = x;
		currentGame->mouseY = y;
		for each(InputListener* listener in currentGame->inputListeners) {
			listener->mousemove(x, y);
		}
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, GLint key, GLint action, GLint mods) {
		if (key != -1) {
			currentGame->mState[key] = action;
			if (action == GLFW_RELEASE) {
				currentGame->mPressed[key] = true;
				for each(InputListener* listener in currentGame->inputListeners) {
					listener->mousedown(currentGame->mouseX, currentGame->mouseY, key);
				}
			} else {
				for each(InputListener* listener in currentGame->inputListeners) {
					listener->mouseup(currentGame->mouseX, currentGame->mouseY, key);
				}
			}
		}
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, GLdouble x, GLdouble y) {
		currentGame->scrollAmount = y;
		currentGame->scrolled = true;
		for each(InputListener* listener in currentGame->inputListeners) {
			listener->mousescroll(y);
		}
	});
}

GLvoid Game::loop() {
	
	isRunning = true;

	lastUpdate = glfwGetTime();

	while(isRunning && !glfwWindowShouldClose(window)) {
		glClearColor(clearRed, clearGreen, clearBlue, clearAlpha);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		deltaTime = glfwGetTime() - lastUpdate;
		lastUpdate = glfwGetTime();

		fpstime += deltaTime;
		frameCounter++;
		if (fpstime >= 1) {
			fpstime--;
			currentFPS = frameCounter;
			frameCounter = 0;
		}

		for (auto i = 0; i < GLFW_KEY_LAST + 1; i++) {
			kbPressed[i] = false;
		}

		for (auto i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; i++) {
			mPressed[i] = false;
		}
		scrollAmount = 0;
		scrolled = false;
		glfwPollEvents();

		update(deltaTime);
		render();
		glfwSwapBuffers(window);
		
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << err << std::endl;
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

GLvoid Game::exit() {
	isRunning = false;
}

GLvoid Game::resize(GLint width, GLint height) {
}

GLint Game::getFPS() const {
	return currentFPS;
}

GLfloat Game::getDeltaTime() const {
	return deltaTime;
}

GLint Game::getWindowWidth() const {
	return windowWidth;
}

GLint Game::getWindowHeight() const {
	return windowHeight;
}

GLint Game::getWindowAspect() const {
	return windowAspect;
}

GLfloat Game::getRandom(GLfloat max) {
	return ((GLfloat)rand() / (GLfloat)(RAND_MAX)) * max;
}

GLboolean Game::isKeyDown(GLuint key) const {
	return kbState[key] > 0;
}

GLboolean Game::isKeyUp(GLuint key) const {
	return kbState[key] == GLFW_RELEASE;
}

GLboolean Game::isKeyPressed(GLuint key) const {
	return kbPressed[key] == GLFW_PRESS;
}

GLboolean Game::isButtonDown(GLuint key) const {
	return mState[key] > 0;
}

GLboolean Game::isButtonUp(GLuint key) const {
	return mState[key] == GLFW_RELEASE;
}

GLboolean Game::isButtonPressed(GLuint key) const {
	return mPressed[key] == GLFW_PRESS;
}

GLuint Game::getMouseX() {
	return mouseX;
}


GLuint Game::getMouseY() {
	return mouseY;
}

GLboolean Game::isScrolled() const {
	return scrolled;
}

GLint Game::getScrollAmount() const {
	return scrollAmount;
}

GLvoid Game::addInputListener(InputListener * listener) {
	inputListeners.push_back(listener);
}

GLvoid Game::removeInputListener(InputListener * listener) {
	
	auto iter = currentGame->inputListeners.begin();

	while (iter != currentGame->inputListeners.end() && *iter != listener)
		iter++;

	if(iter != currentGame->inputListeners.end())
		inputListeners.erase(iter);
}

GLvoid Game::setClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	this->clearRed = red;
	this->clearGreen = green;
	this->clearBlue = blue;
	this->clearAlpha = alpha;
}

GLint Game::getWindowWidth() {
	return windowWidth;
}

GLint Game::getWindowHeight() {
	return windowHeight;
}
