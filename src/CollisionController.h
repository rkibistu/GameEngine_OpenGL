#pragma once

#include "Model.h"
#include "Utilities/utilities.h"


class SceneObject;


class CollisionController {

public:
	CollisionController(SceneObject* sceneObj, Model* baseModel);
	~CollisionController();

	void Update(float deltaTime);

	inline Model* GetAabbModel() { return _aabbModel; }
private:
	//verifica daca obiectul curent a atins alt obiect din scena
	void TestColliding();

	//verifica daca s-a schimbat pozitia/rotatia/scala de la ultimul frame
	bool ModelMatrixChanged();

	//actualizeaza valorile pentru aabbCollider din cadrul modelului _aabbModel
	//	actualizeaza si aabbColidderWolrdSpace
	void UpdateAabbColliderValues();
private:

	//obiectul parinte
	SceneObject* _sceneObject;

	Vector3 _oldPosition;
	Vector3 _oldScale;
	Vector3 _oldRotation;

	//modelul aabb, centrat in origine.
	//	Este folosit si pentru afisarea in HitboxVisualObject
	Model* _aabbModel;

	//AabbCollider convertit in WorldSpace.
	//		este folosit pentru a verifica coliziunile
	Model::AabbCollider* _aabbColliderWorldSpace;
};