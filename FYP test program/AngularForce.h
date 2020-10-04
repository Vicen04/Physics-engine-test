#pragma once
#include "ForceGenerator.h"
#include "Quaternion.h"

class AngularForce :
	public ForceGenerator
{
public:
	AngularForce(float mass, float x, float y, float z);
	~AngularForce();
	void updateForce(Physics *physics, float time);
	Vector3D Torque(Vector3D position, Vector3D force);
	XMFLOAT3X3 InverseMatrix_3X3(XMFLOAT3X3 matrix);
	void UpdateInertia(float mass, float x, float y, float z);

private:
	float _angularDamping = 0.7f;
	XMFLOAT3X3 _momentOfInertia;
	Quaternion _rotation;
};

