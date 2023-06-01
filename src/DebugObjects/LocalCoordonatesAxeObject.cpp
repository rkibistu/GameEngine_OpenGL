#include "stdafx.h"
#include "LocalCoordonatesAxeObject.h"

void LocalCoordonatesAxeObject::Update(float deltaTime) {

	if (_parent == nullptr) {
		std::cout << "[ERROR] Parent is null for object: " << _name << std::endl;
		return;
	}

	SetPosition(_parent->GetPosition());
	SetRotation(_parent->GetRotation());
}

void LocalCoordonatesAxeObject::Draw(Camera* camera) {

	if (_drawWired) {
		DrawDebugWired(camera);
	}
}