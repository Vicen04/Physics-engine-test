#pragma once
#include "ForceGenerator.h"
class Gravity :
	public ForceGenerator
{
public:
	Gravity(XMFLOAT3 gravity);
	~Gravity();

	void updateForce(Physics *physics, float time);
};

