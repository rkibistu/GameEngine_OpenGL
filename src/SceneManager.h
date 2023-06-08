#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include "SceneManagerXmlParser.h"
#include "Fog.h"
#include "Light.h"
#include "AmbientalLight.h"

#include <unordered_map>
#include <map>

struct DefaultSettings {

	Shader* DefaultShader;		//used to all objects that don't specify one
	Shader* WiredShader;		//used to draw all wired objects
	Shader* DebugLightShader;	//used to draw debug light source
	Model* DebugLightModel;		//used to draw debug light source
	Material* DefaultMaterial;	//used to all objects that don't specify one
};

class SceneManager {

public:
	static SceneManager& GetInstance();
	void DestroyInstance();

	void Init(ESContext* esContext);

	//called once per object, at the beggining
	void Start();

	void Update(ESContext* esContext, float deltaTime);

	void Draw(ESContext* esContext);

	inline Camera* GetActiveCamera() { return _activeCamera; }
	inline Fog& GetFog() { return _fog; }
	inline AmbientalLight& GetAmbientalLight() { return _ambientalLight; }

	inline LightObject* GetLight(unsigned int id) { return _lightObjects[id]; }
	inline std::unordered_map<unsigned int, LightObject*>& GetLights() { return _lightObjects; }

	inline SceneObject* GetSceneObject(unsigned int id) { return _sceneObjects[id]; }
	inline std::map<unsigned int, SceneObject*>& GetSceneObjects() { return _sceneObjects; }

	inline Material* GetDefaultMaterial() { return _defaultSettings.DefaultMaterial; }
	inline Shader* GetDefaultShader() { return _defaultSettings.DefaultShader; }
	inline Shader* GetWiredShader() { return _defaultSettings.WiredShader; }
	inline Model* GetDebugLightModel() { return _defaultSettings.DebugLightModel; }
	inline Shader* GetDebugLightShader() { return _defaultSettings.DebugLightShader; }

	inline void SetDebug(bool active) { _debugMode = active; }
	inline bool GetDebug() { return  _debugMode; }
private:

	void CreateDebugAxisObject();

	void UpdateDebugObjects(float deltaTime);

private:
	static SceneManager* _spInstance;
	SceneManager() { ; }
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator= (const SceneManager&) = delete;

	std::map<unsigned int, SceneObject*> _sceneObjects;
	std::unordered_map<unsigned int, SceneObject*> _debugObjects;
	std::unordered_map<unsigned int, LightObject*> _lightObjects;
	std::unordered_map<unsigned int, Camera*> _cameras;
	Camera* _activeCamera;

	bool _debugMode = false;

	Vector3 _backgroundColor;
	Fog _fog;
	AmbientalLight _ambientalLight;
	DefaultSettings _defaultSettings;

	SceneManagerXmlParser _xmlParser;
};