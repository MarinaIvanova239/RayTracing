#include "Objects.h"
#include "Handler.h"
#include "AdditionalItems.h"
#include <iostream>

#pragma warning(disable:4996)

using namespace std;

void Plane::setPoint(int numPoint, Vector3D _point)
{
	switch (numPoint)
	{
	case 1:
		point1 = _point;
		break;
	case 2:
		point2 = _point;
		break;
	case 3:
		point3 = _point;
		break;
	default:
		break;
	}
}

void Plane::setParams()
{
	Matrix view;
	view = view.multiplyMatrix(rotateMatrix).multiplyMatrix(scaleMatrix).multiplyMatrix(translateMatrix);

	point1 = view.multiplyVectorFromRight(point1);
	point2 = view.multiplyVectorFromRight(point2);
	point3 = view.multiplyVectorFromRight(point3);
}

bool Plane::findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest)
{
	Vector3D v1 = substract(point2, point1);
	Vector3D v2 = substract(point3, point2);
	Vector3D norm = cross(v1, v2);

	float t, v, w;
	bool result1, result2, result3;

	result1 = solveLinearEquations(
		ray.direction.x, point1.x - point2.x, point1.x - point3.x, -(point1.x - ray.start.x),
		ray.direction.y, point1.y - point2.y, point1.y - point3.y, -(point1.y - ray.start.y), 
		ray.direction.z, point1.z - point2.z, point1.z - point3.z, -(point1.z - ray.start.z), 
		&t, &v, &w);

	if (!result1)
	{
		result2 = solveLinearEquations(
			ray.direction.x, point2.x - point3.x, point2.x - point1.x, -(point2.x - ray.start.x),
			ray.direction.y, point2.y - point3.y, point2.y - point1.y, -(point2.y - ray.start.y),
			ray.direction.z, point2.z - point3.z, point2.z - point1.z, -(point2.z - ray.start.z),
			&t, &v, &w);

		if (!result2)
		{
			result3 = solveLinearEquations(
				ray.direction.x, point3.x - point1.x, point3.x - point2.x, -(point3.x - ray.start.x),
				ray.direction.y, point3.y - point1.y, point3.y - point2.y, -(point3.y - ray.start.y),
				ray.direction.z, point3.z - point1.z, point3.z - point2.z, -(point3.z - ray.start.z),
				&t, &v, &w);

			if (!result3)
				return false;
		}
	}

	*_point = sum(ray.start, (ray.direction).multiplyNumber(t));
	*_norm = norm.normalize();

	return true;
}

void Triangle::setPoint(int numPoint, Vector3D _point)
{
	switch (numPoint)
	{
	case 1:
		point1 = _point;
		break;
	case 2:
		point2 = _point;
		break;
	case 3:
		point3 = _point;
		break;
	default:
		break;
	}
}

void Triangle::setParams()
{
	Matrix view;
	view = view.multiplyMatrix(rotateMatrix).multiplyMatrix(scaleMatrix).multiplyMatrix(translateMatrix);

	point1 = view.multiplyVectorFromRight(point1);
	point2 = view.multiplyVectorFromRight(point2);
	point3 = view.multiplyVectorFromRight(point3);
}

bool Triangle::findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest)
{
	Vector3D v1 = substract(point2, point1);
	Vector3D v2 = substract(point3, point2);
	Vector3D norm = cross(v1, v2);
	Vector3D interPoint;

	float t, v, w;
	bool result1, result2, result3;

	result1 = solveLinearEquations(
		ray.direction.x, point1.x - point2.x, point1.x - point3.x, -(point1.x - ray.start.x),
		ray.direction.y, point1.y - point2.y, point1.y - point3.y, -(point1.y - ray.start.y),
		ray.direction.z, point1.z - point2.z, point1.z - point3.z, -(point1.z - ray.start.z),
		&t, &v, &w);

	if (!result1)
	{
		result2 = solveLinearEquations(
			ray.direction.x, point2.x - point3.x, point2.x - point1.x, -(point2.x - ray.start.x),
			ray.direction.y, point2.y - point3.y, point2.y - point1.y, -(point2.y - ray.start.y),
			ray.direction.z, point2.z - point3.z, point2.z - point1.z, -(point2.z - ray.start.z),
			&t, &v, &w);

		if (!result2)
		{
			result3 = solveLinearEquations(
				ray.direction.x, point3.x - point1.x, point3.x - point2.x, -(point3.x - ray.start.x),
				ray.direction.y, point3.y - point1.y, point3.y - point2.y, -(point3.y - ray.start.y),
				ray.direction.z, point3.z - point1.z, point3.z - point2.z, -(point3.z - ray.start.z),
				&t, &v, &w);

			if (!result3)
				return false;
		}
	}

	if (!((v >= 0.0) && (w >= 0.0) && (v + w <= 1.0) && (t >= EPS)))
		return false;

	if (t < EPS)
		return false;

	*_point = sum(ray.start, (ray.direction).multiplyNumber(t));
	*_norm = norm.normalize();

	return true;
}

