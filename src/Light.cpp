#include "stdafx.h"
#include "Light.h"
#include "SceneManager.h"
#include "DebugObjects//LightDebugObject.h"

LightObject::LightObject(bool isDebug)
	: _type(Point) {

	_diffuseColor = Vector3();
	_specularColor = Vector3();
	_name = "lightObject";

	_isDebug = isDebug;

	_collidable = false;

	if (!isDebug)
		CreateDebugObjects();
}

LightObject::LightObject(Type type, Vector3 diffuseColor, Vector3 specularColor, bool isDebug)
	: _type(type), _diffuseColor(diffuseColor), _specularColor(specularColor) {

	_isDebug = isDebug;
	_name = "lightObject";

	if (!isDebug)
		CreateDebugObjects();
}

void LightObject::UpdateDebugObjects(float deltaTime) {

	for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {

		it->second->Update(deltaTime);
	}
}

void LightObject::SetUniformsParticular(Camera* camera) {

	_wiredShader->SetUniform3f("u_color", GetDiffuseColor());
}


void LightObject::CreateDebugObjects() {

	//create debug objects specific to light
	SceneObject* debugLight = new LightDebugObject(this);
	_debugObjects.insert({ _debugObjects.size() + 1,debugLight });
}