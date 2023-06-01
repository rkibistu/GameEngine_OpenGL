#pragma once 

#include "SceneObject.h"

class NormalsVisualObject : public SceneObject {

public:
	NormalsVisualObject(Model* baseModel);

	void Update(float deltaTime) override;

	void Draw(Camera* camera) override;

};