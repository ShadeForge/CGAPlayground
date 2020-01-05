#pragma once

#include <GLFW/glfw3.h>

class InputListener
{
public:
	InputListener() = delete;
	virtual ~InputListener() = 0;
	virtual GLvoid keydown(GLint key) = 0;
	virtual GLvoid keyup(GLint key) = 0;
	virtual GLvoid mousemove(GLint x, GLint y) = 0;
	virtual GLvoid mousedown(GLint x, GLint y, GLint button) = 0;
	virtual GLvoid mouseup(GLint x, GLint y, GLint button) = 0;
	virtual GLvoid mousescroll(GLint amount) = 0;
};
