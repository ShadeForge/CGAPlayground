#pragma once
#include <glm.hpp>
#include "Model.h"

class Entity {
protected:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scaling;
	Model* model;
	glm::mat4 modelMatrix;
	GLuint VAO, VBO, EBO;
	GLfloat livingTime;

	void updateMatrix();
public:
	Entity();
	Entity(Model* model);
	virtual ~Entity();
	virtual void render();
	virtual void update(float delta);
	glm::mat4& getModelMatrix();
	void setTranslation(glm::vec3 &translation);
	void setRotation(glm::vec3 &rotation);
	void setScale(glm::vec3 &scaling);
	glm::vec3 getTranslation();
	glm::vec3 getRotation();
	glm::vec3 getScaling();
	GLfloat getLivingTime();
};

