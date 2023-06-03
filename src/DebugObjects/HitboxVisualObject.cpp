#include "stdafx.h"
#include "HitboxVisualObject.h"
#include "Vertex.h"
#include "SceneManager.h"
#include "Vertex.h"


HitboxVisualObject::HitboxVisualObject(Model* baseModel)
	: SceneObject(true) {

	SceneManager& scneneManager = SceneManager::GetInstance();

	Model* aabbModel = new Model();
	aabbModel->LoadAabbModel(baseModel->GetModelResource()->Vertices);
	_model = aabbModel;
	_wiredShader = scneneManager.GetWiredShader();
	_name = "aabb";
	_drawWired = true;
}
HitboxVisualObject::~HitboxVisualObject() {

	if (_model) {
		if(_model->GetModelResource())
			delete _model->GetModelResource();
		delete _model;
		_model = nullptr;
	}
}
void HitboxVisualObject::Update(float deltaTime) {

	if (_parent == nullptr) {
		std::cout << "[ERROR] Parent is null for object: " << _name << std::endl;
		return;
	}

	SetPosition(_parent->GetPosition());
	if (_oldScale != _parent->GetScale() || _oldRotation !=  _parent->GetRotation()) {

		_oldScale = _parent->GetScale();
		_oldRotation = _parent->GetRotation();
	
		_model->UpdateAabbModel(_parent->GetModel()->GetModelResource()->Vertices, _oldScale, _oldRotation);
	}
}
