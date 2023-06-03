#pragma once 

#include "SceneObject.h"

class NormalsVisualObject : public SceneObject {

public:
	NormalsVisualObject(Model* baseModel);
	~NormalsVisualObject();

	void Update(float deltaTime) override;
};