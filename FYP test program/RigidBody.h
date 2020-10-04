#pragma once
#include "Physics.h"
class RigidBody :
	public Physics

{
public:
	RigidBody(std::shared_ptr<Transform>  transform);
	~RigidBody();
	virtual void Update(float time);
	virtual void CalculateBounding(std::vector<XMFLOAT3> test, XMFLOAT3 scale);
	void SetMass(float mass) { _mass = mass; }
	float GetMass() {return _mass;}

private:
	std::shared_ptr<Transform>  _objectWorld;
	float _mass;
};

