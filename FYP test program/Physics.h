#pragma once
#include "ForceGenerator.h"
#include <map>

class Gravity;

enum ObjectMaterial
{
	PLASTIC = 0,
	GLASS,
	METAL,
	STONE,
	SPONGE
};


class Physics
{
public:
	Physics(shared_ptr<Transform>  physics);
	~Physics();
	virtual void Update(float time) = 0;
	void AttachBasicForce(string forceName);
	ForceGenerator* GetForce(string forceName) { return _Force.at(forceName); }
	Vector3D GetallForces();
	Vector3D GetallForcesExcept(string forceName);
	std::string GetForceName(int value) { return _forcesList.at(value); }
	void AddDragForce(float dragforce);
	void AddFriction(float FrictionFactor);
	void AddAngular();
	void AddGravity();
	virtual void CalculateBounding(vector<XMFLOAT3> test, XMFLOAT3 scale) = 0;
	void SetType(std::string type) { _type = type; }
	std::string GetType() { return _type; }

	void SetFluid(bool isInFluid) { _fluid = isInFluid; }
	bool GetFluid() { return _fluid; }
	void SetMass(float mass) { _mass = mass; }
	float GetMass() { return _mass; }
	Vector3D GetVelocityForDragForce();
	Vector3D GetVelocityForFriction() { return _velocityFriction; }
	void SetVelocityForFriction(Vector3D velocity) { _velocityFriction = velocity; };
	int GetForceCount() { return _Force.size(); }

	XMFLOAT3 GetCurrentPosition() { return _transform->GetPosition(); }
	void SetOldPosition() { _oldPosition.SetVector3D(_transform->GetPosition()); }
	XMFLOAT3 GetCurrentRotation() { return _transform->GetRotation(); }
	void SetRotation(XMFLOAT3 rotation) { _transform->SetRotation(rotation); }
	XMFLOAT3 GetOldPosition() { return _oldPosition.GetVector3D(); }
	void SetMinBoundaries(XMFLOAT3 boundaries) { _boundariesMin = boundaries; }
	XMFLOAT3 GetMinBoundaries() { return _boundariesMin; }
	void SetMaxBoundaries(XMFLOAT3 boundaries) { _boundariesMax = boundaries; }
	XMFLOAT3 GetMaxBoundaries() { return _boundariesMax; }
	void SetHitPoint(XMFLOAT3 hitPoint) { _hitPoint.SetVector3D(hitPoint); }
	Vector3D GetHitPoint() { return _hitPoint; }

	XMFLOAT3 GetBoundariesRotatedMax() { return _boundariesRotatedMax; }
	void SetBoundariesRotatedMax(XMFLOAT3 boundaries);
	XMFLOAT3 GetBoundariesRotatedMin() { return _boundariesRotatedMin; }
	void SetBoundariesRotatedMin(XMFLOAT3 boundaries);
	virtual void UpdateBoundingRotation();

private:
	float _mass = 1.0f;
	map<string, ForceGenerator*> _Force;
	vector<string> _forcesList;
	shared_ptr<Transform>  _transform;
	Vector3D _oldPosition, _velocityFriction, _hitPoint;
	XMFLOAT3 _boundariesMin, _boundariesMax, _boundariesRotatedMax, _boundariesRotatedMin;
	string _type;
	bool _fluid = false;
};

