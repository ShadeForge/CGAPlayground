#pragma once
#include <glm.hpp>
#include <GL/glew.h>

class Lightcamera {
private:
	glm::mat4 view;
	glm::mat4 projection;
public:
	Lightcamera();
	Lightcamera(glm::vec3 &position, glm::vec3 &lookDir, glm::vec3 &up, GLfloat aspectRatio, GLfloat fov, GLfloat near, GLfloat far);
	~Lightcamera();
	glm::mat4 getView();
	glm::mat4 getProjection();
};

