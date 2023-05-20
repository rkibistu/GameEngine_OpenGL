#pragma once

#include "SceneObject.h"

class Light : public SceneObject {

public:
	enum Type {
		Point = 0,
	};

public:
	Light();
	Light(Type type, Vector3 diffuseColor, Vector3 specularColor);

	inline Vector3 GetDiffuseColor() { return _diffuseColor; }
	inline Vector3 GetSpecularColor() { return _specularColor; }

private:
	Type _type;
	Vector3 _diffuseColor;
	Vector3 _specularColor;
};