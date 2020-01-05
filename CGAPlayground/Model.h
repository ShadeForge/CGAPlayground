#pragma once
#include <vector>
#include <map>
#include <GL/glew.h>
#include "glm.hpp"

class Model {
public:
	struct ModelGroup {
		GLuint vao;
		GLuint ebo;
		std::vector<GLuint> vbos;		
	};

	Model();
	Model(GLchar* file);
	Model(std::vector<glm::vec3> out_vertices, std::vector<GLuint> out_indices);
	Model(std::vector<glm::vec3> out_vertices, std::vector<glm::vec2> out_uvs, std::vector<GLuint> out_indices);
	Model(std::vector<glm::vec3> out_vertices, std::vector<glm::vec3> out_normals, std::vector<glm::vec2> out_uvs, std::vector<GLuint> out_indices);
	~Model();
	void createVAO(std::vector<glm::vec3> out_vertices, std::vector<GLuint> out_indices);
	void createVAO(std::vector<glm::vec3> out_vertices, std::vector<glm::vec2> out_uvs, std::vector<GLuint> out_indices);
	void createVAO(std::vector<glm::vec3> out_vertices, std::vector<glm::vec3> out_normals, std::vector<glm::vec2> out_uvs, std::vector<GLuint> out_indices);
	GLuint getVAO();
	GLuint getEBO();
	GLuint getVAO(GLchar* key);
	GLuint getVertexCount();
	GLuint getIndiciesCount();
protected:
	GLuint vertexCount;
	GLuint indiciesCount;
	std::map<GLchar*, ModelGroup> modelGroups;
};

