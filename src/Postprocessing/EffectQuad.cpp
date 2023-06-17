#include "stdafx.h"

#include "EffectQuad.h"
#include "SceneManager.h"
#include "Vertex.h"

EffectQuad::EffectQuad(GLuint textureId) {

	_texture = textureId;
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
	if (_quadTexture) {

		delete _quadTexture->GetTextureResource();
		delete _quadTexture;
		_quadTexture = nullptr;
	}
}

void EffectQuad::Init() {

	_quadShader = SceneManager::GetInstance().GetTextShader();
	_quadModel = new Model();
	_quadModel->LoadTextQuad();

	TextureResource* textureResource = new TextureResource();
	textureResource->Type = TEXTURE_TYPE_2D;
	Texture* texture = new Texture();
	texture->SetTextureId(_texture);
	texture->SetTextureResource(textureResource);
	_quadTexture = texture;
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

	_quadShader->SetUniform1i("u_Texture", 0);

	glDrawElements(GL_TRIANGLES, _quadModel->GetIndicesFilledCount(), GL_UNSIGNED_SHORT, nullptr);

	_quadModel->Unbind();
}