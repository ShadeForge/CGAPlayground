#include "AssetManager.h"



AssetManager::AssetManager() {
}

AssetManager::~AssetManager() {
}

GLvoid AssetManager::insertTexture(GLchar * file, GLuint texID) {
	textures.insert(std::pair<GLchar*, GLuint>(file, texID));
}

GLvoid AssetManager::insertModel(GLchar * file, Model * model) {
	models.insert(std::pair<GLchar*, Model*>(file, model));
}

GLvoid AssetManager::insertFont(GLchar * file) {
	Font * font;

	FT_Library ft;

	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	font = new Font(ft, file);

	FT_Done_FreeType(ft);

	fonts.insert(std::pair<GLchar*, Font>(file, *font));
}

GLuint AssetManager::getTexture(GLchar * file) {
	return textures[file];
}

Model* AssetManager::getModel(GLchar * file) {
	return models[file];
}

Font& AssetManager::getFont(GLchar * file) {
	return fonts[file];
}
