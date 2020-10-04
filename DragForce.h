#pragma once
#include "ForceGenerator.h"
class Physics;

class DragForce :
	public ForceGenerator
{
public:
	DragForce(float dragFactor, bool laminar);
	~DragForce();
	void DragLaminarFlow(Vector3D velocity, float dragFactor);
	void DragTurbulentFlow(Vector3D velocity, float dragFactor);
	void updateForce(Physics* physics, float time);
	void CheckSpeed(Vector3D velocity);

private:
	Vector3D unitVel;
	float _dragFactor, velMag;
	bool _laminar;
};

