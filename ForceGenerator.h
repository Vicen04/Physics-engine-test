#pragma once
#include "Transform.h"

class Physics;

class ForceGenerator
{
public:

	virtual void updateForce(Physics* physics, float time) = 0;
	void SetForce(float x, float y, float z) { _Force.SetVector3D(x, y, z); }
	void SetForce(XMFLOAT3 force) { _Force.SetVector3D(force); }

	Vector3D GetForce() { return _Force; }
	Vector3D GetVelocity() { return _velocity; }
	void SetVelocity(float x, float y, float z) { _velocity.SetVector3D(x, y, z); }
	void SetVelocity(XMFLOAT3 speed) { _velocity.SetVector3D(speed); }

	Vector3D GetAcceleration() { return _acceleration; }
	void SetAcceleration(float x, float y, float z) { _acceleration.SetVector3D(x, y, z); }
	void SetAcceleration(XMFLOAT3 acceleration) { _acceleration.SetVector3D(acceleration); }

	void SetStartTime(float time) { start = time; }
	void SetCurrTime(float time) { current = time; }
	float GetStartTime() { return start; }
	float GetCurrTime() { return current; }

private:

	Vector3D _Force, _velocity, _acceleration;
	float start = 0.0f, current = 0.0f;
};