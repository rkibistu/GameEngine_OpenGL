#include "stdafx.h"

#include "LightDebugObject.h"
#include "SceneManager.h"

LightDebugObject::LightDebugObject(LightObject* parent)
	 : LightObject(parent->GetType(), parent->GetDiffuseColor(), parent->GetSpecularColor(), true) {

	SceneManager& sceneManager = SceneManager::GetInstance();

	_parent = parent;
	_drawWired = false;
	_model  = sceneManager.GetDebugLightModel();
	_shader = sceneManager.GetDebugLightShader();
	_name = "lightDebug";
	SetScale(0.2f, 0.2f, 0.2f);
}


void LightDebugObject::Update(float deltaTime) {

	SetPosition(_parent->GetPosition());
	SetRotation(_parent->GetRotation());
}

void LightDebugObject::SetUniformsParticular(Camera* camera) {

	Shader* currentShader = (_drawWired == true) ? _wiredShader : _shader;
	_shader->SetUniform3f("u_color", GetDiffuseColor());
}
