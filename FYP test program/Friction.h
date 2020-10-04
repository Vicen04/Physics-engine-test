#pragma once
#include "ForceGenerator.h"
class Friction :
	public ForceGenerator
{
public:
	Friction(float frictionFactor);
	~Friction();
	void updateForce(Physics* particle, float time);
	void SetFrictionFactor(float newFriction) {	_frictionFactor = newFriction;}
	void Compare();
private:
	Vector3D velocity;
	float _frictionFactor;
};

