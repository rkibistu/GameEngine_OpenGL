#pragma once


#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Camera.h"

#include <vector>
#include "Utilities/utilities.h"

class SceneObject {

public:
	SceneObject();
	~SceneObject();

	void Draw(Camera* camera);
	void DrawWired(Camera* camera);

	void SetModel(Model* model);
	void SetShader(Shader* shader);
	void AddTexture(Texture* texture);

	inline void SetId(unsigned int id) { _id = id; }
	inline unsigned int GetId() { return _id; }
	
	inline void SetPosition(Vector3 position) { _position = position; }
	inline void SetRotation(Vector3 rotation) { _rotation = rotation; }
	inline void SetScale(Vector3 scale) { _scale = scale; }
private:

	Matrix GetModelMatrix();

private:
	unsigned int _id;
	bool _depthTest;

	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

	Model* _model;
	Shader* _shader;
	std::vector<Texture*> _textureResources;

};