#include "Handler.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace std;

#pragma warning(disable:4996)

HandlerClass::HandlerClass()
{
	wrongArguments = false;
	wrongSceneFile = false;
}

void HandlerClass::parseArgumentsString(int argc, char** argv)
{
	if (argc == NUMBER_ARGUMENTS)
	{
		for (int i = 1; i < argc; i++)
		{
			int argLength = strlen(argv[i]);

			if (strncmp(argv[i], "--scene=", SCENE_LENGTH) == 0 && i == 1)
			{
				strncpy(sceneFileName, argv[i] + SCENE_LENGTH, argLength - SCENE_LENGTH);
				sceneFileName[argLength - SCENE_LENGTH] = 0;
			}
			else if (strncmp(argv[i], "--resolution_x=", RESOLUTION_X_LENGTH) == 0 && i == 2)
			{
				scene.resX = strtol(argv[i] + RESOLUTION_X_LENGTH, NULL, 10);
			}
			else if (strncmp(argv[i], "--resolution_y=", RESOLUTION_Y_LENGTH) == 0 && i == 3)
			{
				scene.resY = strtol(argv[i] + RESOLUTION_Y_LENGTH, NULL, 10);
			}
			else if (strncmp(argv[i], "--output=", OUTPUT_LENGTH) == 0 && i == 4)
			{
				strncpy(outputFileName, argv[i] + OUTPUT_LENGTH, argLength - OUTPUT_LENGTH);
				outputFileName[argLength - OUTPUT_LENGTH] = 0;
			}
			else if (strncmp(argv[i], "--trace_depth=", TRACE_DEPTH_LENGTH) == 0 && i == 5)
			{
				scene.traceDepth = strtol(argv[i] + TRACE_DEPTH_LENGTH, NULL, 10);
			}
			else
				wrongArguments = true;
		}
	}
	else
		wrongArguments = true;
}

bool HandlerClass::parseCameraPart(bool *flagNew)
{
	int		ind = 0;
	int		fovX = 0, fovY = 0;
	int		numElems = 0;

	while (fgets(buf, MAX_STRING, f) != NULL)
	{
		ind = 0;
		while (ind < MAX_STRING && (buf[ind] == ' ' || buf[ind] == '\t'))
			ind++;
		if (ind >= MAX_STRING)
			continue;
		if (buf[ind] == '#' || buf[ind] == '\n')
			continue;
		if (strncmp(buf + ind, "position: ", POSITION_LENGTH) == 0)
		{
			float posX = 0.0f, posY = 0.0f, posZ = 0.0f;

			sscanf(buf + ind + POSITION_LENGTH, "{x: %f, y: %f, z: %f}", &posX, &posY, &posZ);
			scene.camera.setPosition(posX, posY, posZ);
			numElems++;
		}
		else if (strncmp(buf + ind, "orientation: ", ORIENTATION_LENGTH) == 0)
		{
			float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;

			sscanf(buf + ind + ORIENTATION_LENGTH, "{h: %f, p: %f, r: %f}", &angleX, &angleY, &angleZ);
			scene.camera.setDirection(angleX, angleY, angleZ);
			numElems++;
		}
		else if(strncmp(buf + ind, "fov_x: ", FOV_X_LENGTH) == 0)
		{
			sscanf(buf + ind + FOV_X_LENGTH, "%i", &fovX);
			numElems++;
		}
		else if (strncmp(buf + ind, "fov_y: ", FOV_Y_LENGTH) == 0)
		{
			sscanf(buf + ind + FOV_Y_LENGTH, "%i", &fovY);
			numElems++;
		}
		else
			return false;

		if (numElems >= 4)
			break;
	}

	if (fovX > 0 && fovY > 0)
		scene.camera.setFOV(fovX, fovY);

	*flagNew = true;

	return true;
}

