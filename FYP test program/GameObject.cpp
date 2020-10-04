#include "GameObject.h"

GameObject::GameObject(shared_ptr<Physics> physics, shared_ptr<Appearance> appearance, shared_ptr<Transform> transform)
{		
	_transform = transform;
	_appearance= appearance;
	_physics = physics;
}

GameObject::~GameObject()
{
	_transform.reset();
	_physics.reset();
	_appearance.reset();
	_type.clear();
}

void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	_appearance->Draw(pImmediateContext);
}

void GameObject::SetMaterial(int value)
{
	float multiplier = 1.0f;
	switch (value)
	{
	case ObjectMaterial::GLASS:
		multiplier = 0.7f;
	case ObjectMaterial::METAL:
		multiplier = 2.0f;
	case ObjectMaterial::STONE:
		multiplier = 1.5f;
	case ObjectMaterial::PLASTIC:
		multiplier = 1.0f;
	case ObjectMaterial::SPONGE:
		multiplier = 0.4f;
	}
	_physics->SetMass(_physics->GetMass() * multiplier);
}