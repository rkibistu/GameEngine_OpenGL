#pragma once

#include <string>

#include "Utilities/utilities.h"

struct TextureResource {
	
	unsigned int ID;
	std::string Path;
	std::string Filename;

	std::string FilePath() { return Path + Filename; }

	std::string Type;

	std::string MinFilter;
	std::string MagFilter;
	std::string WrapS;
	std::string WrapT;
};

class Texture{

public:
	Texture();
	~Texture();

	int Load(TextureResource* textureResource);

	void Bind(int slot);
	void Unbind();

private:
	bool loadTGA(const char* fileName, int* width, int* height, GLenum* format, char** pixels);
	
	

	GLfloat GetParam(std::string param);
private:
	TextureResource* _textureResource;
	GLuint _textureId;

};

