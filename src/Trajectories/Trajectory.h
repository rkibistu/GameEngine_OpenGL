#pragma once

#include <vector>
#include "Utilities/utilities.h"

class Trajectory {

public:
	enum Type {
		Linear = 0,
		LineStrip,
		LineLoop,
		Circle
	};
	enum Direction {
		Forward = 0,
		Alternate
	};

protected:
	Direction _direction;
	unsigned int _iterationCount;
	bool _iterationInfinity;
	float _speed;
	std::vector<Vector3> _checkpoints;
	Vector3 _center;
	unsigned int _radius;
	Vector3 _planeVector1;
	Vector3 _planeVector2;

public:
	Trajectory();

	virtual void Start() = 0;
	virtual void Update(float deltaTime, Vector3& position) = 0;

	inline void SetIterationInfinity(bool state) { _iterationInfinity = state; }
	inline void SetIterationCount(int count) { _iterationCount = count; }
	inline void SetDirection(Direction direction) { _direction = direction; }
	inline void SetSpeed(float speed) { _speed = speed; }
	void AddCheckpoint(Vector3 checkpoint);
	void AddCheckpoint(std::vector<Vector3> checkpoints);
	inline void SetCenter(Vector3 position) { _center = position; }
	inline void SetRadius(float radius) { _radius = radius; }
	inline void SetPlaneVectors(Vector3 v1, Vector3 v2) { _planeVector1 = v1; _planeVector2 = v2; }

	virtual void SetToStartPosition(Vector3& position) = 0;
};