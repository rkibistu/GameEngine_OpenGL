#pragma once

#include <string>

#include "Utilities/utilities.h"

struct TextureResource {

	std::string Path;
};

class Texture{

public:
	Texture();
	Texture(std::string path);
	~Texture();

	void Load();

	void Bind(int slot);
	void Unbind();

private:
	bool loadTGA(const char* fileName, int* width, int* height, GLenum* format, unsigned char** pixels);
private:
	TextureResource* _textureResource;
	GLuint _textureId;
};

