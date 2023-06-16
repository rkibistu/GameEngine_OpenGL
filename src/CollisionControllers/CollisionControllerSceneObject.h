#pragma once


#include "CollisionControllerBase.h"


class CollisionControllerSceneObject : public CollisionControllerBase {

public:
	CollisionControllerSceneObject(SceneObject* sceneObj, Model* baseModel);
	~CollisionControllerSceneObject();

	void Start() override;



protected:


	void CallOnCollisionEnter(SceneObject* collisionObj) override;
	void CallOnCollisionStay(SceneObject* collisionObj) override;
	void CallOnCollisionExit(SceneObject* collisionObj) override;


	//verifica daca s-a schimbat pozitia/rotatia/scala de la ultimul frame
	bool ModelMatrixChanged() override;

	//actualizeaza valorile pentru aabbCollider din cadrul modelului _aabbModel
	//	actualizeaza si aabbColidderWolrdSpace
	void UpdateAabbColliderValues() override;

	//returneaza true daca boiectul de care e atasat cotnroller e collidable
	//false altfel
	bool IsCollidable() override;

	//returneaza true daca other este acelasi cu obiectul de care e atasat controllerul
	bool IsSelf(SceneObject* other) override;

	bool ShouldCheckCollision() override;

private:

	//obiectul de care e atasat controllerul
	SceneObject* _sceneObject;
};