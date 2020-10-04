#pragma once
#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include <vector>

using namespace std;
using namespace DirectX;

class Vector3D
{
public:
	
	Vector3D(float q, float w, float e);
	Vector3D(XMFLOAT3 float3);
	Vector3D();	
	~Vector3D();
	
	bool VectorEqual(XMFLOAT3 vector);
	float DotProduct(XMFLOAT3 vector);
	XMFLOAT3 CrossProduct(XMFLOAT3 vector);
	float Magnitude();
	XMFLOAT3 Normalization();
	XMFLOAT3 MultiplyByFloat(float number);
	XMFLOAT3 DivideByFloat(float divisor);
	XMFLOAT3 AddVector(XMFLOAT3 vector);
	XMFLOAT3 SubstractVector(XMFLOAT3 vector);
	XMFLOAT3 GetVector3D() { return XMFLOAT3(x, y, z); }
	XMFLOAT3 MulpitplyByMatrix_3X3(XMFLOAT3X3 matrix);
	void SetVector3D(float q, float w, float e) { x = q; y = w; z = e; }
	void SetVector3D(XMFLOAT3 vector) { x = vector.x; y = vector.y; z = vector.z; }

private:
	float x;
	float y;
	float z;
};

