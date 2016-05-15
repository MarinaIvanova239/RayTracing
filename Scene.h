#ifndef _SCENECLASS_H_
#define _SCENECLASS_H_

#pragma once

#include <vector>
#include "Objects.h"
#include "Camera.h"

#define MAX_NAME_LENGTH 100

enum LightType
{
	NO_LIGHT = 0,
	POINT = 1,
	DIRECTIONAL = 2
};

class Light
{
public:
	Vector3D	direction;
	Vector3D	start;
	LightType	typeLight;
	Color		color;
};

typedef std::vector<Light> LightList;

class SceneClass
{
public:
	int			screenWidth, screenHeight;

public:
	int			resX, resY;
	int			traceDepth;
	float		depth;
	Camera		camera;
	ObjectsList activeObjects;
	LightList	sceneLight;

public:
	SceneClass();
	~SceneClass();
	void rayTracing(ImageClass* img);
	void countScreenParams();
	Color tracePath(Ray ray, int depth, int indObject);
};

#endif // _SCENECLASS_H_

