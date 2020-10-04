#include "ParticleSystem.h"
#include "Appearance.h"


ParticleSystem::ParticleSystem(Vector3D position, Vector3D size, shared_ptr<Appearance> appearance, int particleDensity)
{
	_appearance = appearance;
	_position = position;
	_size = size;
	numParticles = _size.GetVector3D().x * _size.GetVector3D().z * particleDensity;
	int startCreation = rand() % numParticles;
	float posX, posZ;
	shared_ptr<Transform> temp;
	for (int i = 0; i < startCreation; i++)
	{
		temp = make_shared<Transform>();
		posX = float(rand() % int( 2.0f * size.GetVector3D().x + position.GetVector3D().x)) - size.GetVector3D().x;
		posZ = float(rand() % int( 2.0f * size.GetVector3D().z + position.GetVector3D().z)) - size.GetVector3D().z;
		temp->SetPosition(posX, _position.GetVector3D().y, posZ);
		temp->SetScale(0.1f, 0.1f, 0.1f);
		temp->SetRotation(0.0f, 0.0f, XMConvertToRadians(90.0f));
		_particles.push_back(make_shared<ParticleModel>(_appearance, temp, 0.0f));
	}
	numParticles = numParticles * 10;
}


ParticleSystem::~ParticleSystem()
{
	_particles.clear();
}

void ParticleSystem::Update(float Time)
{
	for (int i = 0; i < _particles.size(); i++)
	{
		if (_particles.at(i)->LifeCheck())
		{
			_particles.erase(_particles.begin() + i);
			i--;
			continue;
		}
		_particles.at(i)->Update(Time);	
	}
	if (_particles.size() < numParticles)
	{
		if (rand() % 10 == 1)
		{
			float posX, posZ;
			shared_ptr<Transform> temp = make_shared<Transform>();
			posX = float(rand() % int(_position.GetVector3D().x + 2.0f * _size.GetVector3D().x)) - _size.GetVector3D().x;
			posZ = float(rand() % int(_position.GetVector3D().z + 2.0f * _size.GetVector3D().z)) - _size.GetVector3D().z;
			temp->SetPosition(posX, _position.GetVector3D().y, posZ);
			temp->SetScale(0.1f, 0.1f, 0.1f);
			temp->SetRotation(0.0f, 0.0f, XMConvertToRadians(90.0f));
			_particles.push_back(make_shared<ParticleModel>(_appearance, temp, Time));
		}
	}
}
