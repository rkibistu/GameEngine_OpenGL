#pragma once

#include "SceneObject.h"

class TerrainObject : public SceneObject {

public:
    TerrainObject();
private:
    void GenerateFlatTerrain(float width, float depth, int numVerticesWidth, int numVerticesDepth, std::vector<float>& vertices, std::vector<unsigned int>& indices);
};

#include <vector>


