#pragma once 

#include "SceneObject.h"

class Fire: public SceneObject {

public:
	Fire(float dispersionMax = 1.0f);

	void Update(float deltaTime) override;

protected:
	void SetUniformsParticular(Camera* camera) override;

private:
	float _dispersionMax;
	float _time;
};