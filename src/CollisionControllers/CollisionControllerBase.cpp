#include "stdafx.h"
#include "CollisionControllerBase.h"
#include "SceneManager.h"




void CollisionControllerBase::Update(float deltaTime) {

	PreUpdate();

	if(ShouldCheckCollision()){
		//recalculate aabb and aabbWorldSpace
		UpdateAabbColliderValues();

		//verificam coliziuni
		if (IsCollidable()) {

			CheckColiisions();
		}
	}
	else {

		//obiectul nu s-a miscat, apelam OnCollisionStay pt obiectele din _isColliding
		CallStayCollisionMethods();
	}
}

void CollisionControllerBase::CheckColiisions() {

	SceneManager& sceneManager = SceneManager::GetInstance();
	auto sceneObejcts = sceneManager.GetSceneObjects();

	for (auto it = sceneObejcts.begin(); it != sceneObejcts.end(); it++) {

		if (it->second->IsCollidable() == false)
			continue;
		if (IsSelf(it->second))
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

void CollisionControllerBase::CallCollisionMethods(SceneObject* collisionObj) {

	//std::cout << "Collide " << _sceneObject->GetName() << " with " << collisionObj->GetName() << std::endl;
	auto it = _isColliding.find(collisionObj->GetId());
	if (it == _isColliding.end()) {

		//first frame of collision
		_isColliding.insert({ collisionObj->GetId(), true });

		CallOnCollisionEnter(collisionObj);
	}
	else {

		//the collision was on
		CallOnCollisionStay(collisionObj);
	}

}

void CollisionControllerBase::CallExitCollisionMethids(SceneObject* collisionObj) {

	auto it = _isColliding.find(collisionObj->GetId());
	if (it != _isColliding.end()) {

		//the collision ended this frame
		_isColliding.erase(it);

		CallOnCollisionExit(collisionObj);
	}
}

void CollisionControllerBase::CallStayCollisionMethods() {

	SceneManager& sceneManager = SceneManager::GetInstance();
	for (auto it = _isColliding.begin(); it != _isColliding.end(); it++) {

		CallOnCollisionStay(sceneManager.GetSceneObject(it->first));
	}
}

