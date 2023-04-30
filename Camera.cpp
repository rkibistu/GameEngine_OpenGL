#include "stdafx.h"
#include "Camera.h"

#include <iostream>

Camera::Camera(Vector3 position, Vector3 target, Vector3 up)
	: _position(position), _target(target), _up(up) {

	_moveSpeed = 1.0f;
	_rotationSpeed = 0.5f;

	_deltaTime = 1.0f;

	_nearClip = 0.1f;
	_farClip = 2000.0f;
	_fov = 45.0f;
	_aspectRatio = 800.0f / 600.0f;
	_projMatrix.SetPerspective(_fov, _aspectRatio, _nearClip, _farClip);

	UpdateAxis();
	UpdateWorldView();
}




void Camera::PrintInfo() {

	std::cout << "Pos: (" << _position.x << ", " << _position.y << ", " << _position.z << ") ";
	std::cout << "Up: (" << _up.x << ", " << _up.y << ", " << _up.z << ") " << std::endl;
}

void Camera::Move(Vector3 direction) {

	MoveOX(direction.x);
	MoveOY(direction.y);
	MoveOZ(direction.z);
}
void Camera::MoveOX(int direction) {
	if (direction == 0)
		return;

	Vector3 velocity = _xAxis * Math::Sign(direction) * _moveSpeed * _deltaTime;

	_position += velocity;
	_target += velocity;

	UpdateWorldView();
}
void Camera::MoveOY(int direction) {
	if (direction == 0)
		return;

	Vector3 velocity = _up * Math::Sign(direction) * _moveSpeed * _deltaTime;

	_position += velocity;
	_target += velocity;

	UpdateWorldView();
}
void Camera::MoveOZ(int direction) {
	if (direction == 0)
		return;

	Vector3 forward = -(_target - _position).Normalize() * Math::Sign(direction);
	Vector3 velocity = forward * _moveSpeed * _deltaTime;

	_position += velocity;
	_target += velocity;

	UpdateWorldView();
}





