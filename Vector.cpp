#include "Vector.h"



Vector3D::Vector3D(float q, float w, float e)
{
	x = q;
	y = w;
	z = e;
}
Vector3D::Vector3D(XMFLOAT3 float3)
{
	x = float3.x;
	y = float3.y;
	z = float3.z;
}
Vector3D::Vector3D()
{
	x = y = z = NULL;
}

Vector3D::~Vector3D()
{
	
}

float Vector3D::DotProduct(XMFLOAT3 vector)
{
	return (x * vector.x + y * vector.y + z + vector.z);
}
XMFLOAT3 Vector3D::CrossProduct(XMFLOAT3 vector)
{
	return XMFLOAT3(y * vector.z - z * vector.y, x * vector.z - z * vector.x , x * vector.y - y * vector.x);
}

float Vector3D::Magnitude()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

XMFLOAT3 Vector3D::Normalization()
{
	return XMFLOAT3(x / Magnitude(), y / Magnitude(), z / Magnitude());
}

bool Vector3D::VectorEqual(XMFLOAT3 vector)
{
	if (vector.x == x && vector.y == y && vector.z == z)
		return true;
	else
		return false;
}

XMFLOAT3 Vector3D::MultiplyByFloat(float number)
{
	return XMFLOAT3(x * number, y * number, z * number);
}

XMFLOAT3 Vector3D::DivideByFloat(float number)
{
	return XMFLOAT3(x / number, y / number, z / number);
}

XMFLOAT3 Vector3D::AddVector(XMFLOAT3 vector)
{
	return XMFLOAT3(vector.x + x, vector.y + y, vector.z + z);
}

XMFLOAT3 Vector3D::SubstractVector(XMFLOAT3 vector)
{
	return XMFLOAT3(x - vector.x, y - vector.y, z - vector.z);
}

XMFLOAT3  Vector3D::MulpitplyByMatrix_3X3(XMFLOAT3X3 matrix)
{
	return XMFLOAT3(matrix._11 * x + matrix._12 * y + matrix._13 * z, matrix._21 * x + matrix._22 * y + matrix._23 * z, matrix._31 * x + matrix._32 * y + matrix._33 * z);
}