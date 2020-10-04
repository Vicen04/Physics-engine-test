#include "CollisionHandler.h"
#include "Friction.h"


CollisionHandler::CollisionHandler()
{

}

CollisionHandler::~CollisionHandler()
{

}

bool CollisionHandler::CheckCollision(Physics* model, string shape, Physics* model2, string shape2)
{
	if (shape == "cube" && shape2 == "cube")
	{
		CalculateCurrentBounds1(model->GetCurrentPosition(), model->GetBoundariesRotatedMax(), model->GetBoundariesRotatedMin());
		CalculateCurrentBounds2(model2->GetCurrentPosition(), model2->GetBoundariesRotatedMax(), model2->GetBoundariesRotatedMin());

		if (CollisionDirectionX() && CollisionDirectionY() && CollisionDirectionZ())
		{
			CollisionDirectionHandler(model, model2);
			return true;
		}
	}
	else if (shape == "sphere" && shape2 == "sphere")
	{
		CalculateSphereBounds1(model->GetCurrentPosition(), model->GetMaxBoundaries());
		CalculateSphereBounds2(model2->GetCurrentPosition(), model2->GetMaxBoundaries());

		if (SphereCollision())
		{
			CollisionDirectionHandler(model, model2);
			return true;
		}
	}
	else if (shape == "sphere" && shape2 == "cube")
	{
		CalculateSphereBounds1(model->GetCurrentPosition(), model->GetMaxBoundaries());
		CalculateCurrentBounds2(model2->GetCurrentPosition(), model2->GetBoundariesRotatedMax(), model2->GetBoundariesRotatedMin());
		if (HybridCollisionSphereBox())
		{
			CollisionDirectionHandler(model, model2);
			return true;
		}
	}
	else if (shape == "cube" && shape2 == "sphere")
	{
		CalculateCurrentBounds1(model->GetCurrentPosition(), model->GetBoundariesRotatedMax(), model->GetBoundariesRotatedMin());
		CalculateSphereBounds2(model2->GetCurrentPosition(), model2->GetMaxBoundaries());
		if (HybridCollisionBoxSphere())
		{
			CollisionDirectionHandler(model, model2);
			return true;
		}
	}

	return false;
}

void CollisionHandler::CollisionWithFixed(GameObject* model, GameObject* model2)
{		
	XMFLOAT3 returnVector = model->GetPhysics()->GetallForces().MultiplyByFloat(-1.0f);
	if (_collidedY)
	{
		if (returnVector.y > 0.0f)
		{
			model->GetTransform()->SetPosition(model->GetTransform()->GetPosition().x, max_Y_2 + 0.0001f + max(model->GetPhysics()->GetBoundariesRotatedMin().y, model->GetPhysics()->GetBoundariesRotatedMax().y), model->GetTransform()->GetPosition().z);
			static_cast<Prop*>(model)->Ground(true);
			static_cast<Friction*>(model->GetPhysics()->GetForce("friction"))->SetFrictionFactor(0.3f);
		}
		else
		{
			model->GetTransform()->SetPosition(model->GetTransform()->GetPosition().x, min_Y_2 - 0.0001f + min(model->GetPhysics()->GetBoundariesRotatedMin().y, model->GetPhysics()->GetBoundariesRotatedMax().y), model->GetTransform()->GetPosition().z);
			model->GetPhysics()->GetForce("external")->SetForce(0.0f, returnVector.y, 0.0f);
		}
	}
	else
	{
		model->GetPhysics()->GetForce("external")->SetForce(returnVector.x, 0.0f, returnVector.z);
		if (_collidedX)
		{

			if (returnVector.x > 0.0f)
				model->GetTransform()->SetPosition(max_X_2 + 0.0001f + max(model->GetPhysics()->GetBoundariesRotatedMin().x, model->GetPhysics()->GetBoundariesRotatedMax().x), model->GetTransform()->GetPosition().y, model->GetTransform()->GetPosition().z);
			else
				model->GetTransform()->SetPosition(min_X_2 - 0.0001f + min(model->GetPhysics()->GetBoundariesRotatedMin().x, model->GetPhysics()->GetBoundariesRotatedMax().x), model->GetTransform()->GetPosition().y, model->GetTransform()->GetPosition().z);

			if (model->GetType() == "player")
				model->GetPhysics()->GetForce("external")->SetStartTime(model->GetPhysics()->GetForce("input")->GetStartTime());
			else
				model->GetPhysics()->GetForce("external")->SetStartTime(model->GetPhysics()->GetForce("external")->GetStartTime());
		}

		else if (_collidedZ)
		{
			if (returnVector.z > 0.0f)
				model->GetTransform()->SetPosition(model->GetTransform()->GetPosition().x, model->GetTransform()->GetPosition().y, max_Z_2 + 0.0001f + max(model->GetPhysics()->GetBoundariesRotatedMin().z, model->GetPhysics()->GetBoundariesRotatedMax().z));
			else
				model->GetTransform()->SetPosition(model->GetTransform()->GetPosition().x, model->GetTransform()->GetPosition().y, min_Z_2 - 0.0001f + min(model->GetPhysics()->GetBoundariesRotatedMin().z, model->GetPhysics()->GetBoundariesRotatedMax().z));

			if (model->GetType() == "player")
				model->GetPhysics()->GetForce("external")->SetStartTime(model->GetPhysics()->GetForce("input")->GetStartTime());
			else
				model->GetPhysics()->GetForce("external")->SetStartTime(model->GetPhysics()->GetForce("external")->GetStartTime());
		}
	}
}

