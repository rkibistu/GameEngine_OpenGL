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
public:
	Trajectory(Type type, float speed, Direction direction = Forward);
	Trajectory(Type type, float speed, unsigned int iterationCount, Direction direction = Forward);
	Trajectory(Type type, float speed, Vector3 center, unsigned int radius);
	Trajectory(Type type, float speed, Vector3 center, unsigned int radius, unsigned int iterationCount);

	void AddCheckpoint(Vector3 checkpoint);
	void AddCheckpoint(std::vector<Vector3> checkpoints);


	void Update(float deltaTime, Vector3& position);

private:
	Type _type;
	Direction _direction;
	unsigned int _iterationCount;
	bool _iterationInfinity;
	float _speed;
	std::vector<Vector3> _checkpoints;
	Vector3 _center;
	unsigned int _radius;
	//rotation plane

	unsigned int _lastCheckpoint;
	Vector3 _currentDirection;
	const unsigned int _epsilon = 5.0f;
	Vector3 _epsilonChecked;
	
	bool _init = false;
	bool _finished = false;

private:

	void CalculateDirection();
	bool CheckTouchCheckpoint(Vector3 currentPosition);
	void PassToNextCheckpoint(Vector3& currentPosition);
};