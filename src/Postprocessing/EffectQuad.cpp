#include "stdafx.h"

#include "EffectQuad.h"
#include "SceneManager.h"
#include "Vertex.h"

EffectQuad::EffectQuad() {

	_quadShader = nullptr;
	_quadModel = nullptr;
	_quadTexture = nullptr;

	Init();
}
EffectQuad::~EffectQuad() {

	if (_quadModel) {

		delete _quadModel->GetModelResource();
		delete _quadModel;
		_quadModel = nullptr;
	}
}

void EffectQuad::Init() {

	_quadModel = new Model();
	_quadModel->LoadQuad();
}

void EffectQuad::Draw() {

	if (_quadModel == nullptr)
		return;
	if (_quadShader == nullptr)
		return;

	_quadModel->BindFilled();
	_quadShader->Bind();

	_quadTexture->Bind(0);

	_quadShader->SetAttributes();

	SetUniforms();

	glDrawElements(GL_TRIANGLES, _quadModel->GetIndicesFilledCount(), GL_UNSIGNED_SHORT, nullptr);

	_quadModel->Unbind();
}

void EffectQuad::SetUniforms() {

	_quadShader->SetUniform1i("u_Texture", 0);

	_quadShader->SetUniform1f("u_blurStep", 0.4); 

	_quadShader->SetUniform1i("u_scene", 0);
	_quadShader->SetUniform1i("u_bloom", 1);
}