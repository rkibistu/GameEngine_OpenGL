#pragma once

#include "SceneObject.h"

//folosita doar pentru a afisa aabb-ul obiectelor
//	foloeste modelul creat in CollisionController, specific fiecarui sceneObject care are aabb

class HitboxVisualObject : public SceneObject {

public:
	//params: obeictul parinte, pointer spre modelul aabb specifc paritnelui
	HitboxVisualObject(SceneObject* parent, Model* baseModel);
	~HitboxVisualObject();

	void Update(float deltaTime) override;
private:

};