#pragma once

#include "TrajectoryLinear.h"



class TrajectoryStripLine : public TrajectoryLinear {

public:
	void PassToNextCheckpoint(Vector3& currentPosition) override;

private:
	void IncrementIterationIndex() override;

private:

	int _movePass = 1;
};