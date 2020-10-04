#include "Fixed.h"

Fixed::Fixed(shared_ptr<Physics> physics, shared_ptr<Appearance> appearance, shared_ptr<Transform> transform) : GameObject(physics, appearance, transform)
{
	SetType("Fixed");
	if (physics->GetType() == "SoftBody")
		physics->AttachBasicForce("external");
}

Fixed::~Fixed()
{

}

void Fixed::Update(float time)
{
	//GetPhysics()->GetForce("external")->updateForce(GetPhysics(), time);
	GetTransform()->Update(time);
}