#pragma once

#include "SceneObject.h"

class TargetLineVisual : public SceneObject {

public:
	TargetLineVisual();

	void Update(float deltaTime) override;
};