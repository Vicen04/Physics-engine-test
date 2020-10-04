#pragma once

#include <string>
#include "Physics.h"
#include "Appearance.h"


class GameObject
{
public:
	GameObject(shared_ptr<Physics> physics, shared_ptr<Appearance> appearance, shared_ptr<Transform> transform);
	virtual ~GameObject();

	void SetType(string type) { _type = type; }
	string GetType() { return _type; }



	Appearance* GetAppearance() { return _appearance.get(); }
	Physics* GetPhysics(){ return _physics.get(); }
	Transform * GetTransform() { return _transform.get(); }
	void SetMaterial(int value);

	virtual void Update(float t) = 0;
	void Draw(ID3D11DeviceContext * pImmediateContext);

private:

	string _type;

	shared_ptr<Transform> _transform;
	shared_ptr<Physics> _physics;
	shared_ptr<Appearance> _appearance;
};

