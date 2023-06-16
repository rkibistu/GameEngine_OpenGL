#include "stdafx.h"
#include "TrajectoryCircle.h"

void TrajectoryCircle::Start() {

	_time = 0.0f;
}
void TrajectoryCircle::Update(float deltaTime, Vector3& position) {

	_time += deltaTime;

	float angle = _speed * _time;
	Vector3 axis;
	Vector3 v1 = _planeVector1;
	Vector3 v2 = _planeVector2;
	v1.Normalize();
	v2.Normalize();


	position.x = _center.x + _radius * (cos(_speed * _time) * v1.x + sin(_speed * _time) * v2.x);
	position.y = _center.y + _radius * (cos(_speed * _time) * v1.y + sin(_speed * _time) * v2.y);
	position.z = _center.z + _radius * (cos(_speed * _time) * v1.z + sin(_speed * _time) * v2.z);
}
void TrajectoryCircle::SetToStartPosition(Vector3& position) {
	position = _center;
}

