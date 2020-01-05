#include "ShaderStage.h"
#include <GLFW/glfw3.h>
#include "Game.h"

ShaderStage::ShaderStage(ShaderProgram * shaderProgram) {
	this->fbID = 0;
	this->shaderProgram = shaderProgram;
	this->rMask = this->gMask = this->bMask = this->aMask = this->depth = GL_TRUE;
	this->blend = GL_FALSE;
	this->blendSrc = GL_ZERO;
	this->blendDst = GL_ZERO;
	this->culling = GL_TRUE;
	this->cullMode = GL_BACK;
	this->startRender = 0;
	this->renderTime = 0;
	this->alphaTest = GL_FALSE;
	this->alphaFunc = 0;
	this->alphaFuncValue = 0;
	this->depthTest = GL_FALSE;
	this->renderTimeIndex = 0;
}

GLvoid ShaderStage::begin() {

	startRender = glfwGetTime();

	glColorMask(rMask, gMask, bMask, aMask);

	glDepthMask(depth);
	if (blend) {
		glEnable(GL_BLEND);
		glBlendFunc(blendSrc, blendDst);
	} else {
		glDisable(GL_BLEND);
	}
	if (culling) {
		glEnable(GL_CULL_FACE);
		glCullFace(cullMode);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if (alphaTest) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(alphaFunc, alphaFuncValue);
	} else {
		glDisable(GL_ALPHA_TEST);
	}
	if (depthTest) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}

	shaderProgram->begin();
	glBindFramebuffer(GL_FRAMEBUFFER, fbID);
}

GLvoid ShaderStage::end() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shaderProgram->end();
	GLfloat time = glfwGetTime() - startRender;
	renderTime += time / 100.0f;
	renderTime -= renderTimeBuffer[renderTimeIndex] / 100.0f;
	renderTimeBuffer[renderTimeIndex] = time;
	renderTimeIndex++;
	renderTimeIndex %= 100;
}

GLvoid ShaderStage::renderBufferAtViewPort(GLint srcX, GLint srcY, GLint srcWidth, GLint srcHeight, GLint dstX, GLint dstY, GLint dstWidth, GLint dstHeight, GLuint destinationBuffer) const {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationBuffer);

	glBlitFramebuffer(srcX, srcY, srcX + srcWidth, srcY + srcHeight, dstX, dstY, dstX + dstWidth, dstY + dstHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GLvoid ShaderStage::clearBuffer(GLenum clearBits) const {
	glBindFramebuffer(GL_FRAMEBUFFER, fbID);
	glClear(clearBits);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLvoid ShaderStage::setFrameBuffer(GLuint fbID) {
	this->fbID = fbID;
}

GLvoid ShaderStage::setColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
	this->rMask = r;
	this->gMask = g;
	this->bMask = b;
	this->aMask = a;
}

GLvoid ShaderStage::setDepthMask(GLboolean depth) {
	this->depth = depth;
}

GLvoid ShaderStage::setDepthTest(GLboolean depthTest) {
	this->depthTest = depthTest;
}

GLvoid ShaderStage::setBlending(GLboolean blend) {
	this->blend = blend;
}

GLvoid ShaderStage::setBlendFunc(GLenum src, GLenum dst) {
	this->blendSrc = src;
	this->blendDst = dst;
}

GLvoid ShaderStage::setAlphaTest(GLenum alphaTest) {
	this->alphaTest = alphaTest;
}

GLvoid ShaderStage::setAlphaFunc(GLenum func, GLfloat value) {
	this->alphaFunc = func;
	this->alphaFuncValue = value;
}

GLvoid ShaderStage::setCullFace(GLboolean culling) {
	this->culling = culling;
}

GLvoid ShaderStage::setCullFaceMode(GLenum mode) {
	this->cullMode = mode;
}

GLuint ShaderStage::getUniform(GLchar* name) const {
	return shaderProgram->getUniform(name);
}

GLdouble ShaderStage::getRenderTime() const {
	return renderTime;
}
