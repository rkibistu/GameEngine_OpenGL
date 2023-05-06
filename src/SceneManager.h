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

private:

	

private:
	static SceneManager* _spInstance;
	SceneManager() { ; }
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator= (const SceneManager&) = delete;

	std::unordered_map<unsigned int, SceneObject*> _sceneObjects;

	SceneManagerXmlParser _xmlParser;

};