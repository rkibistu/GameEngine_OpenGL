#pragma once

#include "DebugObject.h"
#include "Light.h"

class DebugLightObject : public LightObject {

public:
	DebugLightObject();
	DebugLightObject(Type type, Vector3 diffuseColor, Vector3 specularColor);

	void Update(float deltaTime) override;
	void Draw(Camera* camera) override;
};