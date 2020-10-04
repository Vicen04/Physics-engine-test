#include "RigidBody.h"
#include "Gravity.h"

RigidBody::RigidBody(std::shared_ptr<Transform>  transform): Physics(transform)
{
	_objectWorld = transform;
	SetType("RigidBody");
}


RigidBody::~RigidBody()
{
	_objectWorld.reset();
}

void RigidBody::Update(float t)
{
	for (int i = 0; i < GetForceCount(); i++)
	{

			if (!GetFluid() && GetForceName(i) == "dragForce")
				continue;
			if (GetForce(GetForceName(i))->GetStartTime() > 0.0f)
			{
			GetForce(GetForceName(i))->SetCurrTime(t - GetForce(GetForceName(i))->GetStartTime());
			}
			else
				GetForce(GetForceName(i))->SetCurrTime(0.0f);
       		GetForce(GetForceName(i))->updateForce(this, GetForce(GetForceName(i))->GetCurrTime());	
	}

}

void RigidBody::CalculateBounding(std::vector<XMFLOAT3> test, XMFLOAT3 scale)
{
	float minX = INT32_MAX, maxX = INT32_MIN, minY = INT32_MAX, maxY = INT32_MIN, minZ = INT32_MAX, maxZ = INT32_MIN, x, y ,z;

	for (int i = 0; i < test.size(); i++)
	{
		minX = std::fminf(test.at(i).x, minX);
		minY = std::fminf(test.at(i).y, minY);
		minZ = std::fminf(test.at(i).z, minZ);

		maxX = std::fmaxf(test.at(i).x, maxX);
		maxY = std::fmaxf(test.at(i).y, maxY);
		maxZ = std::fmaxf(test.at(i).z, maxZ);
	}

	SetMinBoundaries(XMFLOAT3(minX, minY, minZ));
	SetMaxBoundaries(XMFLOAT3(maxX, maxY, maxZ));

}

