#include "stdafx.h"
#include "HitboxVisualObject.h"
#include "Vertex.h"
#include "SceneManager.h"
#include "Vertex.h"


HitboxVisualObject::HitboxVisualObject(SceneObject* parent, Model* baseModel)
	: SceneObject(true) {

	SceneManager& scneneManager = SceneManager::GetInstance();

	_parent = parent;
	_model = baseModel;
	_wiredShader = scneneManager.GetWiredShader();
	_name = "aabb"; 
	_drawWired = true;
}
HitboxVisualObject::~HitboxVisualObject() {

}
void HitboxVisualObject::Update(float deltaTime) {

	SetPosition(_parent->GetPosition());
}

