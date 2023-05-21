#pragma once

#include "DebugObject.h"

class DebugAxisObject : public DebugObject {

public:
	DebugAxisObject();

	void Draw(Camera* camera) override;
};