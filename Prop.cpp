#include "Prop.h"

Prop::Prop(std::shared_ptr<Physics> physics, std::shared_ptr<Appearance> appearance, std::shared_ptr<Transform> transform): GameObject(physics, appearance, transform)
{
	SetType("Prop");
	physics->AddGravity();
	physics->AttachBasicForce("external");
}

Prop::~Prop()
{

}

void Prop::Update(float time)
{
	if (!_ground && GetPhysics()->GetForce("gravity")->GetStartTime() == 0.0f)
		GetPhysics()->GetForce("gravity")->SetStartTime(time);
	if (GetPhysics()->GetForce("external")->GetForce().Magnitude() > GetPhysics()->GetForce("friction")->GetForce().Magnitude())
	{
		GetPhysics()->SetVelocityForFriction(XMFLOAT3(GetPhysics()->GetForce("external")->GetVelocity().GetVector3D().x, 0.0f, GetPhysics()->GetForce("external")->GetVelocity().GetVector3D().z));
		if (GetPhysics()->GetForce("friction")->GetStartTime() == 0.0f)
			GetPhysics()->GetForce("friction")->SetStartTime(GetPhysics()->GetForce("external")->GetStartTime());
		GetPhysics()->GetForce("external")->SetForce(GetPhysics()->GetForce("external")->GetForce().AddVector(GetPhysics()->GetForce("friction")->GetForce().GetVector3D()));
	}
	else
	{
		GetPhysics()->GetForce("external")->SetForce(0.0f, 0.0f, 0.0f);
		GetPhysics()->GetForce("external")->SetStartTime(0.0f);
		GetPhysics()->GetForce("friction")->SetForce(0.0f, 0.0f, 0.0f);
		GetPhysics()->GetForce("friction")->SetStartTime(0.0f);
	}
	GetPhysics()->Update(time);
	Move();
	GetTransform()->Update(time);
}

void Prop::Move()
{
	GetPhysics()->SetOldPosition();
	Vector3D _speedTotal, _acceleration, _accelerationTotal, _updatedPosition;
	_updatedPosition.SetVector3D(GetPhysics()->GetCurrentPosition());
	for (int i = 0; i < GetPhysics()->GetForceCount(); i++)
	{
		if (_ground && GetPhysics()->GetForceName(i) == "gravity")
			continue;
		_speedTotal.SetVector3D(_speedTotal.AddVector(GetPhysics()->GetForce(GetPhysics()->GetForceName(i))->GetVelocity().MultiplyByFloat(GetPhysics()->GetForce(GetPhysics()->GetForceName(i))->GetCurrTime())));
		_acceleration.SetVector3D(GetPhysics()->GetForce(GetPhysics()->GetForceName(i))->GetAcceleration().MultiplyByFloat(GetPhysics()->GetForce(GetPhysics()->GetForceName(i))->GetCurrTime() * GetPhysics()->GetForce(GetPhysics()->GetForceName(i))->GetCurrTime()));
		_accelerationTotal.SetVector3D(_accelerationTotal.AddVector(_acceleration.DivideByFloat(2.0f)));
	}
	_updatedPosition.SetVector3D(_updatedPosition.AddVector(_speedTotal.GetVector3D()));
	_updatedPosition.SetVector3D(_updatedPosition.AddVector(_accelerationTotal.GetVector3D()));
	GetTransform()->SetPosition(_updatedPosition.GetVector3D());
}

XMFLOAT3 Prop::ForceForExternal(bool ground)
{
	if (!ground)
		return	GetPhysics()->GetForce("external")->GetForce().AddVector(GetPhysics()->GetForce("gravity")->GetForce().GetVector3D());
	else
	return GetPhysics()->GetForce("external")->GetForce().GetVector3D();
}


