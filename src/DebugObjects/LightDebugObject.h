#pragma once 

#include "Light.h"

class LightDebugObject : public LightObject {

public:

	LightDebugObject(LightObject* parent);

	void Update(float deltaTime) override;

	void SetUniformsParticular(Camera* camera) override;
};