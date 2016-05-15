#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#pragma once

#include "Vector3D.h"
#include "Matrix.h"

class Camera
{
private:
	Vector3D	position;
	Vector3D	direction;

	Matrix		rotateMatrix, translateMatrix;
	int			fovX, fovY;

public:
	Camera();
	~Camera();

	void setPosition(float _x, float _y, float _z);
	void setDirection(float _x, float _y, float _z);

	Matrix getTranslateMatrix() { return translateMatrix; };
	Matrix getRotateMatrix() { return rotateMatrix; };

	void setFOV(int _fovX, int _fovY) { fovX = _fovX; fovY = _fovY; };
	void getFOV(int* _fovX, int* _fovY) { *_fovX = fovX; *_fovY = fovY; };
};

#endif // _CAMERACLASS_H_

