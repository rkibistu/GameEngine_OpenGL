
#include "stdafx.h"
#include "TerrainObject.h"

TerrainObject::TerrainObject() {


}


void TerrainObject::GenerateFlatTerrain(float width, float depth, int numVerticesWidth, int numVerticesDepth, std::vector<float>& vertices, std::vector<unsigned int>& indices)
    {
    // Calculate the size of each grid cell
    float cellWidth = width / (numVerticesWidth - 1);
    float cellDepth = depth / (numVerticesDepth - 1);

    // Generate the vertices
    for (int dz = 0; dz < numVerticesDepth; ++dz)
    {
        for (int wx = 0; wx < numVerticesWidth; ++wx)
        {
            float x = wx * cellWidth;
            float z = dz * cellDepth;
            float y = 0.0f; // Flat terrain, y-coordinate is 0

            // Add vertex position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Add vertex normal (assuming a flat terrain, the normal is (0, 1, 0))
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);
        }
    }

    // Generate the indices
    for (int dz = 0; dz < numVerticesDepth - 1; ++dz)
    {
        for (int wx = 0; wx < numVerticesWidth - 1; ++wx)
        {
            int index = dz * numVerticesWidth + wx;

            // Add indices for the two triangles forming the quad
            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + numVerticesWidth);

            indices.push_back(index + 1);
            indices.push_back(index + 1 + numVerticesWidth);
            indices.push_back(index + numVerticesWidth);
        }
    }
    }