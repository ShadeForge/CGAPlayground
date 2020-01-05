#include "Sun.h"



Sun::Sun() {
	this->view = glm::mat4();
	this->projection = glm::mat4();
	this->direction = glm::vec3();
}

Sun::~Sun() {
}

glm::mat4 Sun::getView() {
	return view;
}

glm::mat4 Sun::getProjection() {
	return projection;
}

glm::vec3 Sun::getPosition() {
	return direction;
}

GLvoid Sun::setProjection(glm::mat4& projection) {
	this->projection = projection;;
}

GLvoid Sun::setView(glm::mat4& view) {
	this->view = view;
}

GLvoid Sun::setPosition(glm::vec3 & direction) {
	this->direction = direction;
}
