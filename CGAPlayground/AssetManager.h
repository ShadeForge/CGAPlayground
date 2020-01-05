#pragma once
#include "Font.h"
#include "Model.h"

class AssetManager {
private:
	std::map<std::string, GLuint> textures;
	std::map<std::string, Model*> models;
	std::map<std::string, Font> fonts;
public:
	AssetManager();
	~AssetManager();
	GLvoid insertTexture(GLchar* file, GLuint texID);
	GLvoid insertModel(GLchar* file, Model* model);
	GLvoid insertFont(GLchar* file);
	GLuint getTexture(GLchar* file);
	Model* getModel(GLchar* file);
	Font& getFont(GLchar* file);
};

