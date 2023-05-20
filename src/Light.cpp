#include "stdafx.h"
#include "Light.h"

LightObject::LightObject() 
	: _type(Point){

	_diffuseColor = Vector3();
	_specularColor = Vector3();
}

LightObject::LightObject(Type type, Vector3 diffuseColor, Vector3 specularColor)
	: _type(type), _diffuseColor(diffuseColor), _specularColor(specularColor) {


}
