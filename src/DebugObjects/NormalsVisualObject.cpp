#include "stdafx.h"
#include "NormalsVisualObject.h"
#include "Vertex.h"

NormalsVisualObject::NormalsVisualObject(Model* baseModel) 
	: SceneObject(true) {

	Model* normalModel = new Model();
	normalModel->LoadNormalModel(baseModel->GetModelResource()->Vertices);
	_model = normalModel;
	_name = "normals";
	_drawWired = TRUE;
}
NormalsVisualObject::~NormalsVisualObject() {

	if (_model) {
		if (_model->GetModelResource())
			delete _model->GetModelResource();
		delete _model;
		_model = nullptr;
	}
}

void NormalsVisualObject::Update(float deltaTime) {

	if (_parent == nullptr) {
		std::cout << "[ERROR] Parent is null for object: " << _name << std::endl;
		return;
	}

	SetPosition(_parent->GetPosition());
	SetRotation(_parent->GetRotation());
	SetScale(_parent->GetScale());
}

