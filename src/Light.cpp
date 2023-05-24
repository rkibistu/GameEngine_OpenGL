#include "stdafx.h"
#include "Light.h"
#include "DebugLightObject.h"
#include "SceneManager.h"

LightObject::LightObject(bool isDebug)
	: _type(Point) {

	_diffuseColor = Vector3();
	_specularColor = Vector3();

	_isDebug = isDebug;

	if (!isDebug)
		CreateDebugObjects();
}

LightObject::LightObject(Type type, Vector3 diffuseColor, Vector3 specularColor, bool isDebug)
	: _type(type), _diffuseColor(diffuseColor), _specularColor(specularColor) {

	_isDebug = isDebug;

	if (!isDebug)
		CreateDebugObjects();
}


//
//void LightObject::DrawDebug(Camera* camera, GLenum DrawMode) {
//
//	for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {
//
//		if(it->second->GetDrawWired())
//			it->second->DrawDebug(camera, GL_LINES);
//		else
//			it->second->DrawDebug(camera, GL_TRIANGLES);
//	}
//}

void LightObject::SetUniformsParticular(Camera* camera) {

	_debugShader->SetUniform3f("u_color", GetDiffuseColor());
}

void LightObject::CreateDebugObjects() {
	SceneManager& sceneManager = SceneManager::GetInstance();

	//create debug objects specific to light
	SceneObject* debugLight = new LightObject(_type,_diffuseColor,_specularColor,true);
	debugLight->SetParent(this);
	debugLight->SetDrawWired(false);
	debugLight->SetModel(sceneManager.GetDebugLightModel());
	debugLight->SetDebugShader(sceneManager.GetDebugLightShader());
	debugLight->SetName("lightDebug");
	debugLight->SetScale(0.2f, 0.2f, 0.2f);	

	_debugObjects.insert({ _debugObjects.size() + 1,debugLight });
}