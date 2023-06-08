#include "stdafx.h"
#include "CollisionController.h"
#include "SceneObject.h"
#include "SceneManager.h"

CollisionController::CollisionController(SceneObject* sceneObj, Model::AabbCollider* aabbCollider) {

	_oldPosition = Vector3(0, 0, 0);
	_oldScale = Vector3(0, 0, 0);
	_oldRotation = Vector3(0, 0, 0);

	_sceneObject = sceneObj;
	_aabbCollider = aabbCollider;
	_aabbColliderWorldSpace = new Model::AabbCollider();
}
CollisionController::~CollisionController() {

	if (_aabbColliderWorldSpace)
		delete _aabbColliderWorldSpace;
}

void CollisionController::Update(float deltaTime) {

	bool modelMatrixChanged = ModelMatrixChanged();
	if (modelMatrixChanged && _aabbCollider != nullptr) {

		//recalculate aabb
		//std::cout << "Recalculate aabb for " << _name << std::endl;
		UpdateAabbColliderValues();

		//verif coliziuni
		//verifiam doar daca s-aschimbat pozitia/rotatia/scara. Vara miscare nu exista coliziune
		if (_sceneObject->IsCollidable()) {

			TestColliding();
		}
	}
}

void CollisionController::TestColliding() {

	SceneManager& sceneManager = SceneManager::GetInstance();
	auto sceneObejcts = sceneManager.GetSceneObjects();

	for (auto it = sceneObejcts.begin(); it != sceneObejcts.end(); it++) {

		if (it->second->IsCollidable() == false)
			continue;
		if (it->second == this->_sceneObject)
			continue;

		auto otherAabbColl = it->second->GetCollisionController()->_aabbColliderWorldSpace;
		if (_aabbColliderWorldSpace->OX.x <= otherAabbColl->OX.y && _aabbColliderWorldSpace->OX.y >= otherAabbColl->OX.x &&
			_aabbColliderWorldSpace->OY.x <= otherAabbColl->OY.y && _aabbColliderWorldSpace->OY.y >= otherAabbColl->OY.x &&
			_aabbColliderWorldSpace->OZ.x <= otherAabbColl->OZ.y && _aabbColliderWorldSpace->OZ.y >= otherAabbColl->OZ.x) {

			std::cout << "Collide " << _sceneObject->GetName() << " with " << it->second->GetName() << std::endl;
		}
	}
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

	Vector3 position = _sceneObject->GetPosition();
	_aabbColliderWorldSpace->OX = _aabbCollider->OX + Vector2(position.x, position.x);
	_aabbColliderWorldSpace->OY = _aabbCollider->OY + Vector2(position.y, position.y);
	_aabbColliderWorldSpace->OZ = _aabbCollider->OZ + Vector2(position.z, position.z);
}

