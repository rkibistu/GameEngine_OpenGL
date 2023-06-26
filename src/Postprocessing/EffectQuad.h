#pragma once
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

#include "Utilities/utilities.h"

class EffectQuad {

public:
	EffectQuad();
	~EffectQuad();

	void Draw();

	inline void SetTexture(Texture* texture) { _quadTexture = texture; }
	inline void SetShader(Shader* shader) { _quadShader = shader; }
	

private:
	void Init();

	void SetUniforms();

private:
	//specific to quad
	Shader* _quadShader;
	Model* _quadModel;
	Texture* _quadTexture;

	GLuint _texture;
};