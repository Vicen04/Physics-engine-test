#pragma once
#include "RigidBody.h"

class SoftBody :
	public RigidBody
{
public:
	SoftBody(std::shared_ptr<Transform>  transform);
	~SoftBody();
	void SetSoftBody();
	void Update();
	void Collision();
	void UpdateForces();
	void SetMass(float mass) { _mass = mass; }
	float GetMass() { return _mass; }

private:
	float _mass;
};

