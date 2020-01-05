#pragma once
#include <GL/glew.h>
#include <string>
#include <glm.hpp>
#include <map>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <iostream>

class Font {
private:
	struct Character {
		GLuint TextureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		GLuint Advance;
	};

	GLuint VAO, VBO;
	std::map<GLchar, Character> Characters;
public:
	Font();
	Font(FT_Library &ft, GLchar* file);
	~Font();
	void renderText(std::string text, GLint x, GLint y, GLfloat scale);
};

