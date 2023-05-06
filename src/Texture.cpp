#include "stdafx.h"
#include "Texture.h"

#include "Defines.h"


Texture::Texture(){

	_textureResource = nullptr;
	glGenTextures(1, &_textureId);
}

Texture::~Texture(){

	if (_textureResource)
		delete _textureResource;
	glDeleteTextures(1, &_textureId);
}

int Texture::Load(TextureResource* textureResource){

	bool res;
	unsigned char* imgBuffer = nullptr;
	int textureWidth, textureHeight;
	GLenum BPP = 0;

	_textureResource = textureResource;

	//load img
	res = loadTGA(_textureResource->FilePath().c_str(), &textureWidth, &textureHeight, &BPP, &imgBuffer);
	if (res == false)
		return MY_ERROR_CODE;
	//configure texture
	
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, BPP, textureWidth, textureHeight, 0, BPP, GL_UNSIGNED_BYTE, imgBuffer);

	free(imgBuffer);

	return MY_SUCCES_CODE;
}

void Texture::Bind(int slot){

	glBindTexture(GL_TEXTURE_2D, _textureId);
	glActiveTexture(GL_TEXTURE0 + slot);
}

void Texture::Unbind(){

	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::loadTGA(const char* fileName, int* width, int* height, GLenum* format, unsigned char** pixels) {
	FILE* file = fopen(fileName, "rb");
	if (!file) {
		printf("Error: could not open file: %s\n", fileName);
		return false;
	}

	// Read the header
	unsigned char header[18];
	if (fread(header, sizeof(unsigned char), 18, file) != 18) {
		printf("Error: could not read TGA header\n");
		fclose(file);
		return false;
	}

	// Check if it is a supported format
	if (header[2] != 2 && header[2] != 3) {
		printf("Error: unsupported TGA format\n");
		fclose(file);
		return false;
	}

	// Get image dimensions
	*width = header[12] + (header[13] << 8);
	*height = header[14] + (header[15] << 8);
	int bpp = header[16] / 8;

	// Allocate memory for image data
	int imageSize = *width * *height * bpp;
	*pixels = (unsigned char*)malloc(imageSize);
	if (!*pixels) {
		printf("Error: could not allocate memory for image data\n");
		fclose(file);
		return false;
	}

	// Read the image data
	if (fread(*pixels, sizeof(unsigned char), imageSize, file) != imageSize) {
		printf("Error: could not read image data\n");
		fclose(file);
		free(*pixels);
		return false;
	}

	fclose(file);

	// Set the format based on the number of bytes per pixel
	if (bpp == 3) {
		*format = GL_RGB;
	}
	else {
		*format = GL_RGBA;
	}

	// We want it in RGB format for openGL
	if ((header[17] & (1 << 4)) == 0) {
		// BGR format. Convert ro RGB
		for (int i = 0; i < imageSize; i += bpp) {
			unsigned char temp = (*pixels)[i];
			(*pixels)[i] = (*pixels)[i + 2];
			(*pixels)[i + 2] = temp;
		}
	}






	return true;
}