void CollisionHandler::CollisionWithProp(GameObject* model, GameObject* model2, float time)
{
	Vector3D returnVector = static_cast<Prop*>(model)->ForceForExternal(static_cast<Prop*>(model2)->IsInGround());

	if (model->GetPhysics()->GetallForces().Magnitude() > model2->GetPhysics()->GetallForces().Magnitude())
	{
		if (_collidedY)
		{
			
			model2->GetPhysics()->GetForce("external")->SetForce(returnVector.GetVector3D());
			model2->GetPhysics()->GetForce("external")->SetStartTime(model->GetPhysics()->GetForce("gravity")->GetStartTime());
			model->GetPhysics()->GetForce("external")->SetForce(returnVector.MultiplyByFloat(-1.0f));
			model->GetPhysics()->GetForce("external")->SetStartTime(model->GetPhysics()->GetForce("gravity")->GetStartTime());

			if (returnVector.GetVector3D().y < 0.0f)
			{
				model->GetTransform()->SetPosition(model->GetTransform()->GetPosition().x, max_Y_2 + 0.0001f + max(model->GetPhysics()->GetBoundariesRotatedMin().y, model->GetPhysics()->GetBoundariesRotatedMax().y), model->GetTransform()->GetPosition().z);
				static_cast<Prop*>(model)->Ground(true);
				static_cast<Friction*>(model->GetPhysics()->GetForce("friction"))->SetFrictionFactor(0.3f);

			}
			else if (returnVector.GetVector3D().y > 0.0f)
				model->GetTransform()->SetPosition(model->GetTransform()->GetPosition().x, min_Y_2 - 0.0001f + min(model->GetPhysics()->GetBoundariesRotatedMin().y, model->GetPhysics()->GetBoundariesRotatedMax().y), model->GetTransform()->GetPosition().z);
		}
		else
		{
			model2->GetPhysics()->GetForce("external")->SetForce(returnVector.GetVector3D());
			model2->GetPhysics()->GetForce("external")->SetStartTime(static_cast<Prop*>(model)->TimeToReturn());
			model->GetPhysics()->GetForce("external")->SetForce(returnVector.MultiplyByFloat(-1.0f));
			model->GetPhysics()->GetForce("external")->SetStartTime(static_cast<Prop*>(model)->TimeToReturn());

			if (_collidedX)
			{
				
				if (returnVector.GetVector3D().x < 0.0f)
					model->GetTransform()->SetPosition(max_X_2 + 0.0001f + max(model->GetPhysics()->GetBoundariesRotatedMin().x, model->GetPhysics()->GetBoundariesRotatedMax().x), model->GetTransform()->GetPosition().y, model->GetTransform()->GetPosition().z);
				else if(returnVector.GetVector3D().x > 0.0f)
					model->GetTransform()->SetPosition(min_X_2 - 0.0001f + min(model->GetPhysics()->GetBoundariesRotatedMin().x, model->GetPhysics()->GetBoundariesRotatedMax().x), model->GetTransform()->GetPosition().y, model->GetTransform()->GetPosition().z);
			}

			else if (_collidedZ)
			{
				if (returnVector.GetVector3D().z < 0.0f)
					model->GetTransform()->SetPosition(model->GetTransform()->GetPosition().x, model->GetTransform()->GetPosition().y, 0.0001f + max_Z_2 + max(model->GetPhysics()->GetBoundariesRotatedMin().z, model->GetPhysics()->GetBoundariesRotatedMax().z));
				else if (returnVector.GetVector3D().z > 0.0f)
					model->GetTransform()->SetPosition(model->GetTransform()->GetPosition().x, model->GetTransform()->GetPosition().y, min_Z_2 - 0.0001f + min(model->GetPhysics()->GetBoundariesRotatedMin().z, model->GetPhysics()->GetBoundariesRotatedMax().z));
			}
		}
	}
}


