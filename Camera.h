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
	Matrix UpdateWorldView();


	void Move(Vector3 direction);
	void MoveOX(int diretion);
	void MoveOY(int diretion);
	void MoveOZ(int diretion);

	void RotateOY();

	void SetPerspective(GLfloat fov, GLfloat aspectRatio, GLfloat nearClip, GLfloat farClip);

	inline void SetDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
	inline float GetDeltaTime() { return _deltaTime; }

private:
	
	Matrix _R;
	Matrix _T;

	void UpdateAxis();
	void UpdateWorldMatrix();
	void UpdateViewMatrix();
};

