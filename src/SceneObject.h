#pragma once


#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Material.h"
#include "Camera.h"
#include "Trajectory.h"
#include "CollisionController.h"

#include <vector>
#include <map>
#include "Utilities/utilities.h"

class SceneObject {

public:
	SceneObject(bool isDebugObj = false);
	virtual ~SceneObject();

	virtual void Start();

	virtual void Update(float deltaTime);

	//Deseneaza triunghiuri folsoind _shader
	virtual void Draw(Camera* camera);

	//itereaza prin obiectele de debug si apelaza DrawDebug pt ele. Apelat in sceneManager
	virtual void UpdateDebugObjects(float deltaTime);

	//itereaza prin obiectele de debug si apelaza UpdateDebug pt ele. Apelat in sceneManager
	void DrawDebugObjects(Camera* camera);

	//called by CollisionController on specific moemnts (sugestive from names)
	virtual void OnCollisionEnter(SceneObject* collisionObj);
	virtual void OnCollisionStay(SceneObject* collisionObj);
	virtual void OnCollisionExit(SceneObject* collisionObj);

	void SetModel(Model* model);
	inline Model* GetModel() { return _model; }
	void SetShader(Shader* shader);
	void SetDebugShader(Shader* shader);
	void AddTexture(Texture* texture);
	void SetMaterial(Material* material);

	Matrix GetModelMatrix();

	//Va face ca acest obiect sa urmareasca camera pe axele care au valorea 1 in vectorul _followCameraDirections;
	//	cu un offset fata de camera cu valoarea de: _followCameraOffset;
	//	Trb apelata in Update
	void FollowCamera();

	//This is not generalized, if you change window size -> not working
	void StayOnSreen();

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

	void SetTrajectory(Trajectory* trajectory);
	inline bool IsCollidable() { return _collidable; }
	inline CollisionController* GetCollisionController() { return _collisionController; }
protected:


	void SetUniformsCommon(Camera* camera);
	virtual void SetUniformsParticular(Camera* camera);

	//creeaza obiecte de debug comune tutor obiectelor (axele de coordonote)
	virtual void CreateDebugObjects();

private:

	//Deseneaza triunghiuri folsoind _shader
	void DrawTriangles(Camera* camera);

	//Deseneaza linii folsoind _wiredShader
	void DrawWired(Camera* camera);

protected:
	unsigned int _id;
	std::string _name;

	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

	Vector3 _oldPosition;
	Vector3 _oldScale;
	Vector3 _oldRotation;

	SceneObject* _parent;
	std::map<unsigned int, SceneObject*> _children;
	std::map<unsigned int, SceneObject*> _debugObjects;

	Model* _model;
	Shader* _shader;
	Shader* _wiredShader; //used in wiremode
	std::vector<Texture*> _textureResources;
	Material* _material;

	Trajectory* _trajectory;
	CollisionController* _collisionController;
	bool _collidable;

	bool _depthTest;
	bool _drawWired;
	bool _isDebug;

	Vector3 _followCameraDirections;
	Vector3 _followCameraOffset;
};