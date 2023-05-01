#pragma once

#include "../../../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include


struct Vertex 
{
	Vector3 pos;
	Vector3 color;
	Vector2 uv;
	Vector3 norm;
	Vector3 binorm;
	Vector3 tgt;
};