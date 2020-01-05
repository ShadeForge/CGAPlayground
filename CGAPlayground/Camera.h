#pragma once
#include <GL\glew.h>
#include "glm.hpp"

class Camera {

private:
	const GLfloat FOV = 45.0f;
	const GLfloat NEAR = 0.1f;
	const GLfloat FAR = 10000.0f;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 position;
	glm::vec3 lookDir;
	glm::vec3 up;
	GLfloat zoom;
	GLfloat aspectRatio;
	GLfloat phi, theta;
	glm::mat4x4 createProjectionMatrix();
public:
	Camera() = delete;
	Camera(glm::vec3 &position, glm::vec3 &lookDir, glm::vec3 &up, GLfloat aspectRatio);
	~Camera();
	glm::mat4 getProjection();
	glm::mat4 getView();
	GLvoid update();
	GLvoid setLookDir(glm::vec3 &lookDir);
	GLvoid setZoom(GLfloat zoom);
	GLvoid rotateLookDir(GLfloat theta, GLfloat phi);
	GLvoid translate(glm::vec3 translation);
	GLvoid rotate(glm::vec3 axis, GLfloat rad);
	GLfloat getZoom();
	GLfloat getRotation();
	GLfloat getRotationXZ();
	GLvoid setAspectRatio(GLfloat aspectRatio);
	GLvoid setPosition(glm::vec3 &position);
	glm::vec3 getLookDir();
	glm::vec3 getPosition();
	GLfloat getYaw();
	GLfloat getPitch();
};

