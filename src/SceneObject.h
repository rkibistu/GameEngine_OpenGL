#pragma once


#include "Model.h"
#include "Texture.h"
#include "Shaders.h"

#include <vector>

class SceneObject {

public:

private:
	unsigned int _id;

	Model* _model;
	Shader* _shader;
	std::vector<Texture*> _textures;
};