void Sphere::setParams()
{
	center = translateMatrix.multiplyVectorFromRight(center);
	radius = scaleMatrix.multiplyVectorFromRight(Vector3D(radius, radius, radius)).x;
}


bool Sphere::findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest)
{
	Roots		roots;
	Vector3D	interPoint;
	float		t0, t1;
	Vector3D	l = substract(center, ray.start);
	float		tca, thc, h2;

	tca = dot(l, ray.direction);
	if (tca < 0) 
		return false;

	h2 = dot(l, l) - tca * tca;

	if (h2 > radius * radius) 
		return false;

	thc = sqrt(radius * radius - h2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 < t1 && findClosest || t1 < t0 && !findClosest)
		interPoint = sum(ray.start, (ray.direction).multiplyNumber(t0));
	else
		interPoint = sum(ray.start, (ray.direction).multiplyNumber(t1));

	*_point = interPoint;
	*_norm = substract(interPoint, center).normalize();

	return true;
}

void Cylinder::setParams()
{
	center = translateMatrix.multiplyVectorFromRight(center);
	radius = scaleMatrix.multiplyVectorFromRight(Vector3D(radius, radius, height)).x;
	height = scaleMatrix.multiplyVectorFromRight(Vector3D(radius, radius, height)).z;
}

bool Cylinder::findDiskIntersection(Ray ray, float _height, Vector3D* _point, Vector3D* _norm)
{
	Vector3D	interPoint;

	float z = (_height - ray.start.z) / ray.direction.z;

	if (z < EPS)
		return false;

	interPoint = sum(ray.start, (ray.direction).multiplyNumber(z));

	if (interPoint.x * interPoint.x + interPoint.y * interPoint.y > radius * radius)
		return false;

	*_point = interPoint;
	*_norm = Vector3D(0.0f, 0.0f, (_height > 0) ? 1.0f : -1.0f);

	return true;
}

bool Cylinder::findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest)
{
	Roots		roots;
	Vector3D	interPoint, interNorm;
	Vector3D	interPoint1, interNorm1;
	Vector3D	interPoint2, interNorm2;
	float		t0, t1;
	bool		result1 = true, result2 = true;

	if (fabs(ray.direction.z) > EPS)
	{
		result1 = findDiskIntersection(ray, height / 2, &interPoint1, &interNorm1);
		result2 = findDiskIntersection(ray, -height / 2, &interPoint2, &interNorm2);

		if (result1 && result2)
		{
			float dist1 = dist(ray.start, interPoint1);
			float dist2 = dist(ray.start, interPoint2);

			if (dist1 < dist2 && findClosest || dist2 > dist1 && !findClosest)
			{
				*_point = interPoint1;
				*_norm = interNorm1;
			}
			else
			{
				*_point = interPoint2;
				*_norm = interNorm2;
			}
			return true;
		}
		else if (result1 && !result2)
		{
			*_point = interPoint1;
			*_norm = interNorm1;
		}
		else if (result2 && !result1)
		{
			*_point = interPoint2;
			*_norm = interNorm2;
		}
	}

	float a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y;
	float b = 2.0f * (ray.start.x * ray.direction.x + ray.start.y * ray.direction.y);
	float c = ray.start.x * ray.start.x + ray.start.y * ray.start.y - radius * radius; 

	roots = solveQuadraticEquation(a, b, c);

	if (roots.size() == 0)
		return false;
	if (roots.size() == 1)
	{
		t0 = roots.front();
		if (t0 < EPS)
		{
			if (result1 || result2)
				return true;
			return false;
		}
		interPoint = sum(ray.start, (ray.direction).multiplyNumber(t0));

		if (fabs(interPoint.z) < height)
		{
			if (result1 || result2)
			{
				float dist1 = dist(ray.start, interPoint);
				float dist2 = dist(ray.start, *_point);
				if (dist1 < dist2)
				{
					*_point = interPoint;
					*_norm = Vector3D(interPoint.x, interPoint.y, 0.0f).normalize();
				}
			}
			else
			{
				*_point = interPoint;
				*_norm = Vector3D(interPoint.x, interPoint.y, 0.0f).normalize();
			}
		}
	}
	else if (roots.size() == 2)
	{
		t0 = roots.front();
		t1 = roots.back();
		if (t0 < EPS)
			t0 = t1;
		if (t1 < EPS)
		{
			if (result1 || result2)
				return true;
			return false;
		}

		if (t0 < t1 && findClosest || t0 > t1 && !findClosest)
			interPoint = sum(ray.start, (ray.direction).multiplyNumber(t0));
		else
			interPoint = sum(ray.start, (ray.direction).multiplyNumber(t1));

		if (fabs(interPoint.z) < height)
		{
			if (result1 || result2)
			{
				float dist1 = dist(ray.start, interPoint);
				float dist2 = dist(ray.start, *_point);
				if (dist1 < dist2)
				{
					*_point = interPoint;
					*_norm = Vector3D(interPoint.x, interPoint.y, 0.0f).normalize();
				}
			}
			else
			{
				*_point = interPoint;
				*_norm = Vector3D(interPoint.x, interPoint.y, 0.0f).normalize();
			}
		}
	}

	return true;
}

