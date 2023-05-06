#pragma once
#include "stdafx.h"

#include "Model.h"
#include "Texture.h"
#include "Shaders.h"


#include <vector>

class SceneManager {

public:

	static SceneManager& GetInstance();
	void DestroyInstance();

	void Init();

private:

private:
	static SceneManager* _spInstance;
	SceneManager() { ; }
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator= (const SceneManager&) = delete;

	std::vector<Model*> _modelResources;
	std::vector<Shader*> _shaderResources;
	std::vector<Texture*> _textureResources;
};