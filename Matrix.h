#ifndef _MATRIX_H_
#define _MATRIX_H_

#pragma once

#include "Vector3D.h"

#define SIZE 4

class Matrix
{
private:
	float matrix[SIZE][SIZE];

public:
	Matrix(float x0 = 1.0f, float y0 = 0.0f, float z0 = 0.0f, float w0 = 0.0f,
		float x1 = 0.0f, float y1 = 1.0f, float z1 = 0.0f, float w1 = 0.0f,
		float x2 = 0.0f, float y2 = 0.0f, float z2 = 1.0f, float w2 = 0.0f,
		float x3 = 0.0f, float y3 = 0.0f, float z3 = 0.0f, float w3 = 1.0f);
	~Matrix();

	Matrix &setMatrixColumn(int numCol, float _n0, float _n1, float _n2, float _n3);
	Matrix &setUnitMatrix();
	Matrix multiplyMatrix(Matrix _m);
	Matrix &translate(float _tx, float _ty, float _tz);
	Matrix &scale(float _sx, float _sy, float _sz);
	Matrix &rotateX(float _angle);
	Matrix &rotateY(float _angle);
	Matrix &rotateZ(float _angle);

	Vector3D multiplyVectorFromRight(Vector3D v);
	float getElem(int numRow, int numColumn);
	void setElem(int numRow, int numColumn, float value);
};

#endif // _MATRIX_H_

