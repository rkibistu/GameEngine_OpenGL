#pragma once

#include "SceneManager.h"
#include "EffectQuad.h"
#include "Utilities/utilities.h"

class EffectManager {

public:

	void Init();
	void Destroy();
	void Draw(ESContext* esContext);
	void DrawBloom(ESContext* esContext);
private:
	GLuint _framebuffer;
	GLuint _depthRenderBuffer;
	GLuint _texture;
	GLint _texWidth = 920, _texHeight = 720;
	GLint _maxRenderbufferSize;

	SceneManager& _sceneManager = SceneManager::GetInstance();

	EffectQuad* _effectQuad;
};