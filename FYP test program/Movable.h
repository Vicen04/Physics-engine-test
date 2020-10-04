#pragma once
#include "Prop.h"


class Movable: public Prop
{
public:
	Movable(shared_ptr <Physics> physics, shared_ptr <Appearance> appearance, shared_ptr<Transform> transform);
	~Movable();
	XMFLOAT3 ForceForExternal(bool ground);
	void moveForward(float value, float time);
	void moveRight(float value);
	void moveJump(float time);
	void Update(float time);
	void StopInput();
	void moveConstVel();
	void moveConstAcc();
	virtual float TimeToReturn() { return GetPhysics()->GetForce("input")->GetStartTime(); }

private:
	bool constSpeed, constAccel, _jump = false, _moving= false;
	float oldValue;
};

