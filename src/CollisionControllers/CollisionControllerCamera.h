#pragma once

#include "CollisionControllerBase.h"

class Camera;

class CollisionControllerCamera : public CollisionControllerBase {

public:

	CollisionControllerCamera();
	~CollisionControllerCamera();

	void Start() { ; }

protected:

	void PreUpdate() override;

	void CallOnCollisionEnter(SceneObject* collisionObj) override;
	void CallOnCollisionStay(SceneObject* collisionObj) override;
	void CallOnCollisionExit(SceneObject* collisionObj) override;

	bool CollisionControllerCamera::ModelMatrixChanged() override;

	void UpdateAabbColliderValues() override;


	bool IsCollidable() override;

	bool IsSelf(SceneObject* other) override;

private:
	Camera* _activeCamera;
	Vector2 _colliderSize;
};