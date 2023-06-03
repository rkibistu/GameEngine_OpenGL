#pragma once

#include "TrajectoryLinear.h"



class TrajectoryLineLoop : public TrajectoryLinear {

public:
	void PassToNextCheckpoint(Vector3& currentPosition) override;

private:
	void IncrementIterationIndex() override;

private:

	int _movePass = 1;
	bool _lastLoop = false;
};