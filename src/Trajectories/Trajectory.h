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
	//rotation plane

	int _lastCheckpoint;
	int _nextCheckpoint;
	Vector3 _currentDirection;
	int _iterationIndex;
	const unsigned int _epsilon = 5.0f;
	Vector3 _epsilonChecked;

	bool _finished = false;

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
};