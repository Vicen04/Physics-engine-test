#include "DragForce.h"
#include "Physics.h"

/* Compute drag force acting on object
	dragFactor: factor proportional to damping coefficient and dimensions of object
 */
DragForce::DragForce(float dragFactor, bool laminar)
{	
	_dragFactor = dragFactor;
	_laminar = laminar;
}


DragForce::~DragForce()
{
}

// Motion for object affected by drag 

// Compute drag force acting on object (for laminar flow)

void DragForce::DragLaminarFlow(Vector3D velocity, float dragFactor) 
{
	// calculate components of drag force
	SetForce(-dragFactor * velocity.GetVector3D().x, -dragFactor * velocity.GetVector3D().y, -dragFactor * velocity.GetVector3D().z);
}

// Compute drag force acting on object (for turbulent flow)
void DragForce::DragTurbulentFlow(Vector3D velocity, float dragFactor)
{
	if (!velocity.VectorEqual(XMFLOAT3(0.0f, 0.0f, 0.0f)))
	{
		// calculate unit vector of velocity
		unitVel = velocity.Normalization();

		// calculate magnitude of drag force
		float dragMag = dragFactor * velMag * velMag;

		// calculate components of drag force
		SetForce(-dragMag * unitVel.GetVector3D().x, -dragMag * unitVel.GetVector3D().y, -dragMag * unitVel.GetVector3D().z);
	}
}

void DragForce::updateForce(Physics* physics, float time)
{
	CheckSpeed(physics->GetVelocityForDragForce());

		if (_laminar)
			// calculate drag force for laminar flow
			DragLaminarFlow(physics->GetVelocityForDragForce(), _dragFactor);
		else
			// calculate drag force for turbulent flow
			DragTurbulentFlow(physics->GetVelocityForDragForce(), _dragFactor);

		SetAcceleration(GetForce().DivideByFloat(physics->GetMass()));
		SetVelocity(GetAcceleration().MultiplyByFloat(time));
}

void DragForce::CheckSpeed(Vector3D velocity)
{
	// calculate magnitude of velocity
	velMag = velocity.Magnitude();

	if (velMag > 300.0f)
		_laminar = false;
	else
		_laminar = true;
}