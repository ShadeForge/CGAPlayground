#pragma once
#include "Model.h"
class InstancedModel :
	public Model {
private:
	GLuint streamVbo;
	GLuint maxElements;
	GLfloat* data;
public:
	InstancedModel() {}
	InstancedModel(GLint maxElements);
	~InstancedModel();
	GLvoid loadUpData(glm::mat4& data, GLint index);
	GLvoid bindData(GLint elementCount);
	GLvoid render();
};

