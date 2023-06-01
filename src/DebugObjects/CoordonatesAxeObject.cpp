#include "stdafx.h"
#include "CoordonatesAxeObject.h"

#include "SceneManager.h"
#include "ResourceManager.h"

CoordonateAxeObject::CoordonateAxeObject()
	: SceneObject(true) {

	ResourceManager& resourceManager = ResourceManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();

	
	_model = resourceManager.GetSystemAxisModel();
	_shader = nullptr;
	_debugShader = sceneManager.GetDebugShader();
	_name = "axis";
	SetScale(10.0f, 10.0f, 10.0f);
	_drawWired = true;
	_isDebug = true;
}
