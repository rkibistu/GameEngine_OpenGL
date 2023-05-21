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

	_activeCamera->Update(deltaTime);
	for (auto it = _sceneObjects.begin(); it != _sceneObjects.end(); it++) {

		it->second->Update(deltaTime);
	}
}

void SceneManager::Draw(ESContext* esContext) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (auto it = _sceneObjects.begin(); it != _sceneObjects.end(); it++) {

		if (it->second->GetDrawWired())
			it->second->DrawWired(_activeCamera);
		else
			it->second->Draw(_activeCamera);
	}

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void SceneManager::CreateDebugAxisObject() {

	ResourceManager& resourceManager = ResourceManager::GetInstance();

	SceneObject* axisObject = new SceneObject();
	axisObject->SetModel(resourceManager.GetSystemAxisModel());
	axisObject->SetDrawWired(true);
	axisObject->SetName("axis");


	_sceneObjects.insert({ 999,axisObject });
}