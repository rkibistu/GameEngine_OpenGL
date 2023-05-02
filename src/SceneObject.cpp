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
	for (int i = 0; i < _textures.size(); i++) {

		if (_textures[i])
			delete _textures[i];
	}
	_textures.clear();
}

void SceneObject::Draw() {


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

	_textures.push_back(texture);
}
