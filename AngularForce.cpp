#include "AngularForce.h"
#include "Physics.h"

AngularForce::AngularForce(float mass, float x, float y, float z)
{
	UpdateInertia(mass, x, y, z);
}

AngularForce::~AngularForce()
{

}

void AngularForce::updateForce(Physics *physics, float time)
{
	SetAcceleration(Torque(physics->GetHitPoint(), physics->GetForce("angular")->GetForce()).MulpitplyByMatrix_3X3(InverseMatrix_3X3(_momentOfInertia)));
	SetVelocity(GetAcceleration().MultiplyByFloat(time));
	//damping
	SetVelocity(GetVelocity().AddVector(GetVelocity().MultiplyByFloat(pow(_angularDamping, time))));
	//orientation
	_rotation.addScaledVector(GetVelocity(), time);
	_rotation.normalise();
	//physics->SetRotation(XMFLOAT3());

	if (physics->GetType() == "SoftBody")
	{
		//UpdateInertia(physics->GetMass(), physics->GetBoundaries().x, physics->GetBoundaries().y, physics->GetBoundaries().z);
	}

}

Vector3D AngularForce::Torque(Vector3D position, Vector3D force)
{
	return position.CrossProduct(force.GetVector3D());
}

void AngularForce::UpdateInertia(float mass, float x, float y, float z)
{
	_momentOfInertia._11 = mass * (y * y + z * z);
	_momentOfInertia._12 = -mass *  (y + x);
	_momentOfInertia._13 = -mass *  (x + z);
	_momentOfInertia._21 = -mass *  (y + x);
	_momentOfInertia._22 = mass * (x * x + z * z);
	_momentOfInertia._23 = -mass *  (y + z);
	_momentOfInertia._31 = -mass *  (z + x);
	_momentOfInertia._32 = -mass *  (y + z);
	_momentOfInertia._33 = mass * (y * y + x * x);
}

XMFLOAT3X3 AngularForce::InverseMatrix_3X3(XMFLOAT3X3 matrix)
{
	// get the determinant for every 2x2 in the matrix
	XMFLOAT3X3 inverseMatrix = XMFLOAT3X3();
	inverseMatrix._11 = matrix._22 * matrix._33 - matrix._23 * matrix._32;
	inverseMatrix._12 = matrix._21 * matrix._33 - matrix._23 * matrix._31;
	inverseMatrix._13 = matrix._21 * matrix._32 - matrix._22 * matrix._31;
	inverseMatrix._21 = matrix._12 * matrix._33 - matrix._13 * matrix._32;
	inverseMatrix._22 = matrix._11 * matrix._33 - matrix._13 * matrix._31;
	inverseMatrix._23 = matrix._11 * matrix._32 - matrix._12 * matrix._31;
	inverseMatrix._31 = matrix._12 * matrix._23 - matrix._13 * matrix._22;
	inverseMatrix._32 = matrix._11 * matrix._23 - matrix._13 * matrix._21;
	inverseMatrix._33 = matrix._11 * matrix._22 - matrix._12 * matrix._21;

	// 
	inverseMatrix._12 = -inverseMatrix._12;
	inverseMatrix._21 = -inverseMatrix._21;
	inverseMatrix._23 = -inverseMatrix._23;
	inverseMatrix._32 = -inverseMatrix._32;

	// transpose the matrix
	float temp;
	temp = inverseMatrix._12;
	inverseMatrix._12 = inverseMatrix._21;
	inverseMatrix._21 = temp;
	temp = inverseMatrix._13;
	inverseMatrix._13 = inverseMatrix._31;
	inverseMatrix._31 = temp;
	temp = inverseMatrix._23;
	inverseMatrix._23 = inverseMatrix._32;
	inverseMatrix._32 = temp;

	// multiply by the determinant of the original matrix (temp is the determinant)

	temp = (matrix._11 * (matrix._22 * matrix._33 - matrix._23 * matrix._32)) + (matrix._12 * (matrix._21 * matrix._33 - matrix._23 * matrix._31)) + (matrix._13 * (matrix._21 * matrix._32 - matrix._22 * matrix._31));

	inverseMatrix._11 = inverseMatrix._11 * temp;
	inverseMatrix._12 = inverseMatrix._12 * temp;
	inverseMatrix._13 = inverseMatrix._13 * temp;
	inverseMatrix._21 = inverseMatrix._21 * temp;
	inverseMatrix._22 = inverseMatrix._22 * temp;
	inverseMatrix._23 = inverseMatrix._23 * temp;
	inverseMatrix._31 = inverseMatrix._31 * temp;
	inverseMatrix._32 = inverseMatrix._32 * temp;
	inverseMatrix._33 = inverseMatrix._33 * temp;

	return inverseMatrix;
}