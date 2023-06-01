#include "stdafx.h"
#include "Trajectory.h"

Trajectory::Trajectory(Type type, float speed, Direction direction)
	 : _type(type), _speed(speed), _direction(direction) {

	_lastCheckpoint = 0;
	_iterationInfinity = true;
}	

Trajectory::Trajectory(Type type, float speed, unsigned int iterationCount, Direction direction)
	: Trajectory(type,speed,direction) {

	_iterationCount = iterationCount;
	_iterationInfinity = false;
}

Trajectory::Trajectory(Type type, float speed, Vector3 center, unsigned int radius)
	: _type(type),_speed(speed),_center(center),_radius(radius) {

	_direction = Direction::Forward;
	_iterationInfinity = true;
}

Trajectory::Trajectory(Type type, float speed, Vector3 center, unsigned int radius, unsigned int iterationCount)
	: Trajectory(type,speed,center,radius) {

	_iterationCount = iterationCount;
	_iterationInfinity = false;
}

void Trajectory::AddCheckpoint(Vector3 checkpoint) {

	_checkpoints.push_back(checkpoint);
}
void Trajectory::AddCheckpoint(std::vector<Vector3> checkpoints) {

	for (auto it = checkpoints.begin(); it != checkpoints.end(); it++) {

		_checkpoints.push_back(*it);
	}
}



void Trajectory::Update(float deltaTime, Vector3& position) {

	if (!_init) {

		_init = true;
		_lastCheckpoint = 0;
		_nextCheckpoint = 1;
		CalculateDirection(_lastCheckpoint, _nextCheckpoint);
	}

	if (_finished)
		return;

	if (CheckTouchCheckpoint(position)) {

		PassToNextCheckpoint(position);
		CalculateDirection(_lastCheckpoint,_nextCheckpoint);
	}
	
	position += _currentDirection * deltaTime * _speed;
}

// PRIVATE
void Trajectory::PassToNextCheckpoint(Vector3& currentPosition) {

	_lastCheckpoint++;
	_nextCheckpoint++;
	_epsilonChecked.SetZero();

	if (_type == Type::Linear) {

		if (_nextCheckpoint == _checkpoints.size()) {
			std::cout << "END OF TRAJECTORY\n";
			_finished = true;
		}
	}
	else if (_type == Type::LineStrip) {

		_lastCheckpoint %= (_checkpoints.size() - 1);
		_nextCheckpoint %= (_checkpoints.size() - 1);
	}

	currentPosition = _checkpoints[_lastCheckpoint];

}
void Trajectory::CalculateDirection(unsigned int sourceIndex, unsigned int destIndex) {

	if (destIndex >= _checkpoints.size())
		return;

	_currentDirection = _checkpoints[destIndex] - _checkpoints[sourceIndex];
	_currentDirection.Normalize();
}
bool Trajectory::CheckTouchCheckpoint(Vector3 currentPosition) {

	Vector3 distance = currentPosition - _checkpoints[_lastCheckpoint + 1];
	if (Math::Abs(distance.x) <= _epsilon) {
		_epsilonChecked.x = 1;
	}
	if (Math::Abs(distance.y) <= _epsilon) {
		_epsilonChecked.y = 1;
	}
	if (Math::Abs(distance.z) <= _epsilon) {
		_epsilonChecked.z = 1;
	}
	if (_epsilonChecked.x == 1 && _epsilonChecked.y == 1 && _epsilonChecked.z == 1)
		return true;

	return false;
}