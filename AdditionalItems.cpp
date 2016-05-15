#include "AdditionalItems.h"
#include <math.h>

float convertDegtoRad(float angle)
{
	return angle * (float)PI / 180.f;
}

bool solveLinearEquations(
	float a1, float b1, float c1, float d1,
	float a2, float b2, float c2, float d2,
	float a3, float b3, float c3, float d3,
	float *u, float *v, float* w)
{
	float det1 = b1 * c2 - c1 * b2;
	if (fabs(det1) < EPS)
		return false;
	float det2 = a1 * c2 - c1 * a2;
	float det3 = a2 * c3 - a3 * c2;
	float det4 = b2 * c3 - b3 * c2;

	float denom = det2 * det4 - det1 * det3;

	if (fabs(denom) < EPS)
		return false;

	float tmp1 = d1 * c2 - d2 * c1;
	float tmp2 = d2 * c3 - c2 * d3;

	*u = (det1 * tmp2 - det4 * tmp1) / denom;
	*v = -(det2 * *u + tmp1) / det1;
	*w = -(a1 * *u + b1 * *v + d1) / c1;

	return true;
}

Roots solveQuadraticEquation(float a, float b, float c)
{
	Roots	rootsVector;
	float	discriminant = b * b - 4 * a * c;
	float	root1, root2;

	if (a == 0 && b == 0)
		return rootsVector;

	if (a == 0 && b != 0)
	{
		root1 = -c / b;
		rootsVector.push_back(root1);
		return rootsVector;
	}

	if (discriminant < 0)
		return rootsVector;
	else if (discriminant == 0)
	{
		root1 = -b / (2.0f * a);
		rootsVector.push_back(root1);
	}
	else
	{
		root1 = -b + sqrt(discriminant) / (2.0f * a);
		root2 = -b - sqrt(discriminant) / (2.0f * a);

		rootsVector.push_back(root1);
		rootsVector.push_back(root2);
	}

	return rootsVector;
}