bool HandlerClass::parseCSGNodePart(Objects* curObject, bool* flagNew)
{
	int		ind = 0;

	while (buf[ind] != 'l')
	{
		if (fgets(buf, MAX_STRING, f) == NULL)
			return false;
		
		ind = 0;
		while (ind < MAX_STRING && (buf[ind] == ' ' || buf[ind] == '\t'))
			ind++;
		if (ind >= MAX_STRING)
			continue;
		if (buf[ind] == '#' || buf[ind] == '\n')
			continue;
	}
	
	if (strncmp(buf + ind, "left_node:", LEFT_NODE_LENGTH) == 0)
	{
		bool resultNode = parseNodePart(true, flagNew);
		if (!resultNode)
			return false;
		((DoubleObject*)curObject)->setObject(1, scene.activeObjects.back());
		scene.activeObjects.pop_back();
	}
	else
		return false;
	
	if (strncmp(buf + ind, "right_node:", RIGHT_NODE_LENGTH) == 0)
	{
		bool resultNode = parseNodePart(true, flagNew);
		if (!resultNode)
			return false;
		((DoubleObject*)curObject)->setObject(2, scene.activeObjects.back());
		scene.activeObjects.pop_back();
	}
	else
		return false;


	return true;
}

bool HandlerClass::parseGeometryNode(Objects* curObject)
{
	int		ind = 0;
	int		numElems = 0;
	ObjType type = curObject->typeObject;

	while (fgets(buf, MAX_STRING, f) != NULL)
	{
		ind = 0;
		while (ind < MAX_STRING && (buf[ind] == ' ' || buf[ind] == '\t'))
			ind++;
		if (ind >= MAX_STRING)
			continue;
		if (buf[ind] == '#' || buf[ind] == '\n')
			continue;

		if (strncmp(buf + ind, "color: ", COLOR_LENGTH) == 0)
		{
			float	r = 0, g = 0, b = 0;
			sscanf(buf + ind + COLOR_LENGTH, "(%f, %f, %f)", &r, &g, &b);
			curObject->color.setColor(r, g, b);
		}
		else if (strncmp(buf + ind, "refraction: ", REFRACTION_LENGTH) == 0)
		{
			float	refract = 0.0f;
			sscanf(buf + ind + REFRACTION_LENGTH, "%f", &refract);
			if (refract > 0.0f)
			{
				curObject->isRefractive = true;
				curObject->refractCoeff = refract;
			}
		}
		else if (strncmp(buf + ind, "reflection: ", REFLECTION_LENGTH) == 0)
		{
			if (strncmp(buf + ind + REFLECTION_LENGTH, "yes", 3) == 0)
				curObject->isReflective = true;
			else if (strncmp(buf + ind + REFLECTION_LENGTH, "no", 2) == 0)
				curObject->isReflective = false;
			else
				return false;
		}
		else
		{
			float	p1, p2, p3;
			float	radius, height;
			char	fileName[MAX_NAME];
			int		tempInd = 0;

			switch (type)
			{
			case PLANE:
				if (strncmp(buf + ind, "p0: ", POINT_LENGTH) == 0)
				{
					sscanf(buf + ind + POINT_LENGTH, "{x: %f, y: %f, z: %f}", &p1, &p2, &p3);
					((Plane*)curObject)->setPoint(1, Vector3D(p1, p2, p3));
				}
				else if (strncmp(buf + ind, "p1: ", POINT_LENGTH) == 0)
				{
					sscanf(buf + ind + POINT_LENGTH, "{x: %f, y: %f, z: %f}", &p1, &p2, &p3);
					((Plane*)curObject)->setPoint(2, Vector3D(p1, p2, p3));
				}
				else if (strncmp(buf + ind, "p2: ", POINT_LENGTH) == 0)
				{
					sscanf(buf + ind + POINT_LENGTH, "{x: %f, y: %f, z: %f}", &p1, &p2, &p3);
					((Plane*)curObject)->setPoint(3, Vector3D(p1, p2, p3));
				}
				else
					return false;
				break;
			case TRIANGLE:
				if (strncmp(buf + ind, "p0: ", POINT_LENGTH) == 0)
				{
					sscanf(buf + ind + POINT_LENGTH, "{x: %f, y: %f, z: %f}", &p1, &p2, &p3);
					((Triangle*)curObject)->setPoint(1, Vector3D(p1, p2, p3));
				}
				else if (strncmp(buf + ind, "p1: ", POINT_LENGTH) == 0)
				{
					sscanf(buf + ind + POINT_LENGTH, "{x: %f, y: %f, z: %f}", &p1, &p2, &p3);
					((Triangle*)curObject)->setPoint(2, Vector3D(p1, p2, p3));
				}
				else if (strncmp(buf + ind, "p2: ", POINT_LENGTH) == 0)
				{
					sscanf(buf + ind + POINT_LENGTH, "{x: %f, y: %f, z: %f}", &p1, &p2, &p3);
					((Triangle*)curObject)->setPoint(3, Vector3D(p1, p2, p3));
				}
				else
					return false;
				break;
			case SPHERE:
				if (strncmp(buf + ind, "radius: ", RADIUS_LENGTH) == 0)
				{
					sscanf(buf + ind + RADIUS_LENGTH, "%f", &radius);
					((Sphere*)curObject)->setRadius(radius);
				}
				else
					return false;
				break;
			case CYLINDER:
				if (strncmp(buf + ind, "radius: ", RADIUS_LENGTH) == 0)
				{
					sscanf(buf + ind + RADIUS_LENGTH, "%f", &radius);
					((Cylinder*)curObject)->setRadius(radius);
				}
				else if (strncmp(buf + ind, "height: ", HEIGHT_LENGTH) == 0)
				{
					sscanf(buf + ind + HEIGHT_LENGTH, "%f", &height);
					((Cylinder*)curObject)->setHeight(height);
				}
				else
					return false;
				break;
			case CONE:
				if (strncmp(buf + ind, "radius: ", RADIUS_LENGTH) == 0)
				{
					sscanf(buf + ind + RADIUS_LENGTH, "%f", &radius);
					((Cone*)curObject)->setRadius(radius);
				}
				else if (strncmp(buf + ind, "height: ", HEIGHT_LENGTH) == 0)
				{
					sscanf(buf + ind + HEIGHT_LENGTH, "%f", &height);
					((Cone*)curObject)->setHeight(height);
				}
				else
					return false;
				break;
			case OBJ_MODEL:
				if (strncmp(buf + ind, "file_name: ", FILE_NAME_LENGTH) == 0)
				{
					while (ind + 1 + tempInd < MAX_STRING && buf[ind + 1 + tempInd + FILE_NAME_LENGTH] != '"')
					{
						fileName[tempInd] = buf[ind + 1 + tempInd + FILE_NAME_LENGTH];
						tempInd++;
					}
					if (ind + 1 + tempInd >= MAX_STRING)
						return false;
					fileName[tempInd] = 0;
					((ObjModel*)curObject)->setFileName(fileName);
				}
				else
					return false;
				break;
			default:
				break;
			}
		}
		numElems++;

		if (numElems == 4 && (type == OBJ_MODEL || type == SPHERE))
			return true;
		if (numElems == 5 && (type == CONE || type == CYLINDER))
			return true;
		if (numElems == 6 && (type == PLANE || type == TRIANGLE))
			return true;
	}

	return true;
}

