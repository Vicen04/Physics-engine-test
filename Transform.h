#pragma once
#include "Vector.h"

class Transform
{
public:
	Transform();
	~Transform();
	void Update(float t);

	// Setters and Getters for position/rotation/scale
	void SetPosition(XMFLOAT3 position) { _position->SetVector3D(position); }
	void SetPosition(float x, float y, float z) { _position->SetVector3D(x, y, z); }

	XMFLOAT3 GetPosition() const { return _position->GetVector3D(); }

	void SetScale(XMFLOAT3 scale) { _scale->SetVector3D(scale); }
	void SetScale(float x, float y, float z) { _scale->SetVector3D(x, y, z); }

	XMFLOAT3 GetScale() const { return  _scale->GetVector3D(); }

	void SetRotation(XMFLOAT3 rotation) { _rotation->SetVector3D(rotation); }
	void SetRotation(float x, float y, float z) { _rotation->SetVector3D(x, y, z); }

	XMFLOAT3 GetRotation() const { return _rotation->GetVector3D(); }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void SetParentWorld(Transform* parent) { _parent.reset(parent); }

private:
	shared_ptr<Vector3D> _position, _rotation, _scale;
	XMFLOAT4X4 _world;
	shared_ptr<Transform> _parent;
};

