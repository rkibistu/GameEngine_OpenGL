#include "stdafx.h"

#include "SceneObject.h"
#include "Vertex.h"

SceneObject::SceneObject()
	: _model(nullptr), _shader(nullptr) {
}

SceneObject::~SceneObject(){

	//i dont want to free the pointers here, because they are create in Resource manager
	/// they can be used later or by more SceneObjects
	//	they will be destroyd bu ResourceManager when it is the case
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

	Matrix model = GetModelMatrix();
	Matrix mvp = model * camera->GetMVP();
	_shader->SetUniformMatrix4fv("u_mvp", mvp);

	//_shader->SetUniformMatrix4fv("u_mvp", camera->GetMVP());
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

	_model = model;
}

void SceneObject::SetShader(Shader* shader){

	_shader = shader;
}

void SceneObject::AddTexture(Texture* texture){

	_textureResources.push_back(texture);
}

// PRIVATE

Matrix SceneObject::GetModelMatrix() {

	Matrix positionMat;
	positionMat.SetTranslation(_position);

	Matrix rotationMat;
	rotationMat.SetRotationY(_rotation.y);

	return rotationMat *  positionMat ;
}