bool HandlerClass::parseNodePart(bool isCSGNode, bool* flagNew)
{
	int			    ind = 0;
	Objects		   *newObject = NULL;
	Matrix			translateObjectMatrix, rotateObjectMatrix, scaleObjectMatrix;
	ViewMatrix		structMatrix;
	bool			createNewObject;
	bool			nodeIsAdded;

	while (fgets(buf, MAX_STRING, f) != NULL)
	{
		ind = 0;
		createNewObject = false;
		nodeIsAdded = false;
		while (ind < MAX_STRING && (buf[ind] == ' ' || buf[ind] == '\t'))
			ind++;
		if (ind >= MAX_STRING)
			continue;
		if (buf[ind] == '#' || buf[ind] == '\n')
			continue;
		if (ind == 0)
		{
			*flagNew = false;
			if (newObject != NULL)
				scene.activeObjects.push_back(newObject);
			return true;
		}
		if (buf[ind] == '-')
		{
			if (strncmp(buf + ind, "- lcs: ", LCS_LENGTH) == 0)
			{
				float tX = 0.0f, tY = 0.0f, tZ = 0.0f;
				float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
				float sX = 0.0f, sY = 0.0f, sZ = 0.0f;

				sscanf(buf + ind + LCS_LENGTH, "{x: %f, y: %f, z: %f, h: %f, p: %f, r: %f, sx: %f, sy: %f, sz: %f}",
					&tX, &tY, &tZ, &angleX, &angleY, &angleZ, &sX, &sY, &sZ);

				translateObjectMatrix.setUnitMatrix();
				rotateObjectMatrix.setUnitMatrix();
				scaleObjectMatrix.setUnitMatrix();

				translateObjectMatrix.translate(tX, tY, tZ);
				rotateObjectMatrix.rotateX(angleX).rotateY(angleY).rotateZ(angleZ);
				scaleObjectMatrix.scale(sX, sY, sZ);

				if (listMatrix.size() != 0)
				{
					translateObjectMatrix.multiplyMatrix(listMatrix.back().translate);
					rotateObjectMatrix.multiplyMatrix(listMatrix.back().rotate);
					scaleObjectMatrix.multiplyMatrix(listMatrix.back().scale);
				}

				structMatrix.translate = translateObjectMatrix;
				structMatrix.rotate = rotateObjectMatrix;
				structMatrix.scale = scaleObjectMatrix;
			}
			else if (strncmp(buf + ind, "- node:", NODE_LENGTH) == 0)
			{
				if (newObject != NULL)
					scene.activeObjects.push_back(newObject);

				while (nodeLevels.size() != 0 && ind <= nodeLevels.back())
				{
					nodeLevels.pop_back();
					listMatrix.pop_back();
				}

				if (nodeLevels.size() == 0 || ind > nodeLevels.back())
				{
					nodeLevels.push_back(ind);
					listMatrix.push_back(structMatrix);
				}

				if (isCSGNode)
					return true;

				bool resultNode = parseNodePart(false, flagNew);
				if (!resultNode)
					return false;

				nodeIsAdded = true;
			}
			else if (strncmp(buf + ind, "- csg_union:", CSG_UNION_LENGTH) == 0)
			{
				newObject = new DoubleObject();
				((DoubleObject*)newObject)->setType(CSG_UNION);
				bool resultNode = parseCSGNodePart(newObject, flagNew);
				if (!resultNode)
					return false;
			}
			else if (strncmp(buf + ind, "- csg_difference:", CSG_DIFFERENCE_LENGTH) == 0)
			{
				newObject = new DoubleObject();
				((DoubleObject*)newObject)->setType(CSG_DIFFERENCE);
				bool resultNode = parseCSGNodePart(newObject, flagNew);
				if (!resultNode)
					return false;
			}
			else if (strncmp(buf + ind, "- csg_intersection:", CSG_INTERSECTION_LENGTH) == 0)
			{
				newObject = new DoubleObject();
				((DoubleObject*)newObject)->setType(CSG_INTERSECTION);
				bool resultNode = parseCSGNodePart(newObject, flagNew);
				if (!resultNode)
					return false;
			}
			else if (strncmp(buf + ind, "- plane:", PLANE_LENGTH) == 0)
			{
				newObject = new Plane();
				createNewObject = true;
			}
			else if (strncmp(buf + ind, "- triangle:", TRIANGLE_LENGTH) == 0)
			{
				newObject = new Triangle();
				createNewObject = true;
			}
			else if (strncmp(buf + ind, "- sphere:", SPHERE_LENGTH) == 0)
			{
				newObject = new Sphere();
				createNewObject = true;
			}
			else if (strncmp(buf + ind, "- cylinder:", CYLINDER_LENGTH) == 0)
			{
				newObject = new Cylinder();
				createNewObject = true;
			}
			else if (strncmp(buf + ind, "- cone:", CONE_LENGTH) == 0)
			{
				newObject = new Cone();
				createNewObject = true;
			}
			else if (strncmp(buf + ind, "- obj_model:", OBJ_MODEL_LENGTH) == 0)
			{
				newObject = new ObjModel();
				createNewObject = true;
			}
			else
				return false;

			if (newObject != NULL && createNewObject)
			{
				newObject->translateMatrix = structMatrix.translate;
				newObject->rotateMatrix = structMatrix.rotate;
				newObject->scaleMatrix = structMatrix.scale;
				bool resultNode = parseGeometryNode(newObject);
				if (!resultNode)
					return false;
			}
		}
		else if (buf[ind] == 'r' && isCSGNode)
		{
			if (newObject != NULL)
				scene.activeObjects.push_back(newObject);
			return true;
		}
		else
			return false;
	}

	if (newObject != NULL && !nodeIsAdded)
		scene.activeObjects.push_back(newObject);

	*flagNew = true;
	return true;
}

