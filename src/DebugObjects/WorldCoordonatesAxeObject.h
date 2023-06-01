#pragma once

#include "CoordonatesAxeObject.h"

class WorldCoordonatesAxeObject : public CoordonateAxeObject {

public:

	WorldCoordonatesAxeObject();

	void Update(float deltaTime) override;

	void Draw(Camera* camera) override;


};