#include "stdafx.h"

#include "WorldCoordonatesAxeObject.h"

WorldCoordonatesAxeObject::WorldCoordonatesAxeObject() {

	_name = "sceneAxis";
	SetScale(0.1f, 0.1f, 0.1f);
}

void WorldCoordonatesAxeObject::Update(float deltaTime) {

	StayOnSreen();
}
