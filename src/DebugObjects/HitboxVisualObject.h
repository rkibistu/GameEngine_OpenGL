#pragma once

#include "SceneObject.h"

class HitboxVisualObject : public SceneObject {

public:
	HitboxVisualObject(SceneObject* parent, Model* baseModel);
	~HitboxVisualObject();

	void Update(float deltaTime) override;
private:

};