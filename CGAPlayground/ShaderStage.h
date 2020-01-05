#pragma once
#include <GL/glew.h>
#include "ShaderProgram.h"

class ShaderStage {
private:
	ShaderProgram* shaderProgram;
	GLuint fbID;
	GLboolean rMask, gMask, bMask, aMask;
	GLboolean depth;
	GLboolean depthTest;
	GLboolean blend;
	GLenum blendSrc, blendDst;
	GLboolean alphaTest;
	GLenum alphaFunc;
	GLboolean culling;
	GLenum cullMode;
	GLdouble startRender;
	GLdouble renderTime;
	GLfloat alphaFuncValue;
	GLfloat renderTimeBuffer[100];
	GLint renderTimeIndex;
public:
	ShaderStage() = delete;
	ShaderStage(ShaderProgram* shaderProgram);
	virtual ~ShaderStage() {};
	virtual GLvoid begin();
	virtual GLvoid end();
	GLvoid renderBufferAtViewPort(GLint srcX, GLint srcY, GLint srcWidth, GLint srcHeight, GLint dstX, GLint dstY, GLint dstWidth, GLint dstHeight, GLuint destinationBuffer) const;
	GLvoid clearBuffer(GLenum clearBits) const;
	GLvoid setFrameBuffer(GLuint fbID);
	GLvoid setColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a);
	GLvoid setDepthMask(GLboolean depth);
	GLvoid setDepthTest(GLboolean depthTest);
	GLvoid setBlending(GLboolean blend);
	GLvoid setBlendFunc(GLenum src, GLenum dst);
	GLvoid setAlphaTest(GLenum alphaTest);
	GLvoid setAlphaFunc(GLenum func, GLfloat value);
	GLvoid setCullFace(GLboolean culling);
	GLvoid setCullFaceMode(GLenum mode);
	GLuint getUniform(GLchar* name) const;
	GLdouble getRenderTime() const;
};

