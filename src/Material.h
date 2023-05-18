#pragma once

#include <string>

#include "Utilities/utilities.h"

struct MaterialResource {

	unsigned int ID;
	std::string Path;

	std::string Type;

	std::string FactorTextura;
	std::string FactorReflexieSkybox;
};

class Material {

public:

	int Load(MaterialResource* materialResource);

	inline float GetFactorTextura() { return _factorTextura; }
	inline float GetFactorReflexieTextura() { return _factorReflexieTextura; }
private:
	MaterialResource* _materialResource;
	float _factorTextura;
	float _factorReflexieTextura;
};