#pragma once
#include "GameObject.h"

class Prop : public GameObject
{
public:
	Prop(shared_ptr<Physics> physics, shared_ptr<Appearance> appearance, shared_ptr<Transform> transform);
	~Prop();
	virtual void Update(float time);
	void Ground(bool ground) { _ground = ground; if (ground) GetPhysics()->GetForce("gravity")->SetStartTime(0.0f);}
	virtual XMFLOAT3 ForceForExternal(bool ground);
	virtual float TimeToReturn() { return GetPhysics()->GetForce("external")->GetStartTime(); }
	bool IsInGround() { return _ground; }
	void Move();

private:
	bool _ground = false;
};

