#pragma once
#include "ShaderStage.h"

class ShaderStageBuilder {
private:
	struct VAttribute {
		GLchar* name;
		GLuint id;
	};
	struct FragDataLocation {
		GLchar* name;
		GLenum color;
	};
	std::vector<GLchar*> vertexShaderFiles;
	std::vector<std::string> vertexShaders;
	std::vector<GLchar*> fragmentShaderFiles;
	std::vector<std::string> fragmentShaders;
	std::vector<GLchar*> tessellationControlShaderFiles;
	std::vector<std::string> tessellationControlShaders;
	std::vector<GLchar*> tessellationEvaluationShaderFiles;
	std::vector<std::string> tessellationEvaluationShaders;
	std::vector<GLchar*> geometryShaderFiles;
	std::vector<std::string> geometryShaders;
	std::vector<GLchar*> computeShaderFiles;
	std::vector<std::string> computeShaders;
	std::vector<GLchar*> uniforms;
	std::vector<VAttribute> attributes;
	std::vector<FragDataLocation> fragDataLocations;
public:
	ShaderStageBuilder();
	~ShaderStageBuilder();
	GLvoid addFragmentShaderFile(GLchar* path);
	GLvoid addFragmentShader(GLchar* code);
	GLvoid addVertexShaderFile(GLchar* path);
	GLvoid addVertexShader(GLchar* code);
	GLvoid addGeometryShaderFile(GLchar* path);
	GLvoid addGeometryShader(GLchar* code);
	GLvoid addTessellationControlShaderFile(GLchar* path);
	GLvoid addTessellationControlShader(GLchar* code);
	GLvoid addTessellationEvaluationShaderFile(GLchar* path);
	GLvoid addTessellationEvaluationShader(GLchar* code);
	GLvoid addComputeShaderFile(GLchar* path);
	GLvoid addComputeShader(GLchar* code);
	GLvoid addUniform(GLchar* name);
	GLvoid addAttribute(GLchar* name, GLint id);
	GLvoid addFragDataLocation(GLchar* name, GLenum color);
	ShaderStage* create();
	GLvoid reset();
};

