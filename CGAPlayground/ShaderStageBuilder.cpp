#include "ShaderStageBuilder.h"
#include "ShaderProgram.h"


ShaderStageBuilder::ShaderStageBuilder() {
}


ShaderStageBuilder::~ShaderStageBuilder() {
}

GLvoid ShaderStageBuilder::addFragmentShaderFile(GLchar * path) {
	fragmentShaderFiles.push_back(path);
}

GLvoid ShaderStageBuilder::addFragmentShader(GLchar * code) {
	fragmentShaders.push_back(code);
}

GLvoid ShaderStageBuilder::addVertexShaderFile(GLchar * path) {
	vertexShaderFiles.push_back(path);
}

GLvoid ShaderStageBuilder::addVertexShader(GLchar * code) {
	vertexShaders.push_back(code);
}

GLvoid ShaderStageBuilder::addGeometryShaderFile(GLchar * path) {
	geometryShaderFiles.push_back(path);
}

GLvoid ShaderStageBuilder::addGeometryShader(GLchar * code) {
	geometryShaders.push_back(code);
}

GLvoid ShaderStageBuilder::addTessellationControlShaderFile(GLchar * path) {
	tessellationControlShaderFiles.push_back(path);
}

GLvoid ShaderStageBuilder::addTessellationControlShader(GLchar * code) {
	tessellationControlShaders.push_back(code);
}

GLvoid ShaderStageBuilder::addTessellationEvaluationShaderFile(GLchar * path) {
	tessellationEvaluationShaderFiles.push_back(path);
}

GLvoid ShaderStageBuilder::addTessellationEvaluationShader(GLchar * code) {
	tessellationEvaluationShaders.push_back(code);
}

GLvoid ShaderStageBuilder::addComputeShaderFile(GLchar * path) {
	computeShaderFiles.push_back(path);
}

GLvoid ShaderStageBuilder::addComputeShader(GLchar * code) {
	computeShaders.push_back(code);
}

GLvoid ShaderStageBuilder::addUniform(GLchar * name) {
	uniforms.push_back(name);
}

GLvoid ShaderStageBuilder::addAttribute(GLchar * name, GLint id) {
	VAttribute attribute;
	attribute.name = name;
	attribute.id = id;
	attributes.push_back(attribute);
}

GLvoid ShaderStageBuilder::addFragDataLocation(GLchar * name, GLenum color) {
	FragDataLocation location;
	location.color = color;
	location.name = name;
	fragDataLocations.push_back(location);
}

ShaderStage * ShaderStageBuilder::create() {
	ShaderProgram* shaderProgram = new ShaderProgram(vertexShaderFiles, vertexShaders,
													 fragmentShaderFiles, fragmentShaders,
													 geometryShaderFiles, geometryShaders,
													 tessellationControlShaderFiles, tessellationControlShaders,
													 tessellationEvaluationShaderFiles, tessellationEvaluationShaders,
													 computeShaderFiles, computeShaders);
	for each(VAttribute attribute in attributes) {
		shaderProgram->addAttribute(attribute.name, attribute.id);
	}
	for each(FragDataLocation location in fragDataLocations){
		shaderProgram->addFragDataLocation(location.name, location.color);
	}
	for each(GLchar* uniform in uniforms) {
		shaderProgram->addUniform(uniform);
	}
	ShaderStage* shaderStage = new ShaderStage(shaderProgram);
	reset();
	return shaderStage;
}

GLvoid ShaderStageBuilder::reset() {
	vertexShaderFiles.clear();
	vertexShaders.clear();
	fragmentShaderFiles.clear();
	fragmentShaders.clear();
	geometryShaderFiles.clear();
	geometryShaders.clear();
	computeShaderFiles.clear();
	computeShaders.clear();
	tessellationControlShaderFiles.clear();
	tessellationControlShaders.clear();
	tessellationEvaluationShaderFiles.clear();
	tessellationEvaluationShaders.clear();
	attributes.clear();
	uniforms.clear();
	fragDataLocations.clear();
}