void CollisionHandler::CollisionDirectionHandler(Physics* model, Physics* model2)
{
	CalculateCurrentBounds1(model->GetOldPosition(), model->GetBoundariesRotatedMax(), model->GetBoundariesRotatedMin());

	if (!CollisionDirectionX())
	{
		_collidedX = true;
		_collidedY = false;
		_collidedZ = false;
	}

	else if (!CollisionDirectionZ())
	{
		_collidedX = false;
		_collidedY = false;
		_collidedZ = true;
	}
	else if (!CollisionDirectionY())
	{
		_collidedX = false;
		_collidedY = true;
		_collidedZ = false;
	}
}

bool CollisionHandler::CollisionDirectionX()
{
	if ((max_X >= min_X_2 && min_X <= min_X_2) ||
		(max_X >= min_X_2 && min_X >= min_X_2 && max_X <= max_X_2) ||
		(min_X <= max_X_2 && max_X >= max_X_2) ||
		(min_X <= max_X_2 && max_X <= max_X_2 && min_X >= min_X_2))
	{
		return true;
	}
	return false;
}

bool CollisionHandler::CollisionDirectionY()
{
	if ((min_Y <= max_Y_2 && max_Y >= min_Y_2) ||
		(min_Y <= max_Y_2 && max_Y <= max_Y_2 && min_Y >= min_Y_2) || 
		(max_Y >= min_Y_2 && min_Y <= min_Y_2) ||
		(max_Y >= min_Y_2 && min_Y >= min_Y_2 && max_Y <= max_Y_2))
	{
		return true;
	}
	return false;
}

bool CollisionHandler::CollisionDirectionZ()
{
	if ((max_Z >= min_Z_2 && min_Z <= min_Z_2) ||
		(max_Z >= min_Z_2 && min_Z >= min_Z_2 && max_Z <= max_Z_2) || 
		(min_Z <= max_Z_2 && max_Z >= max_Z_2) ||
		(min_Z <= max_Z_2 && max_Z <= max_Z_2 && min_Z >= min_Z_2))
	{
		return true;
	}
	return false;
}

bool CollisionHandler::SphereCollision()
{
	float distance = sqrt((max_X - max_X_2) * (max_X - max_X_2) +
		(max_Y - max_Y_2) * (max_Y - max_Y_2) +
		(max_Z - max_Z_2) * (max_Z - max_Z_2));
	
	if (distance < (max_X + max_X_2))
	{
		return true;
	}
	return false;
}

bool CollisionHandler::HybridCollisionSphereBox()
{
	//closest point to the sphere
	float x = max(min_X_2, min(max_X, max_X_2));
	float y = max(min_X_2, min(max_Y, max_Y_2));
	float z = max(min_X_2, min(max_Z, max_Z_2));


	float distance = sqrt((x - max_X) * (x - max_X) +
		(y - max_Y) * (y - max_Y) +
		(z - max_Z) * (z - max_Z));

	if (distance < max_X * max_X)
	{
		return true;
	}
	return false;
}

bool CollisionHandler::HybridCollisionBoxSphere()
{
	float x = max(min_X, min(max_X_2, max_X));
	float y = max(min_X, min(max_Y_2, max_Y));
	float z = max(min_X, min(max_Z_2, max_Z));


	float distance = sqrt((x - max_X_2) * (x - max_X_2) +
		(y - max_Y_2) * (y - max_Y_2) +
		(z - max_Z_2) * (z - max_Z_2));

	if (distance < max_X_2 * max_X_2)
	{
		return true;
	}
	return false;
}

