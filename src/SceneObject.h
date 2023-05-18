#pragma once


#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Material.h"
#include "Camera.h"

#include <vector>
#include "Utilities/utilities.h"

class SceneObject {

public:
	SceneObject();
	virtual ~SceneObject();

	virtual void Update(float deltaTime);

	void Draw(Camera* camera);
	void DrawWired(Camera* camera);

	void SetModel(Model* model);
	void SetShader(Shader* shader);
	void AddTexture(Texture* texture);
	void SetMaterial(Material* material);

	inline void SetId(unsigned int id) { _id = id; }
	inline unsigned int GetId() { return _id; }

	inline void SetName(std::string name) { _name = name; }
	inline std::string GetName() { return _name; }
	
	inline void SetPosition(Vector3 position) { _position = position; }
	inline void SetPosition(float v0, float v1, float v2) { _position.x = v0, _position.y = v1, _position.z = v2; }
	inline void SetRotation(Vector3 rotation) { _rotation = rotation; }
	inline void SetScale(Vector3 scale) { _scale = scale; }
	inline void SetFollowCameraDirections(Vector3 directions) { _followCameraDirections = directions; }
	inline void SetFollowCameraOffset(Vector3 offset) { _followCameraOffset = offset; }
protected:

	void FollowCamera();

	Matrix GetModelMatrix();
	
	void SetUniformsCommon(Camera* camera);
	virtual void SetUniformsParticular(Camera* camera);

protected:
	unsigned int _id;
	std::string _name;
	bool _depthTest;

	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

	Model* _model;
	Shader* _shader;
	std::vector<Texture*> _textureResources;
	Material* _material;

	//Material*

	Vector3 _followCameraDirections;
	Vector3 _followCameraOffset;
};