void Cone::setParams()
{
	center = translateMatrix.multiplyVectorFromRight(center);
	radius = scaleMatrix.multiplyVectorFromRight(Vector3D(radius, height, radius)).x;
	height = scaleMatrix.multiplyVectorFromRight(Vector3D(radius, height, radius)).y;
}

bool Cone::findDiskIntersection(Ray ray, Vector3D* _point, Vector3D* _norm)
{
	Vector3D	interPoint;

	float z = (height - ray.start.z) / ray.direction.z;

	if (z < EPS)
		return false;

	interPoint = sum(ray.start, (ray.direction).multiplyNumber(z));

	if (interPoint.x * interPoint.x + interPoint.y * interPoint.y > radius * radius)
		return false;

	*_point = interPoint;
	*_norm = Vector3D(0.0f, 0.0f, 1.0f);

	return true;
}

bool Cone::findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest)
{
	Roots		roots;
	Vector3D	interPoint, interNorm;
	bool		result = true;

	if (fabs(ray.direction.z) > EPS)
	{
		result = findDiskIntersection(ray, &interPoint, &interNorm);

		*_point = interPoint;
		*_norm = interNorm;

		if (result)
			return true;
	}

	return false;
}

void ObjModel::loadObjModel()
{
	FILE	   *file;
	char		buf[MAX_STRING];
	bool		useTexture = false;
	bool		useNormals = false;
	Objects*	curObject;
	VectorList  vertices;

	float		x = 0.0f, y = 0.0f, z = 0.0f;
	int			v1 = 0, v2 = 0, v3 = 0;
	int			text = 0, n = 0;

	file = fopen(fileName, "rb");
	if (file == NULL)
		return;

	while (fgets(buf, MAX_STRING, file) != NULL)
	{
		if (buf[0] == '#' || buf[0] == '\n')
			continue;
		if (buf[0] == 'v')
		{
			switch (buf[1])
			{
			case ' ':
				sscanf_s(buf + 2, "%f %f %f", &x, &y, &z);
				vertices.push_back(Vector3D(x, y, z));
				break;
			case 'n':
				useNormals = true;
				break;
			case 't':
				break;
			default:
				break;
			}
		}
		else if (buf[0] == 'f')
		{
			curObject = new Triangle();
			if (useTexture && useNormals)
				sscanf_s(buf + 2, "%i/%i/%i %i/%i/%i %i/%i/%i", &v1, &text, &n, &v2, &text, &n, &v3, &text, &n);
			else if (!useTexture && useNormals)
				sscanf_s(buf + 2, "%i//%i %i//%i %i//%i", &v1, &n, &v2, &n, &v3, &n);
			else if (useTexture && !useNormals)
				sscanf_s(buf + 2, "%i/%i %i/%i %i/%i", &v1, &text, &v2, &text, &v3, &text);
			else
				sscanf_s(buf + 2, "%i %i %i", &v1, &v2, &v3);
			((Triangle*)curObject)->setPoint(1, vertices[v1 - 1]);
			((Triangle*)curObject)->setPoint(2, vertices[v2 - 1]);
			((Triangle*)curObject)->setPoint(3, vertices[v3 - 1]);
			elems.push_back(curObject);
		}
		else if (buf[0] == 'o')
			continue;
		else if (strncmp(buf, "usemtl", 6) == 0)
			useTexture = true;
		else
		{
			cout << "Wrong format of obj file" << endl;
			fileIsOK = false;
			break;
		}
	}

	fclose(file);
}

