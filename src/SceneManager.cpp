#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"

SceneManager* SceneManager::_spInstance = nullptr;

SceneManager& SceneManager::GetInstance() {

	if (_spInstance == nullptr)
		_spInstance = new SceneManager();
	return *_spInstance;
}
void SceneManager::DestroyInstance() {

	if (_spInstance)
		delete _spInstance;
}

void SceneManager::Init() {

	ResourceManager& resourceManager = ResourceManager::GetInstance();

	_models.reserve(resourceManager.Models().size());
	for (int i = 0; i < resourceManager.Models().size(); i++) {

		Model* tempModel = new Model();
		//tempModel->Load()
	}

	_shaders.reserve(resourceManager.Shaders().size());
	_textures.reserve(resourceManager.Textures().size());

	
}