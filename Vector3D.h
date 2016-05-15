#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#pragma once

class Vector3D
{
public:
	float	x, y, z, w;

public:
	Vector3D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 1.0f);
	~Vector3D();

	float countVecLength();

	Vector3D negate();
	Vector3D &normalize();
	Vector3D multiplyNumber(float n);
};

Vector3D substract(Vector3D v1, Vector3D v2);
Vector3D cross(Vector3D v1, Vector3D v2);
Vector3D sum(Vector3D v1, Vector3D v2);
float dot(Vector3D v1, Vector3D v2);
float dist(Vector3D v1, Vector3D v2);

#endif // _VECTOR3D_H_

