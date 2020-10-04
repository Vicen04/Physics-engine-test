#pragma once
#include "ForceGenerator.h"
class BasicForce :
	public ForceGenerator
{
public:
	BasicForce();
	~BasicForce();
	void updateForce(Physics* physics, float time);

};

