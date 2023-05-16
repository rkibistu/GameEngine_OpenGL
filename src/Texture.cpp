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
	char* imgBuffer = nullptr;
	int textureWidth, textureHeight;
	int outBpp;
	GLenum BPP = 0;

	_textureResource = textureResource;

	res = loadTGA(_textureResource->FilePath().c_str(), &textureWidth, &textureHeight, &BPP, &imgBuffer);


	int bits = 3;

	if (_textureResource->Type == "cube") {

		int faceWidth = textureWidth / 4;
		int faceHeight = textureHeight / 3;

		char** subBuffers;
		subBuffers = new char* [12];
		for (int i = 0; i < 12; i++) {

			int startX = i / 4 * faceHeight;
			int startY = i % 4 * faceWidth;

			subBuffers[i] = new char[faceWidth * faceHeight * bits];

			int j = 0;
			std::cout << startX << " " << startY << "\n";
			for (int x = startX * bits; x < (startX + faceHeight) * bits; x=x+3) {
				for (int y = startY * bits; y < (startY + faceWidth) * bits; y=y+3) {

					subBuffers[i][j] = imgBuffer[x * textureWidth + y];
					subBuffers[i][j + 1] = imgBuffer[x * textureWidth + y + 1];
					subBuffers[i][j + 2] = imgBuffer[x * textureWidth + y + 2];
					j += 3;
				}
			}
		}

		GLenum error;
		while ((error = glGetError()) != GL_NO_ERROR) {}


		Bind(_textureId);

		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << error << std::endl;
		}


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GetParam(_textureResource->MinFilter));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GetParam(_textureResource->MagFilter));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GetParam(_textureResource->WrapS));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GetParam(_textureResource->WrapT));

		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << error << std::endl;
		}

		unsigned int x = sizeof(imgBuffer);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, BPP, faceWidth, faceHeight, 0, BPP, GL_UNSIGNED_BYTE, subBuffers[6]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, BPP, faceWidth, faceHeight, 0, BPP, GL_UNSIGNED_BYTE, subBuffers[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, BPP, faceWidth, faceHeight, 0, BPP, GL_UNSIGNED_BYTE, subBuffers[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, BPP, faceWidth, faceHeight, 0, BPP, GL_UNSIGNED_BYTE, subBuffers[9]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, BPP, faceWidth, faceHeight, 0, BPP, GL_UNSIGNED_BYTE, subBuffers[5]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, BPP, faceWidth, faceHeight, 0, BPP, GL_UNSIGNED_BYTE, subBuffers[7]);

		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << error << std::endl;
		}


		// DONT FORGET TO FREE THE BUFFERS
	}
	else {


		//glActiveTexture(_textureId);
		glBindTexture(GL_TEXTURE_2D, _textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetParam(_textureResource->MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetParam(_textureResource->MagFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetParam(_textureResource->WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetParam(_textureResource->WrapT));

		glTexImage2D(GL_TEXTURE_2D, 0, BPP, textureWidth, textureHeight, 0, BPP, GL_UNSIGNED_BYTE, imgBuffer);
	}

	free(imgBuffer);

	return MY_SUCCES_CODE;
}

void Texture::Bind(int slot) {

	glActiveTexture(GL_TEXTURE0 + slot);

	if (_textureResource->Type == "2d") {

		glBindTexture(GL_TEXTURE_2D, _textureId);
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);
	}

}

void Texture::Unbind() {

	glBindTexture(GL_TEXTURE_2D, 0);
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

GLfloat Texture::GetParam(std::string param) {

	if (param == "NEAREST")
		return GL_NEAREST;
	if (param == "LINEAR")
		return GL_LINEAR;
	if (param == "GL_REPEAT")
		return GL_REPEAT;
	if (param == "CLAMP_TO_EDGE")
		return GL_CLAMP_TO_EDGE;
}
