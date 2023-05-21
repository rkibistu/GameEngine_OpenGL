#include "stdafx.h"
#include "DebugAxisObject.h"
#include "ResourceManager.h"

DebugAxisObject::DebugAxisObject() {

	ResourceManager& resourceManager = ResourceManager::GetInstance();

	SetModel(resourceManager.GetSystemAxisModel());
	SetName("axis");
	SetScale(10.0f, 10.0f, 10.0f);
}

