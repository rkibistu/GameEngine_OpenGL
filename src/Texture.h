#pragma once

#include <string>

#include "Utilities/utilities.h"

#define TEXTURE_TYPE_2D			"2d"
#define TEXTURE_TYPE_CUBE_MAP	"cube"

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

	inline void SetTextureId(GLuint id) { _textureId = id; }
	inline void SetTextureResource(TextureResource* resource) { _textureResource = resource; }
	inline TextureResource* GetTextureResource() { return _textureResource; }

private:
	bool loadTGA(const char* fileName, int* width, int* height, GLenum* format, char** pixels);
	
	int LoadTextureCube();
	int LoadTexture2D();

	// Split the texture laoded in _imgBuffer into countWidth*countHeight subbufers, each is a amtrix
	//	Imagine that you split the texture like a chess board with countWidth columns and coundHeight rows
	char** SplitTexture(int countWidth, int countHeight);

	GLint GetParam(std::string param);
private:
	TextureResource* _textureResource;
	GLuint _textureId;

	char* _imgBuffer;
	int _width;
	int _height;
	GLenum _glBpp;
};

