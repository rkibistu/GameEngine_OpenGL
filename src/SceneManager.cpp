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

	//free memory for camera and for sceneobjects
	for (auto it = _sceneObjects.begin(); it != _sceneObjects.end(); it++) {
		if (it->second)
			delete it->second;
	}
	for (auto it = _cameras.begin(); it != _cameras.end(); it++) {
		if (it->second)
			delete it->second;
	}
	for (auto it = _lightObjects.begin(); it != _lightObjects.end(); it++) {
		if (it->second)
			delete it->second;
	}
	_xmlParser.Destroy();

	if (_spInstance)
		delete _spInstance;
}

void SceneManager::Init(ESContext* esContext) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_xmlParser.Init("Resources/XMLs/sceneManager.xml");
	_xmlParser.ReadDefaultSettings(_defaultSettings);
	_xmlParser.ReadLights(_lightObjects);
	_xmlParser.ReadObjects(_sceneObjects);
	_xmlParser.ReadCameras(_cameras, &_activeCamera);
	_xmlParser.ReadControls();

	_xmlParser.ReadBackgroundColor(_backgroundColor);
	_xmlParser.ReadFog(_fog);
	_xmlParser.ReadAmbientalLight(_ambientalLight);

	glClearColor(_backgroundColor.x, _backgroundColor.y, _backgroundColor.z, 0.0f);
	glEnable(GL_DEPTH_TEST);

	CreateDebugAxisObject();
}

void SceneManager::Update(ESContext* esContext, float deltaTime) {

	if (Input::GetKeyDown(KeyCode::P)) {
		_debugMode = !_debugMode;
	}

	_activeCamera->Update(deltaTime);
	for (auto it = _sceneObjects.begin(); it != _sceneObjects.end(); it++) {

		it->second->Update(deltaTime);
		if (_debugMode) {
			it->second->UpdateDebugObjects(deltaTime);
		}
	}

	for (auto it = _lightObjects.begin(); it != _lightObjects.end(); it++) {

		it->second->Update(deltaTime);
		if (_debugMode) {
			it->second->UpdateDebugObjects(deltaTime);
		}
	}

	UpdateDebugObjects();
}
void SceneManager::Draw(ESContext* esContext) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (auto it = _sceneObjects.begin(); it != _sceneObjects.end(); it++) {

		if (_debugMode) {

			it->second->DrawDebugWired(_activeCamera);
			it->second->DrawDebugObjects(_activeCamera);
		}
		else
			it->second->Draw(_activeCamera);
	}

	if (_debugMode) {

		for (auto it = _lightObjects.begin(); it != _lightObjects.end(); it++) {

			it->second->DrawDebugObjects(_activeCamera);
		}
		for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {

			it->second->DrawDebugWired(_activeCamera);
		}
	}

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void SceneManager::CreateDebugAxisObject() {

	ResourceManager& resourceManager = ResourceManager::GetInstance();

	SceneObject* axisObject = new SceneObject(true);
	axisObject->SetModel(resourceManager.GetSystemAxisModel());

	axisObject->SetDrawWired(true);
	axisObject->SetName("sceneAxis");
	axisObject->SetScale(0.1f, 0.1f, 0.1f);

	_debugObjects.insert({ _debugObjects.size() + 1,axisObject});
}
void SceneManager::UpdateDebugObjects() {

	if (!_debugMode)
		return;

	Vector3 cameraPos = _activeCamera->GetPosition();
	Vector3 offset(9, 7, +20);
	for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {

		//this for scene axis
		it->second->StayOnSreen();
	}
}