#include "stdafx.h"
#include "Collidable.h"
#include "SceneObject.h"
#include "SceneManager.h"

Collidable::Collidable() {

	_aabbObj = nullptr;
}

void Collidable::Update() {

	//not created model yet
	//if (_aabbObj == nullptr)
	//	return;

	//if (_aabbObj->GetModel() == nullptr)
	//	return;

	//SceneManager& sceneManager = SceneManager::GetInstance();

	//auto sceneObejcts = sceneManager.GetSceneObjects();
	//for (auto it = sceneObejcts.begin(); it != sceneObejcts.end(); it++) {

	//	if(CheckCollide(it->second)) {

	//		OnCollisionStay();
	//	}
	//}
	

	//cehck al collisions
	//OnCollisionEnter();
}

bool Collidable::CheckCollide(SceneObject* other) {

	//asta contine vectorii pe fiecare axa care definesc colliderul
	//Model::AabbCollider* thisAabbColl = _aabbObj->GetModel()->GetAabbCollider();

	//SceneObject* otherAabbObj = other->GetAabbObject();
	//if (otherAabbObj == nullptr)
	//	return false;
	//Model* otherModel = otherAabbObj->GetModel();
	//if (otherModel == nullptr)
	//	return false;

	//Model::AabbCollider* otherAabbColl = otherModel->GetAabbCollider();

	//if (thisAabbColl == nullptr || otherAabbColl == nullptr)
	//	return false;

	//if (thisAabbColl->OX.x <= otherAabbColl->OX.y && thisAabbColl->OX.y >= otherAabbColl->OX.x &&
	//	thisAabbColl->OY.x <= otherAabbColl->OY.y && thisAabbColl->OY.y >= otherAabbColl->OY.x &&
	//	thisAabbColl->OZ.x <= otherAabbColl->OZ.y && thisAabbColl->OZ.y >= otherAabbColl->OZ.x) {

	//	return true;
	//}
	return false;
}