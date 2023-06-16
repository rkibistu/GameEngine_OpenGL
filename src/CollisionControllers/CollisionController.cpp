#include "stdafx.h"
#include "CollisionController.h"
#include "SceneObject.h"
#include "SceneManager.h"
#include "Vertex.h"

CollisionController::CollisionController(SceneObject* sceneObj, Model* baseModel) {

	_sceneObject = sceneObj;

	Model* aabbModel = new Model();
	aabbModel->LoadAabbModel(baseModel->GetModelResource()->Vertices);
	_aabbModel = aabbModel;
	_aabbColliderWorldSpace = new Model::AabbCollider();	
}
CollisionController::~CollisionController() {

	if (_aabbModel) {
		if (_aabbModel->GetModelResource())
			delete _aabbModel->GetModelResource();
		delete _aabbModel;
		_aabbModel = nullptr;
	}
	if (_aabbColliderWorldSpace)
		delete _aabbColliderWorldSpace;
}

void CollisionController::Start() {

	ModelMatrixChanged();
	UpdateAabbColliderValues(); //used to set aabbColliderWorldSpace too
}

void CollisionController::CallOnCollisionEnter(SceneObject* collisionObj) {
	_sceneObject->OnCollisionEnter(collisionObj);
}
void CollisionController::CallOnCollisionStay(SceneObject* collisionObj) {
	_sceneObject->OnCollisionStay(collisionObj);
}
void CollisionController::CallOnCollisionExit(SceneObject* collisionObj) {
	_sceneObject->OnCollisionExit(collisionObj);
}

bool CollisionController::ModelMatrixChanged() {

	bool res = false;

	if (_sceneObject->GetPosition() != _oldPosition) {
		res = true;
		_oldPosition = _sceneObject->GetPosition();
	}
	if (_sceneObject->GetRotation() != _oldRotation) {
		res = true;
		_oldRotation = _sceneObject->GetRotation();
	}
	if (_sceneObject->GetScale() != _oldScale) {
		res = true;
		_oldScale = _sceneObject->GetScale();
	}
	return res;
}

void CollisionController::UpdateAabbColliderValues() {

	_aabbModel->UpdateAabbModel(_sceneObject->GetModel()->GetModelResource()->Vertices, _sceneObject->GetScale(), _sceneObject->GetRotation());

	Vector3 position = _sceneObject->GetPosition();
	_aabbColliderWorldSpace->OX = _aabbModel->GetAabbCollider()->OX + Vector2(position.x, position.x);
	_aabbColliderWorldSpace->OY = _aabbModel->GetAabbCollider()->OY + Vector2(position.y, position.y);
	_aabbColliderWorldSpace->OZ = _aabbModel->GetAabbCollider()->OZ + Vector2(position.z, position.z);
}

bool CollisionController::IsCollidable() {

	return _sceneObject->IsCollidable();
}

bool CollisionController::IsSelf(SceneObject* other) {

	return _sceneObject == other;
}


