#include "stdafx.h"
#include "Fire.h"


Fire::Fire(float dispersionMax) 
	: _dispersionMax(dispersionMax) {

	_time = 0.0f;
}

void Fire::Update(float deltaTime) {

	_time += deltaTime / 1.5f;
	if (_time > 1.0f)
		_time -= 1.0f;

	UpdateDebugObjects(deltaTime);
}

void Fire::SetUniformsParticular(Camera* camera) {
	 
	_shader->SetUniform1i("u_Texture", 0);
	_shader->SetUniform1i("u_maskTexture", 1);
	_shader->SetUniform1i("u_displacementTexture", 2);
	_shader->SetUniform1i("u_TextureCube", 3);

	_shader->SetUniform1f("u_displMax", _dispersionMax);
	_shader->SetUniform1f("u_time", _time);
}
