#include "stdafx.h"
#include "Texture.h"

#include "Defines.h"


Texture::Texture() {

	_textureResource = nullptr;
	glGenTextures(1, &_textureId);
}

Texture::~Texture() {

	glDeleteTextures(1, &_textureId);

	//Don't delete texture resources, it is going to be deletele by ResourceManager
}

int Texture::Load(TextureResource* textureResource) {

	bool res;

	_textureResource = textureResource;

	res = loadTGA(_textureResource->FilePath().c_str(), &_width, &_height, &_glBpp, &_imgBuffer);
	if (res != MY_SUCCES_CODE) {

		std::cout << "[ERROR]: Loading tga texture: " << _textureResource->FilePath().c_str() << std::endl;
		return res;
	}

	if (_textureResource->Type == TEXTURE_TYPE_CUBE_MAP) {
		LoadTextureCube();
	}
	else if (_textureResource->Type == TEXTURE_TYPE_2D) {
		LoadTexture2D();
	}
	else {
		std::cout << "[ERROR]: Load texture, uknow type: " << _textureResource->Type << " \n";
		return MY_ERROR_CODE;
	}

	free(_imgBuffer);

	return MY_SUCCES_CODE;
}

void Texture::Bind(int slot) {

	glActiveTexture(GL_TEXTURE0 + slot);

	if (_textureResource->Type == TEXTURE_TYPE_2D) {

		glBindTexture(GL_TEXTURE_2D, _textureId);
	}
	else if (_textureResource->Type == TEXTURE_TYPE_CUBE_MAP) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);
	}
	else {

		std::cout << "[ERROR]: Can't bind texture, unknow type: " << _textureResource->Type << " \n";
	}
}

