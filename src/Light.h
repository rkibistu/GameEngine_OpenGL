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
	LightObject(bool isDebug = false);
	LightObject(Type type, Vector3 diffuseColor, Vector3 specularColor, bool isDebug = false);

	//update all the debug objects
	void UpdateDebugObjects(float deltaTime) override;

	inline Vector3 GetDiffuseColor() { return _diffuseColor; }
	inline Vector3 GetSpecularColor() { return _specularColor; }
	inline Type GetType() { return _type; }
protected:

	void SetUniformsParticular(Camera* camera) override;
	
	//Create debug objects specific to lights
	void CreateDebugObjects() override;

protected:
	Type _type;
	Vector3 _diffuseColor;
	Vector3 _specularColor;
};