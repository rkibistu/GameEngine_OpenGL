#pragma once

#include "SceneObject.h"

class HitboxVisualObject : public SceneObject {

public:
	HitboxVisualObject(Model* baseModel);

	void Update(float deltaTime) override;

	void Draw(Camera* camera) override;

private:

};