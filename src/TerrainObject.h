#pragma once

#include "SceneObject.h"

class TerrainObject : public SceneObject {

public:
    TerrainObject();

    void Update(float deltaTime) override;
private:
};


