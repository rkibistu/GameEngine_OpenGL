#pragma once
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

#include "Utilities/utilities.h"

class EffectQuad {

public:
	EffectQuad(GLuint textureId);
	~EffectQuad();

	void Draw();

private:
	void Init();

private:
	//specific to quad
	Shader* _quadShader;
	Model* _quadModel;
	Texture* _quadTexture;

	GLuint _texture;
};