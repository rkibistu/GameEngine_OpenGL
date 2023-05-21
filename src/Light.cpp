#include "stdafx.h"
#include "Light.h"
#include "DebugLightObject.h"

LightObject::LightObject(bool isDebug)
	: _type(Point) {

	_diffuseColor = Vector3();
	_specularColor = Vector3();

	if (!isDebug)
		CreateDebugObjects();
}

LightObject::LightObject(Type type, Vector3 diffuseColor, Vector3 specularColor, bool isDebug)
	: _type(type), _diffuseColor(diffuseColor), _specularColor(specularColor) {

	if (!isDebug)
		CreateDebugObjects();
}



void LightObject::DrawDebug(Camera* camera) {

	for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {

		it->second->Draw(camera);
	}
}

void LightObject::CreateDebugObjects() {

	//create debug objects specific to light
	SceneObject* debugLight = new DebugLightObject(_type,_diffuseColor,_specularColor);
	debugLight->SetParent(this);

	_debugObjects.insert({ _debugObjects.size() + 1,debugLight });
}