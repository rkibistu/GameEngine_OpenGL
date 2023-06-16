#include "stdafx.h"
#include "CollisionControllerCamera.h"

#include "SceneManager.h"
#include "Camera.h"
#include "Vertex.h"

CollisionControllerCamera::CollisionControllerCamera() {

	_colliderSize.x = 20;
	_colliderSize.y = 40;

	Model* aabbModel = new Model();
	aabbModel->LoadAabbModelCamera(_colliderSize.x,_colliderSize.y);
	_aabbModel = aabbModel;
	_aabbColliderWorldSpace = new Model::AabbCollider();

	_activeCamera = SceneManager::GetInstance().GetActiveCamera();
}
CollisionControllerCamera::~CollisionControllerCamera() {

	if (_aabbModel) {
		if (_aabbModel->GetModelResource())
			delete _aabbModel->GetModelResource();
		delete _aabbModel;
		_aabbModel = nullptr;
	}
	if (_aabbColliderWorldSpace)
		delete _aabbColliderWorldSpace;
}

void CollisionControllerCamera::PreUpdate() {

	_activeCamera = SceneManager::GetInstance().GetActiveCamera();
}

void CollisionControllerCamera::CallOnCollisionEnter(SceneObject* collisionObj) {
	_activeCamera->OnCollisionEnter(collisionObj);
}
void CollisionControllerCamera::CallOnCollisionStay(SceneObject* collisionObj)  {
	_activeCamera->OnCollisionStay(collisionObj);
}
void CollisionControllerCamera::CallOnCollisionExit(SceneObject* collisionObj)  {
	_activeCamera->OnCollisionExit(collisionObj);
}

bool CollisionControllerCamera::ModelMatrixChanged() {

	bool res = false;

	if (_activeCamera->GetPosition() != _oldPosition) {
		res = true;
		_oldPosition = _activeCamera->GetPosition();
	}
	if (_activeCamera->GetRotation() != _oldRotation) {
		res = true;
		_oldRotation = _activeCamera->GetRotation();
	}
	return res;
}

void CollisionControllerCamera::UpdateAabbColliderValues() {

	//_aabbModel->UpdateAabbModel(_sceneObject->GetModel()->GetModelResource()->Vertices, _sceneObject->GetScale(), _sceneObject->GetRotation());

	Vector3 position = _activeCamera->GetPosition();
	_aabbColliderWorldSpace->OX = _aabbModel->GetAabbCollider()->OX + Vector2(position.x, position.x);
	_aabbColliderWorldSpace->OY = _aabbModel->GetAabbCollider()->OY + Vector2(position.y, position.y);
	_aabbColliderWorldSpace->OZ = _aabbModel->GetAabbCollider()->OZ + Vector2(position.z, position.z);
}

bool CollisionControllerCamera::IsCollidable() {

	return true;
}

bool CollisionControllerCamera::IsSelf(SceneObject* other) {

	return false;
}

bool CollisionControllerCamera::ShouldCheckCollision() {

	return true;
}
