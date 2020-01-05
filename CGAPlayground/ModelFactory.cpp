#include "ModelFactory.h"
#include "glm.hpp"




Model * ModelFactory::createTriangleModel() {
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;

	vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
	vertices.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
	vertices.push_back(glm::vec3(0.5f, 0.5f, 0.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	return new Model(vertices, indices);
}

Model * ModelFactory::createPlaneModel() {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<GLuint> indices;

	vertices.push_back(glm::vec3(-0.5f, 0.0f, 0.5f));
	vertices.push_back(glm::vec3(0.5f, 0.0f, 0.5f));
	vertices.push_back(glm::vec3(-0.5f, 0.0f, -0.5f));
	vertices.push_back(glm::vec3(0.5f, 0.0f, -0.5f));

	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	uvs.push_back(glm::vec2(0.0f, 0.0f));
	uvs.push_back(glm::vec2(1.0f, 0.0f));
	uvs.push_back(glm::vec2(0.0f, 1.0f));
	uvs.push_back(glm::vec2(1.0f, 1.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	return new Model(vertices, normals, uvs, indices);
}

Model * ModelFactory::createScreenPlaneModel() {
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;

	vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	return new Model(vertices, indices);
}

Model * ModelFactory::createCube() {
	return nullptr;
}

Model * ModelFactory::createSkybox() {
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;

	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

	for (GLuint i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	return new Model(vertices, indices);
}

Model * ModelFactory::createBox() {
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normals;

	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));

	for (GLuint i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	return new Model(vertices, normals, std::vector<glm::vec2>(), indices);
}

Model * ModelFactory::createWall() {
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normals;

	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));

	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));

	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));

	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));

	for (GLuint i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	return new Model(vertices, normals, std::vector<glm::vec2>(), indices);
}

Model * ModelFactory::createBillboardModel() {
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

	return new Model(vertices, uvs, indices);
}