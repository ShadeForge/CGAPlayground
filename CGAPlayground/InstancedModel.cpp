#include "InstancedModel.h"
#include <gtc/type_ptr.hpp>


InstancedModel::InstancedModel(GLint maxElements) {
	this->maxElements = maxElements;
	data = new GLfloat[maxElements * 16];

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<GLuint> indices;

	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));

	uvs.push_back(glm::vec2(0.0f, 0.0f));
	uvs.push_back(glm::vec2(1.0f, 0.0f));
	uvs.push_back(glm::vec2(0.0f, 1.0f));
	uvs.push_back(glm::vec2(1.0f, 1.0f));

	uvs.push_back(glm::vec2(1.0f, 0.0f));
	uvs.push_back(glm::vec2(0.0f, 0.0f));
	uvs.push_back(glm::vec2(1.0f, 1.0f));
	uvs.push_back(glm::vec2(0.0f, 1.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(7);

	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(6);
	ModelGroup newGroup;
	GLuint vbo1, vbo2;

	glGenVertexArrays(1, &newGroup.vao);
	glGenBuffers(1, &newGroup.ebo);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &vbo2);
	glGenBuffers(1, &streamVbo);

	glBindVertexArray(newGroup.vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, maxElements * 4 * 16, &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (uvs.size() != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo2);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, streamVbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4, (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4, (void*)4);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4, (void*)8);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4, (void*)12);
	glEnableVertexAttribArray(6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newGroup.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertexCount = indices.size() * 3;
	indiciesCount = indices.size();
	modelGroups.insert(std::pair<GLchar*, ModelGroup>("default", newGroup));
}

InstancedModel::~InstancedModel() {
}

GLvoid InstancedModel::loadUpData(glm::mat4& data, GLint index) {
	this->data[index++] = data[0][0];
	this->data[index++] = data[0][1];
	this->data[index++] = data[0][2];
	this->data[index++] = data[0][3];

	this->data[index++] = data[1][0];
	this->data[index++] = data[1][1];
	this->data[index++] = data[1][2];
	this->data[index++] = data[1][3];

	this->data[index++] = data[2][0];
	this->data[index++] = data[2][1];
	this->data[index++] = data[2][2];
	this->data[index++] = data[2][3];

	this->data[index++] = data[3][0];
	this->data[index++] = data[3][1];
	this->data[index++] = data[3][2];
	this->data[index++] = data[3][3];
}

GLvoid InstancedModel::bindData(GLint elementCount) {
	glBindBuffer(GL_ARRAY_BUFFER, streamVbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * 4 * elementCount, data, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLvoid InstancedModel::render() {
	glBindVertexArray(getVAO());

	glDrawElementsInstanced(GL_TRIANGLES, indiciesCount, GL_UNSIGNED_INT, NULL, 1);

	glBindVertexArray(0);
}
