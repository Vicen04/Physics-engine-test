#include "Physics.h"
#include "Gravity.h"
#include "DragForce.h"
#include "Friction.h"
#include "AngularForce.h"
#include "BasicForce.h"
#include <string>

Physics::Physics(shared_ptr<Transform>  physics)
{
	_transform = physics;
}

Physics::~Physics()
{
	for (int i = 0; i < _Force.size(); i++)
	{
		delete _Force.at(_forcesList.at(i));
	}
	_Force.clear();
	_forcesList.clear();
	_transform.reset();
}

void Physics::AttachBasicForce(string forceName)
{
	_forcesList.push_back(forceName);
	_Force.emplace(forceName, new BasicForce());
}

void Physics::AddDragForce(float dragforce)
{
	_forcesList.push_back("dragForce");
	_Force.emplace("dragForce", new DragForce(dragforce, false));
}

void Physics::AddGravity()
{
	_forcesList.push_back("gravity");
	_Force.emplace("gravity", new Gravity(XMFLOAT3(0.0f, -0.00981f, 0.0f)));
}

void Physics::AddFriction(float friction)
{
	_forcesList.push_back("friction");
	_Force.emplace("friction", new Friction(friction));
}

void Physics::AddAngular()
{
	float lengthX = fmaxf(GetCurrentPosition().x - GetMinBoundaries().x, GetMaxBoundaries().x - GetCurrentPosition().x);
	float lengthY = fmaxf(GetCurrentPosition().y - GetMinBoundaries().y, GetMaxBoundaries().y - GetCurrentPosition().y);
	float lengthZ = fmaxf(GetCurrentPosition().z - GetMinBoundaries().y, GetMaxBoundaries().z - GetCurrentPosition().z);	

	_forcesList.push_back("angular");
	_Force.emplace("angular", new AngularForce(GetMass(), lengthX, lengthY, lengthZ));
}

Vector3D Physics::GetVelocityForDragForce()
{
	Vector3D _speedTotal;
	for (int i = 0; i < _forcesList.size(); i++)
	{
		if (_forcesList.at(i) != "dragForce")
			_speedTotal.SetVector3D(_speedTotal.AddVector(_Force.at(_forcesList.at(i))->GetVelocity().GetVector3D()));
	}
	return _speedTotal;
}

Vector3D Physics::GetallForces()
{
	Vector3D ForceTotal;
	for (int i = 0; i < _forcesList.size(); i++)
	{
			ForceTotal.SetVector3D(ForceTotal.AddVector(_Force.at(_forcesList.at(i))->GetForce().GetVector3D()));
	}
	return ForceTotal;
}

Vector3D Physics::GetallForcesExcept(std::string forceName)
{
	Vector3D ForceTotal;
	for (int i = 0; i < _forcesList.size(); i++)
	{
		if (_forcesList.at(i) != forceName)
			ForceTotal.SetVector3D(ForceTotal.AddVector(_Force.at(_forcesList.at(i))->GetForce().GetVector3D()));
	}
	return ForceTotal;
}

void Physics::SetBoundariesRotatedMax(XMFLOAT3 boundaries)
{
	_boundariesRotatedMax.x = boundaries.x;
	_boundariesRotatedMax.y = boundaries.y;
	_boundariesRotatedMax.z = boundaries.z;
}

void Physics::SetBoundariesRotatedMin(XMFLOAT3 boundaries)
{
	_boundariesRotatedMin.x = boundaries.x;
	_boundariesRotatedMin.y = boundaries.y;
	_boundariesRotatedMin.z = boundaries.z;
}

void Physics::UpdateBoundingRotation()
{
	XMVECTOR Boundaries = XMLoadFloat4(&XMFLOAT4(_boundariesMin.x, _boundariesMin.y, _boundariesMin.z, 0.0f));;
	Boundaries = XMVector4Transform(Boundaries, _transform->GetWorldMatrix());
	SetBoundariesRotatedMin(XMFLOAT3(Boundaries.m128_f32[0], Boundaries.m128_f32[1], Boundaries.m128_f32[2]));

	Boundaries = XMLoadFloat4(&XMFLOAT4(_boundariesMax.x, _boundariesMax.y, _boundariesMax.z, 0.0f));
	Boundaries = XMVector4Transform(Boundaries, _transform->GetWorldMatrix());
	SetBoundariesRotatedMax(XMFLOAT3(Boundaries.m128_f32[0], Boundaries.m128_f32[1], Boundaries.m128_f32[2]));
}