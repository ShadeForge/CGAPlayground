#include "FramebufferBuilder.h"
#include <iostream>


FramebufferBuilder::FramebufferBuilder() {
	clear();
}

GLvoid FramebufferBuilder::addTexture(GLint texID, GLenum colorAttachment) {
	TextureAttachment textureAttachment;
	textureAttachment.texID = texID;
	textureAttachment.colorAttachment = colorAttachment;
	attachments.push_back(textureAttachment);
}

GLvoid FramebufferBuilder::setDepthBuffering(GLboolean depthBuffering) {
	this->depthBuffering = depthBuffering;
}

GLvoid FramebufferBuilder::setDepthBufferSize(GLint width, GLint height) {
	this->width = width;
	this->height = height;
}

GLvoid FramebufferBuilder::setDepthFormat(GLenum format) {
	this->format = format;
}

GLuint FramebufferBuilder::create() {
	GLuint* fbID = new GLuint;
	glGenFramebuffers(1, fbID);

	glBindFramebuffer(GL_FRAMEBUFFER, *fbID);
	if (depthBuffering) {
		GLuint dbID;
		glGenRenderbuffers(1, &dbID);
		glBindRenderbuffer(GL_RENDERBUFFER, dbID);
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dbID);
	}
	for each(TextureAttachment attachment in attachments) {
		glFramebufferTexture(GL_FRAMEBUFFER, attachment.colorAttachment, attachment.texID, 0);
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is incompleted" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	clear();
	return *fbID;
}

GLvoid FramebufferBuilder::clear() {
	this->width = 0;
	this->height = 0;
	this->format = 0;
	this->depthBuffering = false;
	this->attachments.clear();
}
