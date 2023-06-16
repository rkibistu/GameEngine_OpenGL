#include "stdafx.h"
#include "Skybox.h"

Skybox::Skybox() {

	_collidable = false;
}

void Skybox::Update(float deltaTime) {

	FollowCamera();
}