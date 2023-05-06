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