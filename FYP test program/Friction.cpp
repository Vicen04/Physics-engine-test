#include "Friction.h"
#include "Physics.h"


Friction::Friction(float frictionFactor)
{
	_frictionFactor = frictionFactor;
}


Friction::~Friction()
{
}

void Friction::updateForce(Physics* physics, float time)
{
	velocity.SetVector3D(physics->GetVelocityForFriction().GetVector3D());
	if (velocity.Magnitude() > 0.0f)
	{
		SetForce(physics->GetForce("gravity")->GetForce().MultiplyByFloat(physics->GetMass()));
		SetForce(GetForce().MultiplyByFloat(_frictionFactor));
		SetForce(GetForce().MultiplyByFloat(-1.0f));
		velocity.SetVector3D(velocity.Normalization());
		SetForce(velocity.MultiplyByFloat(-GetForce().Magnitude()));
	}
	else
		SetForce(0.0f, 0.0f, 0.0f);

	SetAcceleration(GetForce().DivideByFloat(physics->GetMass()));
	SetVelocity(GetAcceleration().MultiplyByFloat(time));


}

void Friction::Compare()
{
	float velX = velocity.GetVector3D().x, velZ = velocity.GetVector3D().x, velX2 = GetVelocity().GetVector3D().x, velZ2 = GetVelocity().GetVector3D().z;

	if (velX < 0)
	{
		velX = -velX;
		velX2 = -velX2;
	}
	if (velZ < 0)
	{
		velZ = -velZ;
		velZ2 = -velZ2;
	}

	if ( velX + velX2 < 0)
	{		
		SetForce(0.0f, 0.0f, GetForce().GetVector3D().z);
		SetAcceleration(0.0f, 0.0f, GetAcceleration().GetVector3D().z);
		SetVelocity(0.0f, 0.0f, GetVelocity().GetVector3D().z);
	}
	if (velZ + velZ2 < 0)
	{
		SetForce(GetForce().GetVector3D().x, 0.0f, 0.0f);
		SetAcceleration(GetForce().GetVector3D().x, 0.0f, 0.0f);
		SetVelocity(GetForce().GetVector3D().x, 0.0f, 0.0f);
	}
}