bool CollisionHandler::CheckBottomCollision(GameObject* model, GameObject* model2)
{
	if (model->GetAppearance()->GetShape() == "cube")
		CalculateCurrentBounds1(model->GetPhysics()->GetCurrentPosition(), model->GetPhysics()->GetBoundariesRotatedMax(), model->GetPhysics()->GetBoundariesRotatedMin());
	else if (model->GetAppearance()->GetShape() == "sphere")
		CalculateSphereBounds1(model->GetPhysics()->GetCurrentPosition(), model->GetPhysics()->GetMaxBoundaries());
	if (model2->GetAppearance()->GetShape() == "cube")
	    CalculateCurrentBounds2(model2->GetPhysics()->GetCurrentPosition(), model2->GetPhysics()->GetBoundariesRotatedMax(), model2->GetPhysics()->GetBoundariesRotatedMin());
	else if (model->GetAppearance()->GetShape() == "sphere")
		CalculateSphereBounds2(model2->GetPhysics()->GetCurrentPosition(), model2->GetPhysics()->GetMaxBoundaries());


	if (CollisionDirectionX() && CollisionDirectionZ())
	{
			if (min_Y <= max_Y_2 + 0.00015f && max_Y >= min_Y_2)
			{
				static_cast<Prop*>(model)->Ground(true);
				static_cast<Friction*>(model->GetPhysics()->GetForce("friction"))->SetFrictionFactor(0.3f);
				return true;
			}
	}

	static_cast<Prop*>(model)->Ground(false);
	static_cast<Friction*>(model->GetPhysics()->GetForce("friction"))->SetFrictionFactor(0.1f);
	return false;
}

void CollisionHandler::CalculateCurrentBounds1(XMFLOAT3 Position, XMFLOAT3 boundariesMax, XMFLOAT3 boundariesMin)
{
	if (boundariesMax.x > boundariesMin.x)
	{
		min_X = Position.x + boundariesMin.x;
		max_X = Position.x + boundariesMax.x;
	}
	else
	{
		min_X = Position.x + boundariesMax.x;
		max_X = Position.x + boundariesMin.x;
	}

	if (boundariesMax.y > boundariesMin.y)
	{
		min_Y = Position.y + boundariesMin.y;
		max_Y = Position.y + boundariesMax.y;
	}
	else
	{
		min_Y = Position.y + boundariesMax.y;
		max_Y = Position.y + boundariesMin.y;
	}

	if (boundariesMax.z > boundariesMin.z)
	{
		min_Z = Position.z + boundariesMin.z;
		max_Z = Position.z + boundariesMax.z;
	}
	else
	{
		min_Z = Position.z + boundariesMax.z;
		max_Z = Position.z + boundariesMin.z;
	}
}
void CollisionHandler::CalculateSphereBounds1(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 boundaries)
{
	max_X = position.x + boundaries.x;
	max_Y = position.y + boundaries.y;
	max_Z = position.z + boundaries.z;
	min_X = position.x - boundaries.x;
	min_Y = position.y - boundaries.y;
	min_Z = position.z - boundaries.z;
}

void CollisionHandler::CalculateSphereBounds2(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3  boundaries)
{
	max_X_2 = position.x + boundaries.x;
	max_Y_2 = position.y + boundaries.y;
	max_Z_2= position.z + boundaries.z;
	min_X_2 = position.x - boundaries.x;
	min_Y_2 = position.y - boundaries.y;
	min_Z_2 = position.z - boundaries.z;
}

void CollisionHandler::CalculateCurrentBounds2(XMFLOAT3 Position, XMFLOAT3 boundariesMax, XMFLOAT3 boundariesMin)
{
	if (boundariesMax.x > boundariesMin.x)
	{
		min_X_2 = Position.x + boundariesMin.x;
		max_X_2 = Position.x + boundariesMax.x;
	}
	else
	{
		min_X_2 = Position.x + boundariesMax.x;
		max_X_2 = Position.x + boundariesMin.x;
	}

	if (boundariesMax.y > boundariesMin.y)
	{
		min_Y_2 = Position.y + boundariesMin.y;
		max_Y_2 = Position.y + boundariesMax.y;
	}
	else
	{
		min_Y_2 = Position.y + boundariesMax.y;
		max_Y_2 = Position.y + boundariesMin.y;
	}

	if (boundariesMax.z > boundariesMin.z)
	{
		min_Z_2 = Position.z + boundariesMin.z;
		max_Z_2 = Position.z + boundariesMax.z;
	}
	else
	{
		min_Z_2 = Position.z + boundariesMax.z;
		max_Z_2 = Position.z + boundariesMin.z;
	}
}
