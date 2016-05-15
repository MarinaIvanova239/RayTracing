#ifndef _ADDITIONALITEMS_H_
#define _ADDITIONALITEMS_H_

#pragma once

#include <vector>

#define PI			3.141592653589793238462643383279502884197169399375105820974944
#define EPS			0.0001f

typedef std::vector<float> Roots;

float convertDegtoRad(float angle);
Roots solveQuadraticEquation(float a, float b, float c);

bool solveLinearEquations(float a1, float b1, float c1, float d1,
	float a2, float b2, float c2, float d2,
	float a3, float b3, float c3, float d3,
	float *u, float *v, float* w);

#endif // _ADDITIONALITEMS_H_
