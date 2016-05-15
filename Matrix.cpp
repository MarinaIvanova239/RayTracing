#include "Matrix.h"
#include <math.h>
#include "AdditionalItems.h"

Matrix::Matrix(float x0, float y0, float z0, float w0,
	float x1, float y1, float z1, float w1,
	float x2, float y2, float z2, float w2,
	float x3, float y3, float z3, float w3)
{
	matrix[0][0] = x0;
	matrix[0][1] = y0;
	matrix[0][2] = z0;
	matrix[0][3] = w0;

	matrix[1][0] = x1;
	matrix[1][1] = y1;
	matrix[1][2] = z1;
	matrix[1][3] = w1;

	matrix[2][0] = x2;
	matrix[2][1] = y2;
	matrix[2][2] = z2;
	matrix[2][3] = w2;

	matrix[3][0] = x3;
	matrix[3][1] = y3;
	matrix[3][2] = z3;
	matrix[3][3] = w3;
}

void Matrix::setElem(int numRow, int numColumn, float value)
{
	matrix[numRow][numColumn] = value;
}

float Matrix::getElem(int numRow, int numColumn)
{
	return matrix[numRow][numColumn];
}

Matrix &Matrix::setMatrixColumn(int numCol, float _n0, float _n1, float _n2, float _n3)
{
	matrix[0][numCol] = _n0;
	matrix[1][numCol] = _n1;
	matrix[2][numCol] = _n2;
	matrix[3][numCol] = _n3;

	return *this;
}

Matrix &Matrix::setUnitMatrix()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (i == j)
				matrix[i][j] = 1;
			else
				matrix[i][j] = 0;
		}
	}

	return *this;
}

Vector3D Matrix::multiplyVectorFromRight(Vector3D v)
{
	return Vector3D(v.x * matrix[0][0] + v.y * matrix[1][0] + v.z * matrix[2][0] + v.w * matrix[3][0],
		v.x * matrix[0][1] + v.y * matrix[1][1] + v.z * matrix[2][1] + v.w * matrix[3][1],
		v.x * matrix[0][2] + v.y * matrix[1][2] + v.z * matrix[2][2] + v.w * matrix[3][2]);
}

Matrix Matrix::multiplyMatrix(Matrix _m)
{
	Matrix newMatrix;

	newMatrix = Matrix(
		matrix[0][0] * _m.getElem(0, 0) + matrix[0][1] * _m.getElem(1, 0) + matrix[0][2] * _m.getElem(2, 0) + matrix[0][3] * _m.getElem(3, 0),
		matrix[0][0] * _m.getElem(0, 1) + matrix[0][1] * _m.getElem(1, 1) + matrix[0][2] * _m.getElem(2, 1) + matrix[0][3] * _m.getElem(3, 1),
		matrix[0][0] * _m.getElem(0, 2) + matrix[0][1] * _m.getElem(1, 2) + matrix[0][2] * _m.getElem(2, 2) + matrix[0][3] * _m.getElem(3, 2),
		matrix[0][0] * _m.getElem(0, 3) + matrix[0][1] * _m.getElem(1, 3) + matrix[0][2] * _m.getElem(2, 3) + matrix[0][3] * _m.getElem(3, 3),

		matrix[1][0] * _m.getElem(0, 0) + matrix[1][1] * _m.getElem(1, 0) + matrix[1][2] * _m.getElem(2, 0) + matrix[1][3] * _m.getElem(3, 0),
		matrix[1][0] * _m.getElem(0, 1) + matrix[1][1] * _m.getElem(1, 1) + matrix[1][2] * _m.getElem(2, 1) + matrix[1][3] * _m.getElem(3, 1),
		matrix[1][0] * _m.getElem(0, 2) + matrix[1][1] * _m.getElem(1, 2) + matrix[1][2] * _m.getElem(2, 2) + matrix[1][3] * _m.getElem(3, 2),
		matrix[1][0] * _m.getElem(0, 3) + matrix[1][1] * _m.getElem(1, 3) + matrix[1][2] * _m.getElem(2, 3) + matrix[1][3] * _m.getElem(3, 3),

		matrix[2][0] * _m.getElem(0, 0) + matrix[2][1] * _m.getElem(1, 0) + matrix[2][2] * _m.getElem(2, 0) + matrix[2][3] * _m.getElem(3, 0),
		matrix[2][0] * _m.getElem(0, 1) + matrix[2][1] * _m.getElem(1, 1) + matrix[2][2] * _m.getElem(2, 1) + matrix[2][3] * _m.getElem(3, 1),
		matrix[2][0] * _m.getElem(0, 2) + matrix[2][1] * _m.getElem(1, 2) + matrix[2][2] * _m.getElem(2, 2) + matrix[2][3] * _m.getElem(3, 2),
		matrix[2][0] * _m.getElem(0, 3) + matrix[2][1] * _m.getElem(1, 3) + matrix[2][2] * _m.getElem(2, 3) + matrix[2][3] * _m.getElem(3, 3),

		matrix[3][0] * _m.getElem(0, 0) + matrix[3][1] * _m.getElem(1, 0) + matrix[3][2] * _m.getElem(2, 0) + matrix[3][3] * _m.getElem(3, 0),
		matrix[3][0] * _m.getElem(0, 1) + matrix[3][1] * _m.getElem(1, 1) + matrix[3][2] * _m.getElem(2, 1) + matrix[3][3] * _m.getElem(3, 1),
		matrix[3][0] * _m.getElem(0, 2) + matrix[3][1] * _m.getElem(1, 2) + matrix[3][2] * _m.getElem(2, 2) + matrix[3][3] * _m.getElem(3, 2),
		matrix[3][0] * _m.getElem(0, 3) + matrix[3][1] * _m.getElem(1, 3) + matrix[3][2] * _m.getElem(2, 3) + matrix[3][3] * _m.getElem(3, 3)
		);
	
	return newMatrix;
}

