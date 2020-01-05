#include "Entity.h"
#include <gtc/matrix_transform.hpp> 
#include <gtx/rotate_vector.hpp> 
#include <GLFW/glfw3.h>


Entity::Entity() {
}

Entity::Entity(Model* model) {
	this->model = model;
	this->livingTime = glfwGetTime();
	this->translation = glm::vec3(0, 0, 0);
	this->rotation = glm::vec3(0, 0, 0);
	this->scaling = glm::vec3(1, 1, 1);
	updateMatrix();
}

Entity::~Entity() {
}

void Entity::render() {
	GLuint vao = model->getVAO();
	GLuint count = model->getIndiciesCount();

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

void Entity::update(float delta) {
}

glm::mat4 &Entity::getModelMatrix() {
	return modelMatrix;
}

void Entity::updateMatrix() {
	modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scaling);
}

void Entity::setTranslation(glm::vec3 & translation) {
	this->translation = translation;
	updateMatrix();
}

void Entity::setRotation(glm::vec3 & rotation) {
	this->rotation = rotation;
	updateMatrix();
}

void Entity::setScale(glm::vec3 & scaling) {
	this->scaling = scaling;
	updateMatrix();
}

glm::vec3 Entity::getTranslation() {
	return translation;
}

glm::vec3 Entity::getRotation() {
	return rotation;
}

glm::vec3 Entity::getScaling() {
	return scaling;
}

GLfloat Entity::getLivingTime() {
	return livingTime;
}
