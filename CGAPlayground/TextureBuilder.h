#pragma once
#include <GL/glew.h>
#include <vector>

class TextureBuilder {
public:
	struct TextureParameter {
		GLenum param;
		GLenum value;
	};
	struct TextureParameterArray {
		GLenum param;
		const GLint* values;
	};
	struct TextureParameterArrayF {
		GLenum param;
		const GLfloat* values;
	};
	struct CubeMapData {
		GLenum face;
		GLubyte* data;
		GLint width;
		GLint height;
	};
	TextureBuilder();
	~TextureBuilder() {};
	GLvoid setTextureType(GLenum type);
	GLvoid setSize(GLint width, GLint height);
	GLvoid setInternalFormat(GLenum internalFormat);
	GLvoid setDataFormat(GLenum dataFormat);
	GLvoid setDataType(GLenum dataType);
	GLvoid setData(GLubyte* data);
	GLvoid addParameter(GLenum param, GLenum value);
	GLvoid addParameterArray(GLenum param, const GLint* values);
	GLvoid addParameterArray(GLenum param, const GLfloat * values);
	GLvoid setLayerCount(GLint count);
	GLvoid setMipmapped(GLboolean mipmapped);
	GLvoid setCubeMapData(GLenum face, GLubyte* data, GLint width, GLint height);
	GLuint create();
	GLvoid clear();
private:
	GLenum type;
	GLint width, height;
	GLenum internalFormat;
	GLenum dataFormat;
	GLenum dataType;
	GLubyte* data;
	std::vector<TextureParameter> parameters;
	std::vector<TextureParameterArray> parameterArrays;
	std::vector<TextureParameterArrayF> parameterFArrays;
	GLint layerCount;
	GLboolean mipmapped;
	std::vector<CubeMapData> cubemapDatas;
};

