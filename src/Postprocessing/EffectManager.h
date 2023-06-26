#pragma once

#include "SceneManager.h"
#include "EffectQuad.h"

#include <unordered_map>
#include <vector>

#include "Utilities/utilities.h"

class EffectManager {

public:

	void Init();
	void Destroy();
	void Draw(ESContext* esContext);

private:
	Texture* CreateTextureObject(GLuint id);

private:
	GLuint _framebuffer[2];
	GLuint _depthRenderBuffer[2];
	GLuint _texture[2];
	std::vector<Texture*> _textures;
	GLint _texWidth = 920, _texHeight = 720;
	GLint _maxRenderbufferSize;

	SceneManager& _sceneManager = SceneManager::GetInstance();

	EffectQuad* _effectQuad;
};