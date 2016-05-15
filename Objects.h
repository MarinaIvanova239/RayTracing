#ifndef _OBJECTSCLASS_H_
#define _OBJECTSCLASS_H_

#pragma once

#include "Matrix.h"
#include "Image.h"

#include <vector>

#define MAX_NAME	100
#define EPS			0.0001f

enum ObjType
{
	NO_TYPE = 0,
	PLANE = 1,
	TRIANGLE = 2,
	SPHERE = 3,
	CYLINDER = 4,
	CONE = 5,
	OBJ_MODEL = 6,
	DOUBLE_OBJ = 7
};

enum CSGType
{
	CSG_NO_TYPE = 0,
	CSG_UNION = 1,
	CSG_DIFFERENCE = 2,
	CSG_INTERSECTION = 3
};


struct Ray
{
	Vector3D direction;
	Vector3D start;
};

typedef std::vector<Vector3D> VectorList;

class Objects
{
public:
	ObjType			typeObject;
	Matrix			translateMatrix, rotateMatrix, scaleMatrix;
	Color			color;
	bool			isRefractive;
	bool			isReflective;
	float			refractCoeff;

public:
	Objects() { typeObject = NO_TYPE; isRefractive = false; isReflective = false; };
	virtual bool findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest) = 0 {};
	virtual void setParams() = 0 {};
};

class Plane : public Objects
{
private:
	Vector3D point1, point2, point3;

public:
	Plane() { typeObject = PLANE; };
	~Plane();
	void setPoint(int numPoint, Vector3D _point);
	bool findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest);
	void setParams();
};

class Triangle : public Objects
{
private:
	Vector3D point1, point2, point3;

public:
	Triangle() { typeObject = TRIANGLE; };
	~Triangle();
	void setPoint(int numPoint, Vector3D _point);
	bool findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest);
	void setParams();
};

class Sphere : public Objects
{
private:
	float		radius;
	Vector3D	center;

public:
	Sphere() { typeObject = SPHERE; };
	~Sphere();
	void setRadius(float _radius) { radius = _radius; };
	bool findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest);
	void setParams();
};

class Cylinder : public Objects
{
private:
	float		radius;
	float		height;
	Vector3D	center;

public:
	Cylinder() { typeObject = CYLINDER; };
	~Cylinder();
	void setRadius(float _radius) { radius = _radius; };
	void setHeight(float _height) { height = _height; };
	bool findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest);
	bool findDiskIntersection(Ray ray, float _height, Vector3D* _point, Vector3D* _norm);
	void setParams();
};

class Cone : public Objects
{
private:
	float		radius;
	float		height;
	Vector3D	center;

public:
	Cone() { typeObject = CONE; };
	~Cone();
	void setRadius(float _radius) { radius = _radius; };
	void setHeight(float _height) { height = _height; };
	bool findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest);
	bool findDiskIntersection(Ray ray, Vector3D* _point, Vector3D* _norm);
	void setParams();
};

typedef std::vector<Objects*> ObjectsList;

class ObjModel : public Objects
{
private:
	char		fileName[MAX_NAME];
	ObjectsList elems;
	bool		fileIsOK;

public:
	ObjModel() { typeObject = OBJ_MODEL; fileIsOK = true; };
	~ObjModel() { elems.clear(); };
	void setFileName(char _fileName[]);
	void loadObjModel();
	bool findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest);
	void setParams();
};

class DoubleObject : public Objects
{
private:
	Objects	   *leftObject, *rightObject;
	CSGType		typeCSG;

public:
	DoubleObject() { typeObject = DOUBLE_OBJ; };
	~DoubleObject();
	void setObject(int numObject, Objects* _object);
	void setType(CSGType _typeCSG) { typeCSG = _typeCSG; };
	CSGType getType() { return typeCSG; };
	bool findIntersection(Ray ray, Vector3D* _point, Vector3D* _norm, bool findClosest);
	void setParams();
};

#endif // _OBJECTSCLASS_H_

