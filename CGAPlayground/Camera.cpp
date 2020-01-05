#include "Camera.h"
#include <GLFW\glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/vector_angle.hpp>


Camera::Camera(glm::vec3 &position, glm::vec3 &lookDir, glm::vec3 &up, GLfloat aspectRatio) {
	this->aspectRatio = aspectRatio;
	this->zoom = 0.0f;
	this->projection = createProjectionMatrix();
	this->position = glm::vec3(position);
	this->lookDir = glm::vec3(lookDir);
	this->up = glm::vec3(up);
	this->view = glm::lookAt(position, position + lookDir, up);
	this->theta = this->phi = 0;
}

Camera::~Camera() {
}

glm::mat4 Camera::getProjection() {
	return projection;
}

glm::mat4 Camera::getView() {
	return view;
}

void Camera::update() {
	view = glm::lookAt(position + lookDir * zoom, position + lookDir, up);
	projection = createProjectionMatrix();
}

glm::mat4x4 Camera::createProjectionMatrix() {
	return glm::perspective(FOV, 16.0f/10.0f, NEAR, FAR);
}

void Camera::setLookDir(glm::vec3 &lookDir) {
	this->lookDir = glm::vec3(lookDir);
}

void Camera::rotateLookDir(GLfloat theta, GLfloat phi) {
	this->phi += phi * 0.05f;
	this->theta += theta * 0.05f;
	if (this->phi > 89.0f)
		this->phi = 89.0f;
	if (this->phi < -89.0f)
		this->phi = -89.0f;
	glm::vec3 front;
	front.x = cos(glm::radians(this->theta)) * cos(glm::radians(this->phi));
	front.y = sin(glm::radians(this->phi));
	front.z = sin(glm::radians(this->theta)) * cos(glm::radians(this->phi));
	lookDir = glm::normalize(front);
}

GLvoid Camera::translate(glm::vec3 translation) {
	position += translation;
}

GLvoid Camera::rotate(glm::vec3 axis, GLfloat rad) {
	lookDir = glm::rotate(lookDir, rad, axis);
}

GLfloat Camera::getZoom() {
	return zoom;
}

GLfloat Camera::getRotation() {
	return glm::angle(lookDir, glm::vec3(0, 0, 1));
}

GLfloat Camera::getRotationXZ() {
	glm::vec2 norm = glm::normalize(glm::vec2(lookDir.x, lookDir.z));
	GLfloat rot = glm::angle(glm::normalize( glm::vec2(0, 1)), norm);
	if (norm.x < 0)
		rot = 2 * 3.14f - rot;
	return rot;
}

GLvoid Camera::setZoom(GLfloat zoom) {
	this->zoom = zoom;
}

GLvoid Camera::setAspectRatio(GLfloat aspectRatio) {
	projection = createProjectionMatrix();
}

GLvoid Camera::setPosition(glm::vec3 & position) {
	this->position = position;
}

glm::vec3 Camera::getLookDir() {
	return lookDir;
}

glm::vec3 Camera::getPosition() {
	return position;
}

GLfloat Camera::getYaw() {
	return phi;
}

GLfloat Camera::getPitch() {
	return theta;
}