void Texture::Unbind() {

	if (_textureResource->Type == TEXTURE_TYPE_CUBE_MAP) {

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	else if (_textureResource->Type == TEXTURE_TYPE_2D) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

bool Texture::loadTGA(const char* fileName, int* width, int* height, GLenum* format, char** pixels) {

	int outBpp;
	*pixels = LoadTGA(fileName, width, height, &outBpp);
	if (*pixels == nullptr)
		return MY_ERROR_CODE;

	if (outBpp / 8 == 3) {
		*format = GL_RGB;
	}
	else if (outBpp / 8 == 4) {
		*format = GL_RGBA;
	}
	else {

		std::cout << "[ERROR] Uknow bpp value in texture!" << std::endl;
		return MY_ERROR_CODE;
	}
	return MY_SUCCES_CODE;
}
//bool Texture::loadTGA(const char* fileName, int* _width, int* height, GLenum* format, unsigned char** pixels) {
//	FILE* file = fopen(fileName, "rb");
//	if (!file) {
//		printf("Error: could not open file: %s\n", fileName);
//		return false;
//	}
//
//	// Read the header
//	unsigned char header[18];
//	if (fread(header, sizeof(unsigned char), 18, file) != 18) {
//		printf("Error: could not read TGA header\n");
//		fclose(file);
//		return false;
//	}
//
//	// Check if it is a supported format
//	if (header[2] != 2 && header[2] != 3) {
//		printf("Error: unsupported TGA format\n");
//		fclose(file);
//		return false;
//	}
//
//	// Get image dimensions
//	*_width = header[12] + (header[13] << 8);
//	*height = header[14] + (header[15] << 8);
//	int bpp = header[16] / 8;
//
//	// Allocate memory for image data
//	int imageSize = *_width * *height * bpp;
//	*pixels = (unsigned char*)malloc(imageSize);
//	if (!*pixels) {
//		printf("Error: could not allocate memory for image data\n");
//		fclose(file);
//		return false;
//	}
//
//	// Read the image data
//	if (fread(*pixels, sizeof(unsigned char), imageSize, file) != imageSize) {
//		printf("Error: could not read image data\n");
//		fclose(file);
//		free(*pixels);
//		return false;
//	}
//
//	fclose(file);
//
//	// Set the format based on the number of bytes per pixel
//	if (bpp == 3) {
//		*format = GL_RGB;
//	}
//	else {
//		*format = GL_RGBA;
//	}
//
//	// We want it in RGB format for openGL
//	if ((header[17] & (1 << 4)) == 0) {
//		// BGR format. Convert ro RGB
//		for (int i = 0; i < imageSize; i += bpp) {
//			unsigned char temp = (*pixels)[i];
//			(*pixels)[i] = (*pixels)[i + 2];
//			(*pixels)[i + 2] = temp;
//		}
//	}
//
//	return true;
//}


int Texture::LoadTextureCube() {

	//this is hardcoded for skybox. maybe moved to xml ?
	int cellWidth = 4;
	int cellHeight = 3;

	int faceWidth = _width / cellWidth;
	int faceHeight = _height / cellHeight;
	char** subBuffers = SplitTexture(cellWidth, cellHeight);

	Bind(_textureId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GetParam(_textureResource->MinFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GetParam(_textureResource->MagFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GetParam(_textureResource->WrapS));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GetParam(_textureResource->WrapT));

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, _glBpp, faceWidth, faceHeight, 0, _glBpp, GL_UNSIGNED_BYTE, subBuffers[6]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, _glBpp, faceWidth, faceHeight, 0, _glBpp, GL_UNSIGNED_BYTE, subBuffers[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, _glBpp, faceWidth, faceHeight, 0, _glBpp, GL_UNSIGNED_BYTE, subBuffers[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, _glBpp, faceWidth, faceHeight, 0, _glBpp, GL_UNSIGNED_BYTE, subBuffers[9]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, _glBpp, faceWidth, faceHeight, 0, _glBpp, GL_UNSIGNED_BYTE, subBuffers[5]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, _glBpp, faceWidth, faceHeight, 0, _glBpp, GL_UNSIGNED_BYTE, subBuffers[7]);

	if (subBuffers != nullptr) {

		for (int i = 0; i < cellWidth * cellHeight; i++) {
			delete subBuffers[i];
		}
		delete subBuffers;
	}

	return MY_SUCCES_CODE;
}
int Texture::LoadTexture2D() {

	//glActiveTexture(_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetParam(_textureResource->MinFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetParam(_textureResource->MagFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetParam(_textureResource->WrapS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetParam(_textureResource->WrapT));

	glTexImage2D(GL_TEXTURE_2D, 0, _glBpp, _width, _height, 0, _glBpp, GL_UNSIGNED_BYTE, _imgBuffer);


	return MY_SUCCES_CODE;
}

char** Texture::SplitTexture(int countWidth, int countHeightt) {

	int faceWidth = _width / countWidth;
	int faceHeight = _height / countHeightt;

	int bitsRGB;
	if (_glBpp == GL_RGB) {
		bitsRGB = 3;
	}
	else if (_glBpp = GL_RGBA) {
		bitsRGB = 4;
	}
	else {

		std::cout << "[ERROR] Unknow BPP value: " << _glBpp << std::endl;
		return nullptr;
	}

	char** subBuffers;
	subBuffers = new char* [countWidth * countHeightt];
	for (int i = 0; i < countWidth * countHeightt; i++) {

		int startX = i / countWidth * faceHeight;
		int startY = i % countWidth * faceWidth;

		subBuffers[i] = new char[faceWidth * faceHeight * bitsRGB];

		int j = 0;
		//std::cout << startX << " " << startY << "\n";
		for (int x = startX * bitsRGB; x < (startX + faceHeight) * bitsRGB; x = x + bitsRGB) {
			for (int y = startY * bitsRGB; y < (startY + faceWidth) * bitsRGB; y = y + bitsRGB) {

				subBuffers[i][j] = _imgBuffer[x * _width + y];
				subBuffers[i][j + 1] = _imgBuffer[x * _width + y + 1];
				subBuffers[i][j + 2] = _imgBuffer[x * _width + y + 2];

				if (bitsRGB == 4) {

					subBuffers[i][j + 3] = _imgBuffer[x * _width + y + 3];
					j++;
				}

				j += 3;
			}
		}
	}
	return subBuffers;
}


GLint Texture::GetParam(std::string param) {

	if (param == "NEAREST")
		return GL_NEAREST;
	if (param == "LINEAR")
		return GL_LINEAR;
	if (param == "GL_REPEAT")
		return GL_REPEAT;
	if (param == "CLAMP_TO_EDGE")
		return GL_CLAMP_TO_EDGE;
}
