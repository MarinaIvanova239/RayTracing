#ifndef _HANDLERCLASS_H_
#define _HANDLERCLASS_H_

#pragma once

#include "Image.h"
#include "Matrix.h"
#include "Scene.h"
#include <vector>

#define MAX_STRING				250
#define MAX_NAME_LENGTH			100

#define SCENE_LENGTH			8
#define RESOLUTION_X_LENGTH		15
#define RESOLUTION_Y_LENGTH		15
#define OUTPUT_LENGTH			9
#define TRACE_DEPTH_LENGTH		14

#define CAMERA_LENGTH			9
#define NODE_LENGTH				7

#define POSITION_LENGTH			10
#define ORIENTATION_LENGTH		13
#define FOV_X_LENGTH			7
#define FOV_Y_LENGTH			7

#define LCS_LENGTH				7
#define CSG_UNION_LENGTH		12
#define CSG_DIFFERENCE_LENGTH	17
#define CSG_INTERSECTION_LENGTH	19
#define PLANE_LENGTH			8
#define TRIANGLE_LENGTH			11
#define SPHERE_LENGTH			9
#define CYLINDER_LENGTH			11
#define CONE_LENGTH				7
#define OBJ_MODEL_LENGTH		12

#define LEFT_NODE_LENGTH		10
#define RIGHT_NODE_LENGTH		11

#define POINT_LENGTH			4
#define RADIUS_LENGTH			8
#define HEIGHT_LENGTH			8
#define FILE_NAME_LENGTH		11
#define COLOR_LENGTH			7
#define REFRACTION_LENGTH		12
#define REFLECTION_LENGTH		12
#define LIGHT_LENGTH			8

#define POINT_LIGHT				7
#define DIRECTIONAL_LIGHT		13

#define NUMBER_ARGUMENTS		6

struct ViewMatrix
{
	Matrix translate;
	Matrix rotate;
	Matrix scale;
};

typedef std::vector<int> NodeLevels;
typedef std::vector<ViewMatrix> MatrixList;

class HandlerClass
{
public:
	bool		wrongArguments;
	bool		wrongSceneFile;
	SceneClass	scene;
	char		buf[MAX_STRING];
	ImageClass	image;
	MatrixList	listMatrix;
	NodeLevels	nodeLevels;
	char		sceneFileName[MAX_NAME_LENGTH];
	char		outputFileName[MAX_NAME_LENGTH];
	FILE*		f;

public:
	HandlerClass();
	~HandlerClass();

	void working();
	void parseArgumentsString(int argc, char** argv);
	void parseSceneFile();
	bool parseCameraPart(bool* flagNew);
	bool parseLightPart(bool* flagNew);
	bool parseNodePart(bool isCSGNode, bool* flagNew);
	bool parseCSGNodePart(Objects* curObject, bool* flagNew);
	bool parseGeometryNode(Objects* curObject);
};

#endif //_HANDLERCLASS_H_
