#pragma once
#include "GameObject.h"

class Fixed : public GameObject
{
public:
	Fixed(shared_ptr<Physics> physics, shared_ptr<Appearance> appearance, shared_ptr<Transform> transform);
	~Fixed();
	void Update(float time);

//please don't ask
private:
	bool fixed = true;
};

