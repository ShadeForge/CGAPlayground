#include "Lightcamera.h"
#include <gtc/matrix_transform.inl>


Lightcamera::Lightcamera() {
}

Lightcamera::Lightcamera(glm::vec3 & position, glm::vec3 & lookDir, glm::vec3 & up, GLfloat aspectRatio, GLfloat fov, GLfloat near, GLfloat far) {
	view = glm::lookAt(position, position + lookDir, up);
	projection = glm::perspective(fov, aspectRatio, near, far);
}


Lightcamera::~Lightcamera() {
}

glm::mat4 Lightcamera::getView() {
	return glm::mat4();
}

glm::mat4 Lightcamera::getProjection() {
	return glm::mat4();
}
