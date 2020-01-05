#pragma once
#include <glm.hpp>
#include <GL/glew.h>

class Sun {
private:
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 direction;
public:
	Sun();
	~Sun();
	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::vec3 getPosition();
	GLvoid setProjection(glm::mat4& projection);
	GLvoid setView(glm::mat4& view);
	GLvoid setPosition(glm::vec3& direction);
};

