#include "BasicForce.h"
#include "Physics.h"

BasicForce::BasicForce()
{
	SetForce(0.0f, 0.0f, 0.0f);
}

BasicForce::~BasicForce()
{

}

void BasicForce::updateForce(Physics* physics, float time)
{
	SetAcceleration(GetForce().DivideByFloat(physics->GetMass()));
	SetVelocity(GetAcceleration().MultiplyByFloat(time));
}