void Camera::Rotate(Vector3 direction) {

	RotateOX(direction.x);
	RotateOY(direction.y);
	RotateOZ(direction.z);
}
void Camera::RotateOX(int direction) {
	if (direction == 0)
		return;

	// go to center -> for rotation to be correct
	Vector3 goBackPos(_position);
	TranslateToOrigin();

	Matrix rotateOX;
	rotateOX.SetRotationX(_rotationSpeed * _deltaTime * Math::Sign(direction));

	Vector4 rotateLocalUp = Vector4(_localUp, 1.0f) * rotateOX;
	Vector4 up = (rotateLocalUp * _worldMatrix).Normalize();
	_up = Vector3(up.x, up.y, up.z);

	Vector4 localTarget(0.0f, 0.0f, -(_target - _position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * rotateOX;
	Vector4 target = rotatedTarget * _worldMatrix;
	_target = Vector3(target.x, target.y, target.z);

	UpdateWorldView();


	// go back to initial pos before starting rotation
	TranslateToPosition(goBackPos);
}
void Camera::RotateOY(int direction) {
	if (direction == 0)
		return;

	// go to center -> for rotation to be correct
	Vector3 goBackPos(_position);
	TranslateToOrigin();

	Vector4 localTarget(0.0f, 0.0f, -(_target - _position).Length(), 1.0f);

	Matrix rotateOY;
	rotateOY.SetRotationY(_rotationSpeed * _deltaTime * Math::Sign(direction));

	Vector4 rotatedTarget = localTarget * rotateOY;

	Vector4 temp = rotatedTarget * _worldMatrix;
	_target.x = temp.x;
	_target.y = temp.y;
	_target.z = temp.z;

	UpdateWorldView();

	// go back to initial pos before starting rotation
	TranslateToPosition(goBackPos);
}
void Camera::RotateOZ(int direction) {
	if (direction == 0)
		return;

	// go to center -> for rotation to be correct
	Vector3 goBackPos(_position);
	TranslateToOrigin();

	std::cout << "RESET: " << _position.x << " " << _position.y << " " << _position.z << std::endl;


	Matrix rotateOZ;
	rotateOZ.SetRotationZ(_rotationSpeed * _deltaTime * Math::Sign(direction));

	Vector4 rotateLocalUp = Vector4(_localUp, 1.0f) * rotateOZ;
	Vector4 up = (rotateLocalUp * _worldMatrix).Normalize();
	_up = Vector3(up.x, up.y, up.z);

	Vector4 localTarget(0.0f, 0.0f, -(_target - _position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * rotateOZ;
	Vector4 target = rotatedTarget * _worldMatrix;
	_target = Vector3(target.x, target.y, target.z);

	UpdateWorldView();

	// go back to initial pos before starting rotation
	TranslateToPosition(goBackPos);
}

void Camera::SetPerspective(GLfloat fov, GLfloat aspectRatio, GLfloat nearClip, GLfloat farClip) {

	_fov = fov;
	_nearClip = nearClip;
	_farClip = farClip;
	_aspectRatio = aspectRatio;

	_projMatrix.SetPerspective(_fov, _aspectRatio, _nearClip, _farClip);
}

void Camera::UpdateAxis() {

	_yAxis = _up.Normalize();
	_zAxis = -(_target - _position).Normalize();
	_xAxis = (_zAxis.Cross(_yAxis)).Normalize();
}
void Camera::UpdateWorldView() {

	UpdateAxis();
	UpdateWorldMatrix();
	UpdateViewMatrix();
}
void Camera::UpdateWorldMatrix() {

	//rotation matrix
	_R.m[0][0] = _xAxis.x;
	_R.m[0][1] = _xAxis.y;
	_R.m[0][2] = _xAxis.z;
	_R.m[0][3] = 0;

	_R.m[1][0] = _yAxis.x;
	_R.m[1][1] = _yAxis.y;
	_R.m[1][2] = _yAxis.z;
	_R.m[1][3] = 0;

	_R.m[2][0] = _zAxis.x;
	_R.m[2][1] = _zAxis.y;
	_R.m[2][2] = _zAxis.z;
	_R.m[2][3] = 0;

	_R.m[3][0] = 0;
	_R.m[3][1] = 0;
	_R.m[3][2] = 0;
	_R.m[3][3] = 1;

	//transpose matrix
	_T.SetTranslation(_position.x, _position.y, _position.z);

	_worldMatrix = _R * _T;
}
void Camera::UpdateViewMatrix() {

	//rotation matrix
	_R.m[0][0] = _xAxis.x;
	_R.m[1][0] = _xAxis.y;
	_R.m[2][0] = _xAxis.z;
	_R.m[3][0] = 0;

	_R.m[0][1] = _yAxis.x;
	_R.m[1][1] = _yAxis.y;
	_R.m[2][1] = _yAxis.z;
	_R.m[3][1] = 0;

	_R.m[0][2] = _zAxis.x;
	_R.m[1][2] = _zAxis.y;
	_R.m[2][2] = _zAxis.z;
	_R.m[3][2] = 0;

	_R.m[0][3] = 0;
	_R.m[1][3] = 0;
	_R.m[2][3] = 0;
	_R.m[3][3] = 1;

	//transpose matrix
	_T.SetTranslation(-_position.x, -_position.y, -_position.z);

	_viewMatrix = _T * _R;
}

void Camera::SetPosition(Vector3 position, Vector3 target) {

	_position = position;
	_target = _position;

	UpdateWorldView();
}
void Camera::TranslateToOrigin() {


	_target -= _position;
	_position -= _position;

	UpdateWorldView();
}
void Camera::TranslateToPosition(Vector3 position) {

	TranslateToOrigin();

	_target += position;
	_position += position;

	UpdateWorldView();
}