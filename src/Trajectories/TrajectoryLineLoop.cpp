#include "stdafx.h"
#include "TrajectoryLineLoop.h"

void TrajectoryLineLoop::PassToNextCheckpoint(Vector3& currentPosition) {

	std::cout << "L: " << _lastCheckpoint << "  N: " << _nextCheckpoint << "\n";

	if (_lastCheckpoint == 1 && _nextCheckpoint == 0)
		int x = 3;

	_lastCheckpoint += _movePass;
	_nextCheckpoint += _movePass;


	if (_lastLoop == false &&
		(_nextCheckpoint == _checkpoints.size() && _movePass == 1) ||
		(_nextCheckpoint == 0 && _movePass == -1)) {

		_lastLoop = true;
		_lastCheckpoint %= _checkpoints.size();
		_nextCheckpoint %= _checkpoints.size();
		std::cout << "L: " << _lastCheckpoint << "  N: " << _nextCheckpoint << "\n\n";
		currentPosition = _checkpoints[_lastCheckpoint];
		return;
	}

	if (_lastLoop == true) {

		if (_direction == Direction::Alternate) {

			_movePass = (_movePass == 1) ? -1 : 1;
			Math::Swap(_lastCheckpoint, _nextCheckpoint);
			_lastCheckpoint += _movePass;
			_nextCheckpoint += _movePass;

		}
		_lastLoop = false;
	}

	_lastCheckpoint = (_lastCheckpoint + _checkpoints.size()) % _checkpoints.size();
	_nextCheckpoint = (_nextCheckpoint + _checkpoints.size()) % _checkpoints.size();

	std::cout << "L: " << _lastCheckpoint << "  N: " << _nextCheckpoint << "\n\n";

	currentPosition = _checkpoints[_lastCheckpoint];
}

void TrajectoryLineLoop::IncrementIterationIndex() {

	if (_lastLoop == true) {

		_iterationIndex++;
		std::cout << "INCREMENT!\n";
	}
}