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


	_xmlParser.Init("Resources/XMLs/sceneManager.xml");
	_xmlParser.ReadObjects(_sceneObjects);
}
void SceneManager::Draw(Camera* camera) {

	for (auto it = _sceneObjects.begin(); it != _sceneObjects.end(); it++) {

		it->second->Draw(camera);
	}
}