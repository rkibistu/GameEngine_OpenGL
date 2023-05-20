#include "stdafx.h"
#include "Light.h"

Light::Light() 
	: _type(Point){

	_diffuseColor = Vector3();
	_specularColor = Vector3();
}

Light::Light(Type type, Vector3 diffuseColor, Vector3 specularColor)
	: _type(type), _diffuseColor(diffuseColor), _specularColor(specularColor) {


}
