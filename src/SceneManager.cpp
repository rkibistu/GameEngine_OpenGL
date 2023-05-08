#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Input.h"

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
	_xmlParser.ReadCameras(_cameras, &_activeCamera);
	_xmlParser.ReadControls();
}

void SceneManager::Update(float deltaTime) {

	_activeCamera->Update(deltaTime);
}

void SceneManager::Draw() {

	for (auto it = _sceneObjects.begin(); it != _sceneObjects.end(); it++) {

		it->second->Draw(_activeCamera);
	}
}