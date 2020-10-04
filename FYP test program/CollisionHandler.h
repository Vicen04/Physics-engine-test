#pragma once
#include "Movable.h"

class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();
	bool CheckCollision(Physics* model, string shape, Physics* model2, string shape2);
	void CalculateCurrentBounds1(DirectX::XMFLOAT3 Position, DirectX::XMFLOAT3  boundariesMax, DirectX::XMFLOAT3  boundariesMin);
	void CalculateSphereBounds1(DirectX::XMFLOAT3 Position, DirectX::XMFLOAT3  boundaries);
	void CalculateCurrentBounds2(DirectX::XMFLOAT3  Position, DirectX::XMFLOAT3  boundariesMax, DirectX::XMFLOAT3  boundariesMin);
	void CalculateSphereBounds2(DirectX::XMFLOAT3 Position, DirectX::XMFLOAT3  boundaries);
	bool CheckBottomCollision(GameObject* model, GameObject* model2);
	void CollisionWithFixed(GameObject* model, GameObject* model2);
    void CollisionWithProp(GameObject* model, GameObject* model2, float time);
	void CollisionDirectionHandler(Physics* model, Physics* model2);
	bool CollisionDirectionX();
	bool CollisionDirectionY();
	bool CollisionDirectionZ();
	bool SphereCollision();
	bool HybridCollisionSphereBox();
	bool HybridCollisionBoxSphere();


private:
	float min_X, max_X, min_Y, max_Y, min_Z, max_Z, min_X_2, max_X_2, min_Y_2, max_Y_2, min_Z_2, max_Z_2, angle_check = XMConvertToRadians(180.0f);
	bool _collidedX, _collidedY, _collidedZ;
};

