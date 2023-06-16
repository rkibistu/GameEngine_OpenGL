#pragma once

#include "Model.h"

#include <map>

#include "Utilities/utilities.h"

class SceneObject;

class CollisionControllerBase {

public:

	virtual ~CollisionControllerBase() { ; }

	void Update(float deltaTime);

protected:

	//this is first method called in udpate every frame
	virtual void PreUpdate() { ; }

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

	//Doar apeleaza OnCollisionEnter/Stay/Exit pt obiectul de care e atasat/camera
	virtual void CallOnCollisionEnter(SceneObject* collisionObj) = 0;
	virtual void CallOnCollisionStay(SceneObject* collisionObj) = 0;
	virtual void CallOnCollisionExit(SceneObject* collisionObj) = 0;

	//verifica daca s-a schimbat pozitia/rotatia/scala de la ultimul frame
	virtual bool ModelMatrixChanged() = 0;

	//actualizeaza valorile pentru aabbCollider din cadrul modelului _aabbModel
	//	actualizeaza si aabbColidderWolrdSpace
	virtual void UpdateAabbColliderValues() = 0;

	//returneaza true daca boiectul de care e atasat cotnroller e collidable
	//false altfel
	virtual bool IsCollidable() = 0;

	//returneaza true daca other este acelasi cu obiectul de care e atasat controllerul
	virtual bool IsSelf(SceneObject* other) = 0;

protected:
	//folosite sa vedem cand s-a realizat o miscare
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