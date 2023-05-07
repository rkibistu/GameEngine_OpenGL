#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include "SceneManagerXmlParser.h"

#include <unordered_map>


class SceneManager {

public:

	static SceneManager& GetInstance();
	void DestroyInstance();

	void Init();

	void Draw(Camera* camera);

private:

	

private:
	static SceneManager* _spInstance;
	SceneManager() { ; }
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator= (const SceneManager&) = delete;

	std::unordered_map<unsigned int, SceneObject*> _sceneObjects;
	std::unordered_map<unsigned int, Camera*> _cameras;
	Camera* _activeCamera;

	SceneManagerXmlParser _xmlParser;

};