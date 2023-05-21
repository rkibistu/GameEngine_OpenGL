#pragma once

#include "DebugObject.h"

class DebugNormalsObject : public DebugObject {

public:
	DebugNormalsObject(std::vector<Vertex>& vertices);
	~DebugNormalsObject();
};