#pragma once

#include "Model.h"
#include "Utilities/utilities.h"


class SceneObject;


class CollisionController {

public:
	CollisionController(SceneObject* sceneObj, Model::AabbCollider* aabbCollider);
	~CollisionController();
	void Update(float deltaTime);

private:
	void TestColliding();

	bool ModelMatrixChanged();
	void UpdateAabbColliderValues();
private:
	SceneObject* _sceneObject;

	Vector3 _oldPosition;
	Vector3 _oldScale;
	Vector3 _oldRotation;

	Model::AabbCollider* _aabbCollider;
	Model::AabbCollider* _aabbColliderWorldSpace;
};