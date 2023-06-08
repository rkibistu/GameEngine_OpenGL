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
	void TestColliding();

	bool ModelMatrixChanged();
	void UpdateAabbColliderValues();
private:
	SceneObject* _sceneObject;

	Vector3 _oldPosition;
	Vector3 _oldScale;
	Vector3 _oldRotation;

	Model* _aabbModel;
	Model::AabbCollider* _aabbColliderWorldSpace;
};