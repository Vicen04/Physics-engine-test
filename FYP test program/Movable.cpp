#include "Movable.h"
#include "Friction.h"


Movable::Movable(shared_ptr <Physics> physics, shared_ptr <Appearance> appearance, shared_ptr<Transform> transform): Prop(physics, appearance, transform)
{
	physics->AttachBasicForce("input");
	physics->AttachBasicForce("jump");
	SetType("Player");
}

Movable::~Movable()
{

}

void Movable::Update(float t)
{
	if (!IsInGround() && GetPhysics()->GetForce("gravity")->GetStartTime() == 0.0f)
		GetPhysics()->GetForce("gravity")->SetStartTime(t);

	if (_jump)
	{
		GetPhysics()->GetForce("jump")->SetForce(GetPhysics()->GetForce("jump")->GetForce().AddVector(GetPhysics()->GetForce("gravity")->GetForce().MultiplyByFloat(0.1f)));
		if (GetPhysics()->GetForce("jump")->GetForce().GetVector3D().y < 0.0f)
		{
			_jump = false;
			GetPhysics()->GetForce("jump")->SetForce(0.0f, 0.0f, 0.0f);
			GetPhysics()->GetForce("jump")->SetStartTime(0.0f);
		}
	}
	GetPhysics()->SetVelocityForFriction(GetPhysics()->GetForce("input")->GetForce().AddVector(GetPhysics()->GetForce("external")->GetForce().GetVector3D()));
	GetPhysics()->Update(t);
	Move();
	GetTransform()->Update(t);

	if (_moving)
		StopInput();

	GetPhysics()->GetForce("external")->SetForce(0.0f, 0.0f, 0.0f);

}

XMFLOAT3 Movable::ForceForExternal(bool ground)
{
	Vector3D toExternal;

	if (!ground)
	{
		toExternal.SetVector3D(toExternal.AddVector(GetPhysics()->GetForce("jump")->GetForce().GetVector3D()));
		toExternal.SetVector3D(toExternal.AddVector(GetPhysics()->GetForce("gravity")->GetForce().GetVector3D()));
	}
	else
		toExternal.SetVector3D(GetPhysics()->GetForce("input")->GetForce().AddVector(GetPhysics()->GetForce("external")->GetForce().GetVector3D()));

	return toExternal.GetVector3D();
}


void Movable::moveForward(float value, float time)
{
	if (GetPhysics()->GetForce("input")->GetStartTime() == 0.0f || value != oldValue)
	{
		GetPhysics()->GetForce("input")->SetStartTime(time);
		oldValue = value;
	}
	_moving = true;
	GetPhysics()->GetForce("input")->SetForce(value * 0.06f * sinf(GetPhysics()->GetCurrentRotation().y), 0.0f, value * 0.06f * cosf(GetPhysics()->GetCurrentRotation().y));
}

void Movable::moveRight(float value)
{
	if (_moving == true)
		GetPhysics()->SetRotation(XMFLOAT3(GetPhysics()->GetCurrentRotation().x, GetPhysics()->GetCurrentRotation().y + value * 0.006f, GetPhysics()->GetCurrentRotation().z));
}
void Movable::moveJump(float time)
{
	if (!_jump && IsInGround())
	{
		_jump = true;
		GetPhysics()->GetForce("jump")->SetForce(XMFLOAT3(0.0f, 0.18f, 0.0));
		GetPhysics()->GetForce("jump")->SetStartTime(time);
		Ground(false);
		static_cast<Friction*>(GetPhysics()->GetForce("friction"))->SetFrictionFactor(0.1f);
		GetPhysics()->GetForce("gravity")->SetStartTime(time);
	}
}

void Movable::StopInput()
{
	GetPhysics()->GetForce("input")->SetForce(GetPhysics()->GetForce("input")->GetForce().AddVector(GetPhysics()->GetForce("friction")->GetForce().GetVector3D()));
	if (GetPhysics()->GetForce("input")->GetForce().Magnitude() < GetPhysics()->GetForce("friction")->GetForce().Magnitude())
	{
		_moving = false;
		GetPhysics()->GetForce("input")->SetForce(0.0f, 0.0f, 0.0f);
		GetPhysics()->GetForce("input")->SetStartTime(0.0f);
		GetPhysics()->GetForce("friction")->SetStartTime(0.0f);
		GetPhysics()->GetForce("friction")->SetForce(0.0f, 0.0f, 0.0f);;
	}
}

void Movable::moveConstVel()
{

}

void Movable::moveConstAcc()
{

}