bool HandlerClass::parseLightPart(bool* flagNew)
{
	int		ind = 0;
	Light	newLight;

	while (fgets(buf, MAX_STRING, f) != NULL)
	{
		ind = 0;
		while (ind < MAX_STRING && (buf[ind] == ' ' || buf[ind] == '\t'))
			ind++;
		if (ind >= MAX_STRING)
			continue;
		if (buf[ind] == '#' || buf[ind] == '\n')
			continue;
		if (ind == 0)
		{
			*flagNew = false;
			return true;
		}
		if (strncmp(buf + ind, "point: ", POINT_LIGHT) == 0)
		{
			float	x = 0.0f, y = 0.f, z = 0.0f;
			float	r = 0, g = 0, b = 0;
			sscanf(buf + ind + POINT_LIGHT, "{x: %f, y: %f, z: %f}, (%f, %f, %f)", &x, &y, &z, &r, &g, &b);
			newLight.color = Color(r, g, b);
			newLight.start = Vector3D(x, y, z);
			newLight.direction = Vector3D(0.0f, 0.0f, 0.0f);
			newLight.typeLight = POINT;
			scene.sceneLight.push_back(newLight);
		}
		else if (strncmp(buf + ind, "directional: ", DIRECTIONAL_LIGHT) == 0)
		{
			float	x = 0.0f, y = 0.f, z = 0.0f;
			float	r = 0, g = 0, b = 0;
			sscanf(buf + ind + DIRECTIONAL_LIGHT, "{x: %f, y: %f, z: %f}, (%f, %f, %f)", &x, &y, &z, &r, &g, &b);
			newLight.color = Color(r, g, b);
			newLight.start = Vector3D(0.0f, 0.0f, 0.0f);
			newLight.direction = Vector3D(x, y, z);
			newLight.typeLight = DIRECTIONAL;
			scene.sceneLight.push_back(newLight);
		}
		else
			return false;
	}

	*flagNew = true;
	return true;
}

