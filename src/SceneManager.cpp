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
}

void SceneManager::Update() {

	Vector3 cameraMoveDirection;

	cameraMoveDirection.z = Input::GetAxis("Depth");
	cameraMoveDirection.x = Input::GetAxis("Horizontal");
	cameraMoveDirection.y = Input::GetAxis("Vertical");

	_activeCamera->Move(cameraMoveDirection);
}

void SceneManager::Draw() {

	for (auto it = _sceneObjects.begin(); it != _sceneObjects.end(); it++) {

		it->second->Draw(_activeCamera);
	}
}