#include "stdafx.h"
#include "Trajectory.h"

Trajectory::Trajectory() {
}

void Trajectory::AddCheckpoint(Vector3 checkpoint) {

	_checkpoints.push_back(checkpoint);
}
void Trajectory::AddCheckpoint(std::vector<Vector3> checkpoints) {

	for (auto it = checkpoints.begin(); it != checkpoints.end(); it++) {

		_checkpoints.push_back(*it);
	}
}