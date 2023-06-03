#include "stdafx.h"
#include "TrajectoryCircle.h"

void TrajectoryCircle::Start() {

	_time = 0.0f;
}
void TrajectoryCircle::Update(float deltaTime, Vector3& position) {

	_time += deltaTime;
	//position.x = _center.x + _radius * cos(_speed * _time);
	//position.y = _center.y + _radius * sin(_speed * _time);
	//position.z = _center.z + _radius * sin(_speed * _time);

	

	float angle = _speed * _time;
	Vector3 axis;
	Vector3 v1(0, 1, 1);
	Vector3 v2(1, 1, 0);
	axis = v1.Cross(v2);
	axis.Normalize();

	//DID IT!

	position.x = _center.x + _radius * (cos(_speed * _time) * v1.x + sin(_speed * _time) *v2.x);
	position.y = _center.y + _radius * (cos(_speed * _time) * v1.y + sin(_speed * _time) *v2.y);
	position.z = _center.z + _radius * (cos(_speed * _time) * v1.z + sin(_speed * _time) *v2.z);








	//if (!_init) {

	//	_init = true;
	//	position = position + v1 * _radius ;

	//}

	//angle = 0.01f;
	//float cosAngle = cos(angle);
	//float sinAngle = sin(angle);

	////angle = 45;

	//double rotationMatrix[3][3];
	//rotationMatrix[0][0] = cosAngle + (1 - cosAngle) * axis.x * axis.x;
	//rotationMatrix[0][1] = (1 - cosAngle) * axis.x * axis.y - sinAngle * axis.z;
	//rotationMatrix[0][2] = (1 - cosAngle) * axis.x * axis.z + sinAngle * axis.y;

	//rotationMatrix[1][0] = (1 - cosAngle) * axis.y * axis.x + sinAngle * axis.z;
	//rotationMatrix[1][1] = cosAngle + (1 - cosAngle) * axis.y * axis.y;
	//rotationMatrix[1][2] = (1 - cosAngle) * axis.y * axis.z - sinAngle * axis.x;

	//rotationMatrix[2][0] = (1 - cosAngle) * axis.z * axis.x - sinAngle * axis.y;
	//rotationMatrix[2][1] = (1 - cosAngle) * axis.z * axis.y + sinAngle * axis.x;
	//rotationMatrix[2][2] = cosAngle + (1 - cosAngle) * axis.z * axis.z;


	//Vector3 object;
	//
	//object.x = rotationMatrix[0][0] * position.x + rotationMatrix[0][1] * position.y + rotationMatrix[0][2] * position.z;
	//object.y = rotationMatrix[1][0] * position.x + rotationMatrix[1][1] * position.y + rotationMatrix[1][2] * position.z;
	//object.z = rotationMatrix[2][0] * position.x + rotationMatrix[2][1] * position.y + rotationMatrix[2][2] * position.z;


	//position = object;
}

