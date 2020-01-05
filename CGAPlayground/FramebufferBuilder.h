#pragma once
#include <GL/glew.h>
#include <vector>

class FramebufferBuilder {
public:
	FramebufferBuilder();
	~FramebufferBuilder() {};
	GLvoid addTexture(GLint texID, GLenum colorAttachment);
	GLvoid setDepthBuffering(GLboolean depthBuffering);
	GLvoid setDepthBufferSize(GLint width, GLint height);
	GLvoid setDepthFormat(GLenum format);
	GLuint create();
	GLvoid clear();
private:
	struct TextureAttachment {
		GLint texID;
		GLenum colorAttachment;
	};
	std::vector<TextureAttachment> attachments;
	GLboolean depthBuffering;
	GLint width, height;
	GLenum format;
};

