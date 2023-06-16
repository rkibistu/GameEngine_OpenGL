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

void CollisionController::Update(float deltaTime) {

	bool modelMatrixChanged = ModelMatrixChanged();
	if (modelMatrixChanged && _aabbModel->GetAabbCollider() != nullptr) {

		//recalculate aabb and aabbWorldSpace
		UpdateAabbColliderValues();

		//verificam coliziuni
		if (_sceneObject->IsCollidable()) {

			CheckColiisions();
		}
	}
	else {

		//obiectul nu s-a miscat, apelam OnCollisionStay pt obiectele din _isColliding
		CallStayCollisionMethods();
	}
}

void CollisionController::CheckColiisions() {

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

			CallCollisionMethods(it->second);
		}
		else {
			CallExitCollisionMethids(it->second);
		}
	}
}

void CollisionController::CallCollisionMethods(SceneObject* collisionObj) {

	//std::cout << "Collide " << _sceneObject->GetName() << " with " << collisionObj->GetName() << std::endl;
	auto it = _isColliding.find(collisionObj->GetId());
	if (it == _isColliding.end()) {

		//first frame of collision
		_isColliding.insert({ collisionObj->GetId(), true });

		_sceneObject->OnCollisionEnter(collisionObj);
	}
	else {

		//the collision was on
		_sceneObject->OnCollisionStay(collisionObj);
	}

}

void CollisionController::CallExitCollisionMethids(SceneObject* collisionObj) {

	auto it = _isColliding.find(collisionObj->GetId());
	if (it != _isColliding.end()) {

		//the collision ended this frame
		_isColliding.erase(it);

		_sceneObject->OnCollisionExit(collisionObj);
	}
}

void CollisionController::CallStayCollisionMethods() {

	SceneManager& sceneManager = SceneManager::GetInstance();
	for (auto it = _isColliding.begin(); it != _isColliding.end(); it++) {

		_sceneObject->OnCollisionStay(sceneManager.GetSceneObject(it->first));
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

	_aabbModel->UpdateAabbModel(_sceneObject->GetModel()->GetModelResource()->Vertices, _sceneObject->GetScale(), _sceneObject->GetRotation());

	Vector3 position = _sceneObject->GetPosition();
	_aabbColliderWorldSpace->OX = _aabbModel->GetAabbCollider()->OX + Vector2(position.x, position.x);
	_aabbColliderWorldSpace->OY = _aabbModel->GetAabbCollider()->OY + Vector2(position.y, position.y);
	_aabbColliderWorldSpace->OZ = _aabbModel->GetAabbCollider()->OZ + Vector2(position.z, position.z);
}

