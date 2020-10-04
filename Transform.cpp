#include "Transform.h"


Transform::Transform()
{
	_position.reset(new Vector3D());
	_rotation.reset(new Vector3D());
	_scale.reset(new Vector3D(1.0f, 1.0f, 1.0f));
	_parent.reset();
}


Transform::~Transform()
{
}

void Transform::Update(float t)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_scale->GetVector3D().x, _scale->GetVector3D().y, _scale->GetVector3D().z);
	XMMATRIX rotation = XMMatrixRotationX(_rotation->GetVector3D().x) * XMMatrixRotationY(_rotation->GetVector3D().y) * XMMatrixRotationZ(_rotation->GetVector3D().z);
	//rotation = XMMatrixMultiply(rotation, _angularRotation);
	XMMATRIX translation = XMMatrixTranslation(_position->GetVector3D().x, _position->GetVector3D().y, _position->GetVector3D().z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);

	if (_parent.get() != nullptr)
	{
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetWorldMatrix());
	}
}