#include "stdafx.h"

#include "TrajectoryLineStrip.h"

void TrajectoryStripLine::PassToNextCheckpoint(Vector3& currentPosition) {

	_lastCheckpoint += _movePass;
	_nextCheckpoint += _movePass;

	if (_nextCheckpoint >= _checkpoints.size() || _nextCheckpoint < 0) {

		//suntem la ultimul punct din lista
		if (_direction == Direction::Alternate) {
			
			_movePass = (_movePass == 1) ? -1 : 1;
			Math::Swap(_lastCheckpoint, _nextCheckpoint);
			_lastCheckpoint += _movePass;
			_nextCheckpoint += _movePass;

		}
		else if (_direction == Direction::Forward) {

			_lastCheckpoint = (_lastCheckpoint + _checkpoints.size() - 1) % (_checkpoints.size() - 1);
			_nextCheckpoint = (_nextCheckpoint + _checkpoints.size() - 1) % (_checkpoints.size() - 1);
		}
	}

	currentPosition = _checkpoints[_lastCheckpoint];
}

void TrajectoryStripLine::IncrementIterationIndex() {

	if (_nextCheckpoint == _checkpoints.size() - 1 || _nextCheckpoint == 0)
		_iterationIndex++;
}