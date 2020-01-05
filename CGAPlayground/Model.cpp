#include "Model.h"
#include <iostream>

Model::Model() {
}

Model::Model(GLchar * path) {
	
	std::vector< GLuint > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return;
	}
	while (1) {

		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		} else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		} else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		} else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			GLuint vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	std::vector< glm::vec3 > out_vertices;
	std::vector< glm::vec3 > out_normals;
	std::vector< glm::vec2 > out_uvs;
	std::vector< GLuint > out_indices;

	for (GLuint i = 0; i < vertexIndices.size(); i++)
	{
		out_vertices.push_back(temp_vertices[vertexIndices[i] - 1]);
	}

	for (GLuint i = 0; i < normalIndices.size(); i++) {
		out_normals.push_back(temp_normals[normalIndices[i] - 1]);
	}

	if (temp_uvs.size() != 0) {
		for (GLuint i = 0; i < uvIndices.size(); i++) {
			out_uvs.push_back(temp_uvs[uvIndices[i] - 1]);
		}
	}
	
	for (int i = 0; i < vertexIndices.size(); i++)
	{
		out_indices.push_back(i);
	}
	createVAO(out_vertices, out_normals, out_uvs, out_indices);
}

Model::Model(std::vector<glm::vec3> out_vertices, std::vector<GLuint> out_indices) {
	createVAO(out_vertices, out_indices);
}

Model::Model(std::vector<glm::vec3> out_vertices, std::vector<glm::vec2> out_uvs, std::vector<GLuint> out_indices) {
	createVAO(out_vertices, out_uvs, out_indices);
}

Model::Model(std::vector<glm::vec3> out_vertices, std::vector<glm::vec3> out_normals, std::vector<glm::vec2> out_uvs, std::vector<GLuint> out_indices) {
	createVAO(out_vertices, out_normals, out_uvs, out_indices);
}


Model::~Model() {
	for (auto group = modelGroups.begin(); group != modelGroups.end(); ++group) {
		GLuint vao = group->second.vao;
		GLuint ebo = group->second.ebo;
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &ebo);

		for each(GLuint vbo in group->second.vbos) {
			glDeleteBuffers(1, &vbo);
		}
	}
}

void Model::createVAO(std::vector<glm::vec3> out_vertices, std::vector<GLuint> out_indices) {
	ModelGroup newGroup;
	GLuint vbo;
	glGenVertexArrays(1, &newGroup.vao);
	glGenBuffers(1, &newGroup.ebo);
	glGenBuffers(1, &vbo);

	glBindVertexArray(newGroup.vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newGroup.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_indices.size() * sizeof(GLuint), &out_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertexCount = out_indices.size() * 3;
	indiciesCount = vertexCount;
	modelGroups.insert(std::pair<GLchar*, ModelGroup>("default", newGroup));
}

void Model::createVAO(std::vector<glm::vec3> out_vertices, std::vector<glm::vec2> out_uvs, std::vector<GLuint> out_indices) {
	ModelGroup newGroup;
	GLuint vbo1, vbo2;
	glGenVertexArrays(1, &newGroup.vao);
	glGenBuffers(1, &newGroup.ebo);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &vbo2);

	glBindVertexArray(newGroup.vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (out_uvs.size() != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo2);
		glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(glm::vec2), &out_uvs[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newGroup.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_indices.size() * sizeof(GLuint), &out_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertexCount = out_indices.size() * 3;
	indiciesCount = vertexCount;
	modelGroups.insert(std::pair<GLchar*, ModelGroup>("default", newGroup));
}

void Model::createVAO(std::vector<glm::vec3> out_vertices, std::vector<glm::vec3> out_normals, std::vector<glm::vec2> out_uvs, std::vector<GLuint> out_indices) {
	ModelGroup newGroup;
	GLuint vbo1, vbo2, vbo3;
	glGenVertexArrays(1, &newGroup.vao);
	glGenBuffers(1, &newGroup.ebo);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &vbo2);

	if (out_uvs.size() != 0) {
		glGenBuffers(1, &vbo3);
	}

	glBindVertexArray(newGroup.vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(glm::vec3), &out_normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (out_uvs.size() != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo3);
		glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(glm::vec2), &out_uvs[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newGroup.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_indices.size() * sizeof(GLuint), &out_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertexCount = out_indices.size() * 3;
	indiciesCount = vertexCount;
	modelGroups.insert(std::pair<GLchar*, ModelGroup>("default", newGroup));
}

GLuint Model::getVAO() {
	return modelGroups.begin()->second.vao;
}

GLuint Model::getEBO()
{
	return modelGroups.begin()->second.ebo;
}

GLuint Model::getVAO(GLchar * key) {
	return modelGroups.at(key).vao;
}

GLuint Model::getVertexCount() {
	return vertexCount;
}

GLuint Model::getIndiciesCount() {
	return indiciesCount;
}
