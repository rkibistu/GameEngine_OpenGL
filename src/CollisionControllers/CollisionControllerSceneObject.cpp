#include "stdafx.h"
#include "CollisionControllerSceneObject.h"
#include "SceneObject.h"
#include "SceneManager.h"
#include "Vertex.h"

CollisionControllerSceneObject::CollisionControllerSceneObject(SceneObject* sceneObj, Model* baseModel) {

	_sceneObject = sceneObj;

	Model* aabbModel = new Model();
	aabbModel->LoadAabbModel(baseModel->GetModelResource()->Vertices);
	_aabbModel = aabbModel;
	_aabbColliderWorldSpace = new Model::AabbCollider();	
}
CollisionControllerSceneObject::~CollisionControllerSceneObject() {

	if (_aabbModel) {
		if (_aabbModel->GetModelResource())
			delete _aabbModel->GetModelResource();
		delete _aabbModel;
		_aabbModel = nullptr;
	}
	if (_aabbColliderWorldSpace)
		delete _aabbColliderWorldSpace;
}

void CollisionControllerSceneObject::Start() {

	ModelMatrixChanged();
	UpdateAabbColliderValues(); //used to set aabbColliderWorldSpace too
}

void CollisionControllerSceneObject::CallOnCollisionEnter(SceneObject* collisionObj) {
	_sceneObject->OnCollisionEnter(collisionObj);
	if (_reciprocityCall)
		collisionObj->OnCollisionEnter(_sceneObject);
}
void CollisionControllerSceneObject::CallOnCollisionStay(SceneObject* collisionObj) {
	_sceneObject->OnCollisionStay(collisionObj);
	if (_reciprocityCall)
		collisionObj->OnCollisionStay(_sceneObject);
}
void CollisionControllerSceneObject::CallOnCollisionExit(SceneObject* collisionObj) {
	_sceneObject->OnCollisionExit(collisionObj);
	if (_reciprocityCall)
		collisionObj->OnCollisionExit(_sceneObject);
}

bool CollisionControllerSceneObject::ModelMatrixChanged() {

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

void CollisionControllerSceneObject::UpdateAabbColliderValues() {

	_aabbModel->UpdateAabbModel(_sceneObject->GetModel()->GetModelResource()->Vertices, _sceneObject->GetScale(), _sceneObject->GetRotation());

	Vector3 position = _sceneObject->GetPosition();
	_aabbColliderWorldSpace->OX = _aabbModel->GetAabbCollider()->OX + Vector2(position.x, position.x);
	_aabbColliderWorldSpace->OY = _aabbModel->GetAabbCollider()->OY + Vector2(position.y, position.y);
	_aabbColliderWorldSpace->OZ = _aabbModel->GetAabbCollider()->OZ + Vector2(position.z, position.z);
}

bool CollisionControllerSceneObject::IsCollidable() {

	return _sceneObject->IsCollidable();
}

bool CollisionControllerSceneObject::IsSelf(SceneObject* other) {

	return _sceneObject == other;
}


bool CollisionControllerSceneObject::ShouldCheckCollision() {

	bool modelMatrixChanged = ModelMatrixChanged();
	if (modelMatrixChanged && _aabbModel->GetAabbCollider() != nullptr)
		return true;
	return false;
}
