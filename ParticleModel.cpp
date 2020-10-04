#include "ParticleModel.h"
#include "Appearance.h"

ParticleModel::ParticleModel(shared_ptr<Appearance> appearance, shared_ptr<Transform> transform, float TimeOfCreation)
{
	_appearance = appearance;
	_transform = transform;
	_speed.SetVector3D(0.0f, float(rand() % 50)/ -250.0f, 0.0f);
	lifetime = rand() % 10 + 1.0f + TimeOfCreation;
}


ParticleModel::~ParticleModel()
{

}

void ParticleModel::Update(float t)
{
	if (t < lifetime)
	{
		_transform->SetPosition(_transform->GetPosition().x, _transform->GetPosition().y + _speed.GetVector3D().y * t, _transform->GetPosition().z);
		_transform->Update(t);
	}
	else
		destroy = true;
}

void ParticleModel::Draw(ID3D11DeviceContext * pImmediateContext)
{
	_appearance->Draw(pImmediateContext);
}
