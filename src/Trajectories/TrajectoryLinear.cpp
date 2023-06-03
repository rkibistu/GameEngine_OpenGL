#include "stdafx.h"
#include "TrajectoryLinear.h"


TrajectoryLinear::TrajectoryLinear() {

	_direction = Direction::Forward;
	_iterationInfinity = true;
	_speed = 1.0f;
}




void TrajectoryLinear::Start() {

	_lastCheckpoint = 0;
	_nextCheckpoint = 1;
	CalculateDirection(_lastCheckpoint, _nextCheckpoint);

	_epsilonChecked.SetZero();
	_iterationIndex = 0;
}
void TrajectoryLinear::Update(float deltaTime, Vector3& position) {

	if (_finished)
		return;

	if (CheckTouchCheckpoint(position)) {

		_epsilonChecked.SetZero();


		IncrementIterationIndex();
		if (CheckForFinish() == true) {
			_finished = true;
			return;
		}

		PassToNextCheckpoint(position);
		CalculateDirection(_lastCheckpoint, _nextCheckpoint);
	}

	position += _currentDirection * deltaTime * _speed;
}

// PRIVATE
void TrajectoryLinear::CalculateDirection(unsigned int sourceIndex, unsigned int destIndex) {

	if (destIndex >= _checkpoints.size())
		return;

	_currentDirection = _checkpoints[destIndex] - _checkpoints[sourceIndex];
	_currentDirection.Normalize();
}
bool TrajectoryLinear::CheckTouchCheckpoint(Vector3 currentPosition) {

	Vector3 distance = currentPosition - _checkpoints[_nextCheckpoint];
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
bool TrajectoryLinear::CheckForFinish() {

	if (_iterationInfinity)
		return false;

	if (_iterationIndex >= _iterationCount) {

		return true;
	}

	return false;
}
void TrajectoryLinear::IncrementIterationIndex() {

	_iterationIndex++;
}
void TrajectoryLinear::PassToNextCheckpoint(Vector3& currentPosition) {

	if (_direction == Direction::Alternate) {

		Math::Swap(_lastCheckpoint, _nextCheckpoint);
	}
	currentPosition = _checkpoints[_lastCheckpoint];
}