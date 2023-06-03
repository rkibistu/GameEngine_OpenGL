#pragma once

#include "Trajectory.h"

#include <vector>

#include "Utilities/utilities.h"

class TrajectoryLinear : public Trajectory {


public:

	TrajectoryLinear();

	void Start() override;
	void Update(float deltaTime, Vector3& position) override;

protected:

	void CalculateDirection(unsigned int sourceIndex, unsigned int destIndex);
	
	bool CheckTouchCheckpoint(Vector3 currentPosition);
	
	bool CheckForFinish();

	//increment index, the moment is diffrent for evry type
	virtual void IncrementIterationIndex();
	
	virtual void PassToNextCheckpoint(Vector3& currentPosition);


};