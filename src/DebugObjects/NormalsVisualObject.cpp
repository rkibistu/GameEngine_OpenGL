#include "stdafx.h"
#include "NormalsVisualObject.h"

NormalsVisualObject::NormalsVisualObject(Model* baseModel) 
	: SceneObject(true) {

	Model* normalModel = new Model();
	normalModel->LoadNormalModel(baseModel->GetModelResource()->Vertices);
	_model = normalModel;
	_name = "normals";
	_drawWired = TRUE;
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

void NormalsVisualObject::Draw(Camera* camera) {

	if (_drawWired) {
		DrawDebugWired(camera);
	}
}