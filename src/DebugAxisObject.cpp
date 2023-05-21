#include "stdafx.h"
#include "DebugAxisObject.h"
#include "ResourceManager.h"

DebugAxisObject::DebugAxisObject() {

	ResourceManager& resourceManager = ResourceManager::GetInstance();

	SetModel(resourceManager.GetSystemAxisModel());
	SetName("axis");
	SetScale(10.0f, 10.0f, 10.0f);
}

void DebugAxisObject::Draw(Camera* camera) {
	
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