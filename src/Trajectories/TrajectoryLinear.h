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
	int _lastCheckpoint;
	int _nextCheckpoint;
	Vector3 _currentDirection;
	int _iterationIndex;
	const unsigned int _epsilon = 5.0f;
	Vector3 _epsilonChecked;

	bool _finished = false;


protected:

	void CalculateDirection(unsigned int sourceIndex, unsigned int destIndex);
	
	bool CheckTouchCheckpoint(Vector3 currentPosition);
	
	bool CheckForFinish();

	//increment index, the moment is diffrent for evry type
	virtual void IncrementIterationIndex();
	
	virtual void PassToNextCheckpoint(Vector3& currentPosition);


};