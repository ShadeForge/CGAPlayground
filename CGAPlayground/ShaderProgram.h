#pragma once
#include <GL/glew.h>
#include <vector>
#include <map>

class ShaderProgram {
public:
	ShaderProgram() = delete;
	ShaderProgram(GLchar* vertexShader, GLchar* fragmentShader, GLchar* geometryShader = "");
	ShaderProgram(std::vector<GLchar*> vertexShaderFiles, std::vector<GLchar*> fragmentShaderFiles, std::vector<GLchar*> geometryShaderFiles = std::vector<GLchar*>());
	ShaderProgram(std::vector<GLchar*> vertexShaderFiles, std::vector<std::string> vertexShaders, std::vector<GLchar*> fragmentShaderFiles, std::vector<std::string> fragmentShaders, std::vector<GLchar*> tessellationControlShaderFiles, std::vector<std::string> tessellationControlShaders, std::vector<GLchar*> tessellationEvaluationShaderFiles, std::vector<std::string> tessellationEvaluationShaders, std::vector<GLchar*> geometryShaderFiles, std::vector<std::string> geometryShaders, std::vector<GLchar*> computeShaderFiles, std::vector<std::string> computeShaders);
	virtual GLvoid begin();
	virtual GLvoid end();
	GLvoid addUniform(GLchar* name);
	GLvoid addAttribute(GLchar* name, GLint id) const;
	GLvoid addFragDataLocation(GLchar* name, GLenum color) const;
	GLuint getUniform(GLchar* name);
protected:
	GLint programID;
private:
	std::map<std::string, GLuint> uniforms;
	static std::string readShaderFile(GLchar * path);
	static GLint createShader(const GLchar * shader, GLenum type);
	GLvoid initializate(std::vector<GLchar*> vertexShaderFiles, std::vector<std::string> vertexShaders, std::vector<GLchar*> fragmentShaderFiles, std::vector<std::string> fragmentShaders, std::vector<GLchar*> tessellationControlShaderFiles, std::vector<std::string> tessellationControlShaders, std::vector<GLchar*> tessellationEvaluationShaderFiles, std::vector<std::string> tessellationEvaluationShaders, std::vector<GLchar*> geometryShaderFiles, std::vector<std::string> geometryShaders, std::vector<GLchar*> computeShaderFiles, std::vector<std::string> computeShaders);
};

