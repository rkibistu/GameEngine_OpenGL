#pragma once

#include "SceneObject.h"

class TerrainObject : public SceneObject {

public:
    TerrainObject(int size, int cellCount);

    void Update(float deltaTime) override;



private:
    void SetUniformsParticular(Camera* camera) override;

    Vector3 _deplasament;
    Vector3 _cellsDepls;

    int _size;
    int _countCells;
    int _width;
};


