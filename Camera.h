#pragma once

#include "../Utilities/utilities.h" 

class Camera
{
private:

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
public:

	Camera(Vector3 position, Vector3 target, Vector3 up);


	void PrintInfo();

	void Move(Vector3 direction);
	void MoveOX(int diretion);
	void MoveOY(int diretion);
	void MoveOZ(int diretion);

	void Rotate(Vector3 direction);
	void RotateOX(int direction);
	void RotateOY(int direction);
	void RotateOZ(int direction);

	void SetPerspective(GLfloat fov, GLfloat aspectRatio, GLfloat nearClip, GLfloat farClip);

	inline void SetDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
	inline float GetDeltaTime() { return _deltaTime; }

	inline Matrix GetMVP() { return _viewMatrix * _projMatrix; }

private:
	
	Matrix _R;
	Matrix _T;

	Vector3 _localUp = Vector3(0.0f, 1.0f, 0.0f);

	void UpdateAxis();
	void UpdateWorldView();
	void UpdateWorldMatrix();
	void UpdateViewMatrix();
};

