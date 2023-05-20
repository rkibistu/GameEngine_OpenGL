#pragma once

#include "SceneObject.h"

class LightObject : public SceneObject {

public:
	enum Type {
		Point = 0,
		Directional,
		Spot
	};

public:
	LightObject();
	LightObject(Type type, Vector3 diffuseColor, Vector3 specularColor);

	inline Vector3 GetDiffuseColor() { return _diffuseColor; }
	inline Vector3 GetSpecularColor() { return _specularColor; }
	inline Type GetType() { return _type; }
private:
	Type _type;
	Vector3 _diffuseColor;
	Vector3 _specularColor;
};