#include "Gravity.h"
#include "Physics.h"


Gravity::Gravity(XMFLOAT3 gravity)
{
	SetForce(gravity.x, gravity.y, gravity.z);
}


Gravity::~Gravity()
{
}

void Gravity::updateForce(Physics *physics, float time)
{
	if (physics->GetMass() <= 0) return;
	SetAcceleration(GetForce().DivideByFloat(physics->GetMass()));
	SetVelocity(GetAcceleration().MultiplyByFloat(time));
}

