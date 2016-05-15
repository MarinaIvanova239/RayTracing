#include "Vector3D.h"
#include <math.h>

Vector3D::Vector3D(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Vector3D substract(Vector3D v1, Vector3D v2)
{
	return Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3D sum(Vector3D v1, Vector3D v2)
{
	return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3D cross(Vector3D v1, Vector3D v2)
{
	float x1 = v1.y * v2.z - v1.z * v2.y;
	float y1 = v1.z * v2.x - v1.x * v2.z;
	float z1 = v1.x * v2.y - v1.y * v2.x;

	return Vector3D(x1, y1, z1);
}

Vector3D Vector3D::negate()
{
	return Vector3D(-x, -y, -z);
}

float dot(Vector3D v1, Vector3D v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vector3D::countVecLength()
{
	return sqrt(x * x + y * y + z * z);
}

float dist(Vector3D v1, Vector3D v2)
{
	return sqrt((v1.x - v2.x) * (v1.x - v2.x) + 
		(v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
}

Vector3D Vector3D::multiplyNumber(float n)
{
	return Vector3D(x * n, y * n, z * n);
}

Vector3D &Vector3D::normalize()
{
	float vLength = (*this).countVecLength();

	if (vLength > 0)
	{
		x /= vLength;
		y /= vLength;
		z /= vLength;
	}

	return *this;
}

Vector3D::~Vector3D()
{
}
