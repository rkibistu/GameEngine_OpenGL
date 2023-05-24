#pragma once


#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Material.h"
#include "Camera.h"

#include <vector>
#include <map>
#include "Utilities/utilities.h"

class SceneObject {

public:
	SceneObject(bool isDebugObj = false);
	virtual ~SceneObject();

	virtual void Update(float deltaTime);

	virtual void Draw(Camera* camera);
	virtual void DrawWired(Camera* camera);
	virtual void DrawDebug(Camera* camera);
	virtual void DrawDebugWired(Camera* camera);

	void UpdateDebugObjects(float deltaTime);
	void DrawDebugObjects(Camera* camera);

	void SetModel(Model* model);
	void SetShader(Shader* shader);
	void SetDebugShader(Shader* shader);
	void AddTexture(Texture* texture);
	void SetMaterial(Material* material);

	inline void SetId(unsigned int id) { _id = id; }
	inline unsigned int GetId() { return _id; }

	inline void SetName(std::string name) { _name = name; }
	inline std::string GetName() { return _name; }

	inline void SetPosition(Vector3 position) { _position = position; }
	inline void SetPosition(float v0, float v1, float v2) { _position.x = v0; _position.y = v1; _position.z = v2; }
	inline Vector3 GetPosition() { return _position; }

	inline Vector3 GetRotation() { return _rotation; }

	inline void SetRotation(Vector3 rotation) { _rotation = rotation; }

	inline void SetScale(Vector3 scale) { _scale = scale; }
	inline void SetScale(float v0, float v1, float v2) { _scale.x = v0, _scale.y = v1, _scale.z = v2; }
	inline Vector3 GetScale() { return _scale; }
	
	inline void SetFollowCameraDirections(Vector3 directions) { _followCameraDirections = directions; }
	inline void SetFollowCameraOffset(Vector3 offset) { _followCameraOffset = offset; }

	inline void SetDrawWired(bool state) { _drawWired = state; }
	inline bool GetDrawWired() { return _drawWired; }

	inline void SetParent(SceneObject* parent) { _parent = parent; }
protected:

	void FollowCamera();

	Matrix GetModelMatrix();

	void SetUniformsCommon(Camera* camera);
	virtual void SetUniformsParticular(Camera* camera);
	void SetUniformsCommonDebug(Camera* camera);

	virtual void CreateDebugObjects();


	void CopyParentTransform();

protected:
	unsigned int _id;
	std::string _name;

	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

	SceneObject* _parent;
	std::map<unsigned int, SceneObject*> _children;
	std::map<unsigned int, SceneObject*> _debugObjects;

	Model* _model;
	Shader* _shader;
	Shader* _debugShader; //used in wiredMode
	std::vector<Texture*> _textureResources;
	Material* _material;

	bool _depthTest;
	bool _drawWired;
	bool _isDebug;

	Vector3 _followCameraDirections;
	Vector3 _followCameraOffset;
};