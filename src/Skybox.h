#pragma once

#include "SceneObject.h"

class Skybox : public SceneObject {

public:
	Skybox();
	void Update(float deltaTime) override;

};