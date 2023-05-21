#pragma once

#include "SceneObject.h"

class DebugObject : public SceneObject {

public:
	DebugObject();

	virtual void Update(float deltaTime) override;
};