#pragma once

#include "CoordonatesAxeObject.h"

class LocalCoordonatesAxeObject : public CoordonateAxeObject {

public:
	
	void Update(float deltaTime) override;

	void Draw(Camera* camera) override;


};