Matrix &Matrix::translate(float _tx, float _ty, float _tz)
{
	Matrix trMatrix;

	trMatrix.setElem(3, 0, _tx);
	trMatrix.setElem(3, 1, _ty);
	trMatrix.setElem(3, 2, _tz);

	*this = trMatrix.multiplyMatrix(*this);

	return *this;
}

Matrix &Matrix::scale(float _sx, float _sy, float _sz)
{
	Matrix scMatrix;

	scMatrix.setElem(0, 0, _sx);
	scMatrix.setElem(1, 1, _sy);
	scMatrix.setElem(2, 2, _sz);

	*this = scMatrix.multiplyMatrix(*this);

	return *this;
}

Matrix &Matrix::rotateX(float _angle)
{
	Matrix	rotMatrix;
	float	radAngle = convertDegtoRad(_angle);
	float	sinValue = sin(radAngle);
	float	cosValue = cos(radAngle);

	rotMatrix.setElem(1, 1, cosValue);
	rotMatrix.setElem(1, 2, -sinValue);
	rotMatrix.setElem(2, 1, sinValue);
	rotMatrix.setElem(2, 2, cosValue);

	*this = rotMatrix.multiplyMatrix(*this);

	return *this;
}

Matrix &Matrix::rotateY(float _angle)
{
	Matrix	rotMatrix;
	float	radAngle = convertDegtoRad(_angle);
	float	sinValue = sin(radAngle);
	float	cosValue = cos(radAngle);

	rotMatrix.setElem(0, 0, cosValue);
	rotMatrix.setElem(0, 2, sinValue);
	rotMatrix.setElem(2, 0, -sinValue);
	rotMatrix.setElem(2, 2, cosValue);

	*this = rotMatrix.multiplyMatrix(*this);

	return *this;
}

Matrix &Matrix::rotateZ(float _angle)
{
	Matrix	rotMatrix;
	float	radAngle = convertDegtoRad(_angle);
	float	sinValue = sin(radAngle);
	float	cosValue = cos(radAngle);

	rotMatrix.setElem(0, 0, cosValue);
	rotMatrix.setElem(0, 1, -sinValue);
	rotMatrix.setElem(1, 0, sinValue);
	rotMatrix.setElem(1, 1, cosValue);

	*this = rotMatrix.multiplyMatrix(*this);

	return *this;
}

Matrix::~Matrix()
{
}
