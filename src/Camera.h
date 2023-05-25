#pragma once

#include "Utilities/utilities.h" 

class Camera
{

public:
	Camera() { ; }
	Camera(Vector3 position, Vector3 target, Vector3 up);
	
	void Init(Vector3 position, Vector3 target, Vector3 up, float fov, float nearClip, float farClip);

	void Update(float deltaTime);

	//Metode folosite pt a controla camera
	void DoFirstPersonMovement();
	void RotateUsingArrows();
	void RotateUsingMouse();

	void PrintInfo();

	//Astea iau in calcul _moveSpeed si _deltaTime
	void Move(Vector3 direction);
	void MoveOX(int diretion);
	void MoveOY(int diretion);
	void MoveOZ(int diretion);

	void Rotate(Vector3 direction);
	void RotateOX(int direction);
	void RotateOY(int direction);
	void RotateOZ(int direction);

	void SetPerspective(GLfloat fov, GLfloat aspectRatio, GLfloat nearClip, GLfloat farClip);

	inline void SetId(unsigned int id) { _ID = id; }
	inline unsigned int GetId() { return _ID; }

	inline void SetDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
	inline float GetDeltaTime() { return _deltaTime; }

	inline void SetMoveSpeed(float value) { _moveSpeed = value; }
	inline void SetRotationSpeed(float value) { _rotationSpeed = value; }

	inline Matrix GetMVP() { return _viewMatrix * _projMatrix; }
	//inline Matrix GetMVP() { return _projMatrix; }

	inline Vector3 GetPosition() { return _position; }

	inline Vector3 GetForward() { return _zAxis; }
	inline Vector3 GetRight() { return _xAxis; }
	inline Vector3 GetTarget() { return _target; }
	inline Vector3 GetUp() { return _up; }

private:
	
	Matrix _R;
	Matrix _T;

	Vector3 _localUp = Vector3(0.0f, 1.0f, 0.0f);

	void UpdateAxis();
	void UpdateWorldView();
	void UpdateWorldMatrix();
	void UpdateViewMatrix();


	void TranslateToOrigin();
	void TranslateToPosition(Vector3 position);
private:

	unsigned int _ID;

	Vector3 _position;
	Vector3 _target;
	Vector3 _up;

	float _moveSpeed;
	float _rotationSpeed;

	float _deltaTime;

	GLfloat _nearClip;
	GLfloat _farClip;
	GLfloat _fov;
	GLfloat _aspectRatio;

	Vector3 _xAxis;
	Vector3 _yAxis;
	Vector3 _zAxis;

	Matrix _worldMatrix; //g_camera transform
	Matrix _viewMatrix;; //inverse of world matrix

	Matrix _projMatrix;
};

