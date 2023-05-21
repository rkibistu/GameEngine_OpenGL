#include "stdafx.h"
#include "DebugLightObject.h"
#include "SceneManager.h"

DebugLightObject::DebugLightObject()
	: LightObject(true) {

	SceneManager& sceneManager = SceneManager::GetInstance();

	SetModel(sceneManager.GetDebugLightModel());
	_debugShader = sceneManager.GetDebugLightShader();
	SetName("lightDebug");
	SetScale(0.2f, 0.2f, 0.2f);
}
DebugLightObject::DebugLightObject(Type type, Vector3 diffuseColor, Vector3 specularColor)
	: DebugLightObject() {

	_type = type;
	_diffuseColor = diffuseColor;
	_specularColor = specularColor;
}

void DebugLightObject::Update(float deltaTime) {

	SetPosition(_parent->GetPosition());
	SetRotation(_parent->GetRotation());
}


void DebugLightObject::Draw(Camera* camera) {

	if (_model == nullptr)
		return;
	if (_shader == nullptr)
		return;

	_model->BindFilled();
	_debugShader->Bind();

	_debugShader->SetAttributes();

	SetUniformsCommon(camera);
	SetUniformsParticular(camera);

	_debugShader->SetUniform3f("u_color", GetDiffuseColor());

	glDrawElements(GL_TRIANGLES, _model->GetIndicesFilledCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}