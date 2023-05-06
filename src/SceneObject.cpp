#include "stdafx.h"

#include "SceneObject.h"
#include "Vertex.h"

SceneObject::SceneObject()
	: _model(nullptr), _shader(nullptr) {
}

SceneObject::~SceneObject(){

	if (_model) {
		delete _model;
	}
	if (_shader) {
		delete _shader;
	}
	for (int i = 0; i < _textureResources.size(); i++) {

		if (_textureResources[i])
			delete _textureResources[i];
	}
	_textureResources.clear();
}

void SceneObject::Draw(Camera* camera) {

	if (_model == nullptr)
		return;
	if (_shader == nullptr)
		return;


	_model->BindFilled();
	_shader->Bind();
	for (int i = 0; i < _textureResources.size(); i++) {
		_textureResources[i]->Bind(i);
	}

	_shader->SetAttributes();
	_shader->SetUniformMatrix4fv("u_mvp", camera->GetMVP());
	_shader->SetUniform1i("u_Texture", 0);

	glDrawElements(GL_TRIANGLES, _model->GetIndicesFilledCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}
void SceneObject::DrawWired(Camera* camera) {
	
	if (_model == nullptr)
		return;
	if (_shader == nullptr)
		return;

	_shader->Bind();
	_model->BindWired();
	for (int i = 0; i < _textureResources.size(); i++) {
		_textureResources[i]->Bind(i);
	}

	_shader->SetAttributes();
	_shader->SetUniformMatrix4fv("u_mvp", camera->GetMVP());
	_shader->SetUniform1i("u_Texture", 0);

	glDrawElements(GL_LINES, _model->GetIndicesWiredCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}

void SceneObject::SetModel(Model* model){
	if (_model)
		delete _model;
	_model = model;
}

void SceneObject::SetShader(Shader* shader){

	if (_shader)
		delete _shader;
	_shader = shader;
}

void SceneObject::AddTexture(Texture* texture){

	_textureResources.push_back(texture);
}
