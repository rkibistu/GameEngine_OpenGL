#pragma once

#include "Model.h"

#include <map>

#include "Utilities/utilities.h"


class SceneObject;


class CollisionController {

public:
	CollisionController(SceneObject* sceneObj, Model* baseModel);
	~CollisionController();

	void Start();
	void Update(float deltaTime);

	inline Model* GetAabbModel() { return _aabbModel; }
private:
	//apelata pt obiecetele in miscare
	//verifica daca obiectul curent a atins alt obiect din scena
	void CheckColiisions();

	//apelata in CheckCollisions
	//apelata in momentul unei coliziuni pt obiectele in miscare
	//	gestioneaza structura de tip  map ce tine evidenta coliziunilor
	//	apeleaza functia specifica de coliziune (enter sau Stay) din sceneObject
	void CallCollisionMethods(SceneObject* collisionObj);

	//apelata in CheckCollisions
	//apelata cand nu este detectata o colziune pt un obiect in miscare
	//	verifica daca in ultimul frame coliziunea a existat
	//	in caz afirmativ apeleaza functia specifica de exit pentru sceneObject
	void CallExitCollisionMethids(SceneObject* collisionObj);

	//apelata doar cand nu are loc miscare, in Update
	//apeleaza OnCollisionStay pentru toate obiectele cu care avem coliziuni active
	//			= pt toate din _isColliding
	// nu face verificari suplimentare
	void CallStayCollisionMethods();

	//verifica daca s-a schimbat pozitia/rotatia/scala de la ultimul frame
	bool ModelMatrixChanged();

	//actualizeaza valorile pentru aabbCollider din cadrul modelului _aabbModel
	//	actualizeaza si aabbColidderWolrdSpace
	void UpdateAabbColliderValues();
private:

	//obiectul parinte
	SceneObject* _sceneObject;

	Vector3 _oldPosition;
	Vector3 _oldScale;
	Vector3 _oldRotation;

	//modelul aabb, centrat in origine.
	//	Este folosit si pentru afisarea in HitboxVisualObject
	Model* _aabbModel;

	//AabbCollider convertit in WorldSpace.
	//		este folosit pentru a verifica coliziunile
	Model::AabbCollider* _aabbColliderWorldSpace;

	//here we keep all the objects we collided with
	std::map<unsigned int, bool> _isColliding;
};