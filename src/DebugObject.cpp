#include "stdafx.h"
#include "DebugObject.h"

DebugObject::DebugObject() 
	: SceneObject(true) {

}

void DebugObject::Update(float deltaTime) {

	SetPosition(_parent->GetPosition());
	SetRotation(_parent->GetRotation());
	SetScale(_parent->GetScale());
}

void DebugObject::Draw(Camera* camera) {

	if (_model == nullptr)
		return;
	if (_debugShader == nullptr)
		return;
	_debugShader->Bind();
	_model->BindWired();

	_debugShader->SetAttributes();

	SetUniformsCommon(camera);
	SetUniformsParticular(camera);

	glDrawElements(GL_LINES, _model->GetIndicesWiredCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}