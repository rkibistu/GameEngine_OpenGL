#include "stdafx.h"
#include "DebugObject.h"

DebugObject::DebugObject() 
	: SceneObject(true) {

}

void DebugObject::Update(float deltaTime) {

	SetPosition(_parent->GetPosition());
	SetRotation(_parent->GetRotation());
}

