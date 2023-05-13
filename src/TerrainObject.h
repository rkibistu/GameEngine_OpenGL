#pragma once

#include "SceneObject.h"

class TerrainObject : public SceneObject {

public:
    TerrainObject(int size, int cellCount);
    TerrainObject(int size, int cellCount, Vector3 heights);

    void Update(float deltaTime) override;



private:
    void SetUniformsParticular(Camera* camera) override;

    Vector3 _deplasament;

    int _size;
    int _countCells;
    int _width;

    Vector3 _heigths;
};


