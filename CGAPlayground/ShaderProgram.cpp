#include "ShaderProgram.h"
#include <iostream>
#include <fstream>
#include <sstream>


ShaderProgram::ShaderProgram(GLchar * vertexShader, GLchar * fragmentShader, GLchar* geometryShader) {
	std::vector<GLchar*> vertexShaders;
	std::vector<GLchar*> fragmentShaders;
	std::vector<GLchar*> geometryShaders;;
	vertexShaders.push_back(vertexShader);
	fragmentShaders.push_back(fragmentShader);
	if (geometryShader != "")
		geometryShaders.push_back(geometryShader);
	initializate(vertexShaders, std::vector<std::string>(),
				 fragmentShaders, std::vector<std::string>(),
				 geometryShaders, std::vector<std::string>(),
				 std::vector<GLchar*>(), std::vector<std::string>(),
				 std::vector<GLchar*>(), std::vector<std::string>(),
				 std::vector<GLchar*>(), std::vector<std::string>());
}

ShaderProgram::ShaderProgram(std::vector<GLchar*> vertexShaderFiles, std::vector<GLchar*> fragmentShaderFiles, std::vector<GLchar*> geometryShaderFiles) {
	initializate(vertexShaderFiles, std::vector<std::string>(),
				 fragmentShaderFiles, std::vector<std::string>(),
				 geometryShaderFiles, std::vector<std::string>(),
				 std::vector<GLchar*>(), std::vector<std::string>(),
				 std::vector<GLchar*>(), std::vector<std::string>(),
				 std::vector<GLchar*>(), std::vector<std::string>());
}

ShaderProgram::ShaderProgram(std::vector<GLchar*> vertexShaderFiles, std::vector<std::string> vertexShaders,
						 std::vector<GLchar*> fragmentShaderFiles, std::vector<std::string> fragmentShaders,
						 std::vector<GLchar*> geometryShaderFiles, std::vector<std::string> geometryShaders,
						 std::vector<GLchar*> tessellationControlShaderFiles, std::vector<std::string> tessellationControlShaders,
						 std::vector<GLchar*> tessellationEvaluationShaderFiles, std::vector<std::string> tessellationEvaluationShaders,
						 std::vector<GLchar*> computeShaderFiles, std::vector<std::string> computeShaders) {
	initializate(vertexShaderFiles, vertexShaders,
				 fragmentShaderFiles, fragmentShaders,
				 geometryShaderFiles, geometryShaders,
				 tessellationControlShaderFiles, tessellationControlShaders,
				 tessellationEvaluationShaderFiles, tessellationEvaluationShaders,
				 computeShaderFiles, computeShaders);
}

