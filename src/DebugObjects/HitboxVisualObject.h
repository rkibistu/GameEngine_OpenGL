#pragma once

#include "SceneObject.h"

class HitboxVisualObject : public SceneObject {

public:
	HitboxVisualObject(Model* baseModel);
	~HitboxVisualObject();

	void Update(float deltaTime) override;
private:

};