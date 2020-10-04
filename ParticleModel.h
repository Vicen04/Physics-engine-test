#pragma once
class Appearance;
#include "Transform.h"

class ParticleModel
{
public:
	ParticleModel(shared_ptr<Appearance> appearance, shared_ptr<Transform> transform, float TimeOfCreation);
	~ParticleModel();
	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);
	Transform* GetTransform() { return _transform.get(); }
	bool LifeCheck() { return destroy;}
	
private:
	shared_ptr<Transform> _transform;
	shared_ptr<Appearance> _appearance;
	Vector3D _speed;
	float lifetime = 0.0f;
	bool destroy = false;
};

