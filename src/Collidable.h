#pragma once


class SceneObject;

class Collidable {

public:
	Collidable();

	virtual void OnCollisionEnter() = 0;
	virtual void OnCollisionStay() = 0;
	virtual void OnCollisionExit() = 0;

	void Update();

	inline void SetAabbObject(SceneObject* obj) { _aabbObj = obj; }
	inline SceneObject* GetAabbObject() { return _aabbObj; }

private:
	bool CheckCollide(SceneObject* other);
private:
	SceneObject* _aabbObj;

	bool _collisionEnter;
	bool _collisionEnterOld;
};