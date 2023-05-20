#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include "SceneManagerXmlParser.h"
#include "Fog.h"
#include "Light.h"

#include <unordered_map>
#include <map>


class SceneManager {

public:

	static SceneManager& GetInstance();
	void DestroyInstance();

	void Init(ESContext* esContext);

	void Update(ESContext* esContext, float deltaTime);

	void Draw(ESContext* esContext);

	inline Camera* GetActiveCamera() { return _activeCamera; }
	inline Fog& GetFog() { return _fog; }
private:

	

private:
	static SceneManager* _spInstance;
	SceneManager() { ; }
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator= (const SceneManager&) = delete;

	//orderrede
	std::map<unsigned int, SceneObject*> _sceneObjects;
	std::unordered_map<unsigned int, Light*> _lightObjects;
	std::unordered_map<unsigned int, Camera*> _cameras;
	Camera* _activeCamera;

	Vector3 _backgroundColor;
	Fog _fog;

	SceneManagerXmlParser _xmlParser;
};