void HandlerClass::parseSceneFile()
{
	int		ind = 0;
	bool	resultParse;
	bool	getNew = true;
	char*	res = NULL;

	f = fopen(sceneFileName, "rb");
	if (f == NULL)
	{
		wrongSceneFile = true;
		return;
	}

	res = fgets(buf, MAX_STRING, f);

	while (res != NULL)
	{
		ind = 0;
		while (ind < MAX_STRING && (buf[ind] == ' ' || buf[ind] == '\t'))
			ind++;
		if (ind >= MAX_STRING)
		{
			res = fgets(buf, MAX_STRING, f);
			continue;
		}
		if (buf[ind] == '#' || buf[ind] == '\n')
		{
			res = fgets(buf, MAX_STRING, f);
			continue;
		}
		else if (buf[ind] == '-')
		{
			if (strncmp(buf + ind, "- camera:", CAMERA_LENGTH) == 0)
				resultParse = parseCameraPart(&getNew);
			else if (strncmp(buf + ind, "- node:", NODE_LENGTH) == 0)
				resultParse = parseNodePart(false, &getNew);
			else if (strncmp(buf + ind, "- light:", LIGHT_LENGTH) == 0)
				resultParse = parseLightPart(&getNew);

			if (!resultParse)
			{
				wrongSceneFile = true;
				return;
			}

			if (getNew)
				res = fgets(buf, MAX_STRING, f);
		}
		else
		{
			wrongSceneFile = true;
			return;
		}
	}

	fclose(f);
}

void HandlerClass::working()
{
	scene.countScreenParams();
	image.createImage(scene.screenWidth, scene.screenHeight);
	scene.rayTracing(&image);
	image.saveOutputImage(outputFileName);
}


HandlerClass::~HandlerClass()
{
	listMatrix.clear();
	nodeLevels.clear();
}