GLvoid ShaderProgram::initializate(std::vector<GLchar*> vertexShaderFiles, std::vector<std::string> vertexShaders,
								 std::vector<GLchar*> fragmentShaderFiles, std::vector<std::string> fragmentShaders,
								 std::vector<GLchar*> geometryShaderFiles, std::vector<std::string> geometryShaders,
								 std::vector<GLchar*> tessellationControlShaderFiles, std::vector<std::string> tessellationControlShaders,
								 std::vector<GLchar*> tessellationEvaluationShaderFiles, std::vector<std::string> tessellationEvaluationShaders,
								 std::vector<GLchar*> computeShaderFiles, std::vector<std::string> computeShaders) {

	for each(GLchar* vertexShaderFile in vertexShaderFiles) {
		vertexShaders.push_back(readShaderFile(vertexShaderFile));
	}

	for each(GLchar* fragmentShaderFile in fragmentShaderFiles) {
		fragmentShaders.push_back(readShaderFile(fragmentShaderFile));
	}

	for each(GLchar* geometryShaderFile in geometryShaderFiles) {
		geometryShaders.push_back(readShaderFile(geometryShaderFile));
	}

	for each(GLchar* tesselationControlShaderFile in tessellationControlShaderFiles) {
		tessellationControlShaders.push_back(readShaderFile(tesselationControlShaderFile));
	}

	for each(GLchar* tesselationEvaluationShaderFile in tessellationEvaluationShaderFiles) {
		tessellationEvaluationShaders.push_back(readShaderFile(tesselationEvaluationShaderFile));
	}

	for each(GLchar* computeShaderFile in computeShaderFiles) {
		computeShaders.push_back(readShaderFile(computeShaderFile));
	}

	std::vector<GLint> compiledVS;
	std::vector<GLint> compiledFS;
	std::vector<GLint> compiledGS;
	std::vector<GLint> compiledTCS;
	std::vector<GLint> compiledTES;
	std::vector<GLint> compiledCS;

	programID = glCreateProgram();

	for each(std::string vertexShader in vertexShaders) {
		GLint vsID = createShader(vertexShader.c_str(), GL_VERTEX_SHADER);
		glAttachShader(programID, vsID);
		compiledVS.push_back(vsID);
	}

	for each(std::string fragmentShader in fragmentShaders) {
		GLint fsID = createShader(fragmentShader.c_str(), GL_FRAGMENT_SHADER);
		glAttachShader(programID, fsID);
		compiledVS.push_back(fsID);
	}

	for each(std::string geometryShader in geometryShaders) {
		GLint gsID = createShader(geometryShader.c_str(), GL_GEOMETRY_SHADER);
		glAttachShader(programID, gsID);
		compiledVS.push_back(gsID);
	}

	for each(std::string computeShader in computeShaders) {
		GLint csID = createShader(computeShader.c_str(), GL_COMPUTE_SHADER);
		glAttachShader(programID, csID);
		compiledVS.push_back(csID);
	}

	for each(std::string tessellationControlShader in tessellationControlShaders) {
		GLint tcsID = createShader(tessellationControlShader.c_str(), GL_TESS_CONTROL_SHADER);
		glAttachShader(programID, tcsID);
		compiledVS.push_back(tcsID);
	}

	for each(std::string tessellationEvaluationShader in tessellationEvaluationShaders) {
		GLint tesID = createShader(tessellationEvaluationShader.c_str(), GL_TESS_EVALUATION_SHADER);
		glAttachShader(programID, tesID);
		compiledVS.push_back(tesID);
	}

	glLinkProgram(programID);

	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	for each(GLint vsID in compiledVS) {
		glDeleteShader(vsID);
	}
	for each(GLint fsID in compiledFS) {
		glDeleteShader(fsID);
	}
	for each(GLint gsID in compiledGS) {
		glDeleteShader(gsID);
	}
	for each(GLint csID in compiledCS) {
		glDeleteShader(csID);
	}
	for each(GLint tcsID in compiledTCS) {
		glDeleteShader(tcsID);
	}
	for each(GLint tesID in compiledTES) {
		glDeleteShader(tesID);
	}
}

std::string ShaderProgram::readShaderFile(GLchar* path) {
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	shaderFile.exceptions(std::ifstream::badbit);
	try {
		shaderFile.open(path);

		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return shaderStream.str();
}

GLint ShaderProgram::createShader(const GLchar* shader, GLenum type) {
	GLint success;
	GLchar infoLog[512];
	GLint vsID = glCreateShader(type);

	glShaderSource(vsID, 1, &shader, NULL);
	glCompileShader(vsID);

	glGetShaderiv(vsID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vsID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::";
		switch (type) {
		case GL_GEOMETRY_SHADER:
			std::cout << "GEOMETRY";
			break;
		case GL_FRAGMENT_SHADER:
			std::cout << "FRAGMENT";
			break;
		case GL_VERTEX_SHADER:
			std::cout << "VERTEX";
			break;
		case GL_TESS_CONTROL_SHADER:
			std::cout << "TESSELLATION_CONTROL";
			break;
		case GL_TESS_EVALUATION_SHADER:
			std::cout << "TESSELLATION_EVALUATION";
			break;
		case GL_COMPUTE_SHADER:
			std::cout << "COMPUTE";
			break;
		}
		std::cout << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return vsID;
}

GLvoid ShaderProgram::begin() {
	glUseProgram(programID);
}

GLvoid ShaderProgram::end() {
	glUseProgram(0);
}

GLvoid ShaderProgram::addUniform(GLchar * name) {
	GLuint location = glGetUniformLocation(programID, name);
	uniforms.insert(std::pair<std::string, GLuint>(name, location));
}

GLvoid ShaderProgram::addAttribute(GLchar * name, GLint id) const {
	glBindAttribLocation(programID, id, name);
}

GLvoid ShaderProgram::addFragDataLocation(GLchar * name, GLenum color) const {
	glBindFragDataLocation(programID, color, name);
}

GLuint ShaderProgram::getUniform(GLchar* name) {
	return uniforms[name];
}
