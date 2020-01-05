#include "TextureBuilder.h"
#include <ostream>
#include <iostream>

TextureBuilder::TextureBuilder() {
	clear();
}

GLvoid TextureBuilder::setTextureType(GLenum type) {
	this->type = type;
}

GLvoid TextureBuilder::setSize(GLint width, GLint height) {
	this->width = width;
	this->height = height;
}

GLvoid TextureBuilder::setInternalFormat(GLenum internalFormat) {
	this->internalFormat = internalFormat;
}

GLvoid TextureBuilder::setDataFormat(GLenum dataFormat) {
	this->dataFormat = dataFormat;
}

GLvoid TextureBuilder::setDataType(GLenum dataType) {
	this->dataType = dataType;
}

GLvoid TextureBuilder::setData(GLubyte * data) {
	this->data = data;
}

GLvoid TextureBuilder::addParameter(GLenum param, GLenum value) {
	TextureParameter parameter;
	parameter.param = param;
	parameter.value = value;
	parameters.push_back(parameter);
}

GLvoid TextureBuilder::addParameterArray(GLenum param, const GLint * values) {
	TextureParameterArray parameterArray;
	parameterArray.param = param;
	parameterArray.values = values;
	parameterArrays.push_back(parameterArray);
}

GLvoid TextureBuilder::addParameterArray(GLenum param, const GLfloat * values) {
	TextureParameterArrayF parameterArray;
	parameterArray.param = param;
	parameterArray.values = values;
	parameterFArrays.push_back(parameterArray);
}

GLvoid TextureBuilder::setLayerCount(GLint count) {
	this->layerCount = count;
}

GLvoid TextureBuilder::setMipmapped(GLboolean mipmapped) {
	this->mipmapped = mipmapped;
}

GLvoid TextureBuilder::setCubeMapData(GLenum face, GLubyte * data, GLint width, GLint height) {
	CubeMapData cubeMapData;
	cubeMapData.face = face;
	cubeMapData.data = data;
	cubeMapData.width = width;
	cubeMapData.height = height;
	this->cubemapDatas.push_back(cubeMapData);
}

GLuint TextureBuilder::create() {
	GLuint* texID = new GLuint;
	
	glGenTextures(1, texID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(type, *texID);

	if(type == GL_TEXTURE_CUBE_MAP) {
		for each(CubeMapData data in cubemapDatas) {
			glTexImage2D(data.face, 0, internalFormat, data.width, data.height, 0, dataFormat, dataType, data.data);
		}
	} else if(type == GL_TEXTURE_2D_ARRAY || type == GL_TEXTURE_3D) {
		glTexImage3D(type, 0, internalFormat, width, height, layerCount, 0, dataFormat, dataType, data);
	} else {
		glTexImage2D(type, 0, internalFormat, width, height, 0, dataFormat, dataType, data);
	}
	if(mipmapped)
		glGenerateMipmap(type);

	for each(TextureParameter parameter in parameters) {
		glTexParameteri(type, parameter.param, parameter.value);
	}

	for each(TextureParameterArray parameter in parameterArrays) {
		glTexParameteriv(type, parameter.param, parameter.values);
	}

	for each(TextureParameterArrayF parameter in parameterFArrays) {
		glTexParameterfv(type, parameter.param, parameter.values);
	}

	glBindTexture(type, 0);
	clear();

	return *texID;
}

GLvoid TextureBuilder::clear() {
	this->type = 0;
	this->width = 0;
	this->height = 0;
	this->internalFormat = 0;
	this->dataFormat = 0;
	this->dataType = 0;
	this->data = nullptr;
	this->layerCount = 0;
	this->mipmapped = GL_FALSE;
	this->parameters.clear();
	this->parameterArrays.clear();
	this->cubemapDatas.clear();
}
