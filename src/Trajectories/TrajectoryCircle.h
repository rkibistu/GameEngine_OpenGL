#pragma once

#include "Trajectory.h"

class TrajectoryCircle : public Trajectory {

public:
	void Start() override;
	void Update(float deltaTime, Vector3& position) override;

private:

	float _time;

	bool _init = false;
};