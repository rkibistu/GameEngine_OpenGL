#include "stdafx.h"

#include "Material.h"
#include "Defines.h"

int Material::Load(MaterialResource* materialResource) {

	_materialResource = materialResource;

	_factorTextura = atof(_materialResource->FactorTextura.c_str());
	_factorReflexieTextura = atof(_materialResource->FactorReflexieSkybox.c_str());

	return MY_SUCCES_CODE;
}