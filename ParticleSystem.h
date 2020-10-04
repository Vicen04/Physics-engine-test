#pragma once
#include "ParticleModel.h"

class Vector3D;
class Appearance;

class ParticleSystem
{
public:
	//particle density changes the quantity of particles that can spawn, it is usually 10 times the area of the plane created by the size in the Y axis of the object.
	ParticleSystem(Vector3D position, Vector3D size, shared_ptr<Appearance> appearance, int particleDensity);
	~ParticleSystem();
	
	void Update(float deltaTime);
	int GetParticlesTotal() { return _particles.size(); }
	ParticleModel* GetParticle(int i) { return _particles.at(i).get(); }
	Appearance* GetAppearance() { return _appearance.get(); }

private: 
	Vector3D _position, _size;
	shared_ptr<Appearance> _appearance;
	vector<shared_ptr<ParticleModel>> _particles;
	int numParticles;
};