void ObjModel::setFileName(char _fileName[])
{
	strncpy(fileName, _fileName, MAX_NAME);
	loadObjModel();
}

void ObjModel::setParams()
{
	for (unsigned i = 0; i < elems.size(); i++)
		elems[i]->setParams();
}

bool ObjModel::findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest)
{
	Vector3D	interPoint, interNorm;
	float		minDist = -10.f;

	if (!fileIsOK)
		return false;

	for (unsigned i = 0; i < elems.size(); i++)
	{
		if (elems[i]->findIntersection(ray, &interPoint, &interNorm, findClosest))
		{
			float distance = dist(ray.start, interPoint);
			if (minDist == -10.f)
				minDist = distance;
			if (distance <= minDist)
			{
				*_point = interPoint;
				*_norm = interNorm;
				minDist = distance;
			}
		}
	}

	if (minDist == -10.f)
		return false;
	return true;
}

void DoubleObject::setObject(int numObject, Objects* _object)
{
	switch (numObject)
	{
	case 1:
		leftObject = _object;
		break;
	case 2:
		rightObject = _object;
		break;
	default:
		break;
	}
}

void DoubleObject::setParams()
{
	leftObject->setParams();
	rightObject->setParams();

	color = leftObject->color;
}

bool DoubleObject::findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest)
{
	Vector3D	interPoint1, interNorm1;
	Vector3D	interPoint2, interNorm2;
	bool		result1, result2;

	result1 = leftObject->findIntersection(ray, &interPoint1, &interNorm1, true);
	result2 = rightObject->findIntersection(ray, &interPoint2, &interNorm2, true);

	if (!result1 && !result2)
		return false;

	switch (typeCSG)
	{
	case CSG_UNION:
		if (result1 && !result2)
		{
			*_point = interPoint1;
			*_norm = interNorm1;
		}
		else if (!result1 && result2)
		{
			*_point = interPoint2;
			*_norm = interNorm2;
		}
		else
		{
			float dist1 = dist(ray.start, interPoint1);
			float dist2 = dist(ray.start, interPoint2);
			if (dist1 < dist2)
			{
				*_point = interPoint1;
				*_norm = interNorm1;
			}
			else
			{
				*_point = interPoint2;
				*_norm = interNorm2;
			}
		}
		break;
	case CSG_INTERSECTION:
		if (result1 && result2)
		{
			float dist1 = dist(ray.start, interPoint1);
			float dist2 = dist(ray.start, interPoint2);
			if (dist1 < dist2)
			{
				*_point = interPoint2;
				*_norm = interNorm2;
			}
			else
			{
				*_point = interPoint1;
				*_norm = interNorm1;
			}
		}
		else
			return false;
		break;
	case CSG_DIFFERENCE:
		if (result1 && !result2)
		{
			*_point = interPoint1;
			*_norm = interNorm1;
		}
		else if (result1 && result2)
		{
			float dist1 = dist(ray.start, interPoint1);
			float dist2 = dist(ray.start, interPoint2);

			if (dist1 < dist2)
			{
				*_point = interPoint1;
				*_norm = interNorm1;
			}
			else if (dist1 > dist2)
			{
				Vector3D	tempPoint1, tempNorm1;
				Vector3D	tempPoint2, tempNorm2;
				bool		r1, r2;

				r1 = leftObject->findIntersection(ray, &tempPoint1, &tempNorm1, false);
				r2 = rightObject->findIntersection(ray, &tempPoint2, &tempNorm2, false);

				float d1 = dist(ray.start, tempPoint1);
				float d2 = dist(ray.start, tempPoint2);

				if (d1 <= d2)
					return false;
				else if (d1 > d2)
				{
					*_point = tempPoint2;
					*_norm = tempNorm2.negate();
				}
			}
			else if (dist1 == dist2)
				return false;
		}
		else
			return false;
		break;
	default:
		break;
	}

	return true;
}