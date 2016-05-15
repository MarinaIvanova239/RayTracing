#include "Scene.h"
#include "Camera.h"
#include "AdditionalItems.h"
#include "Image.h"
#include <math.h>

SceneClass::SceneClass()
{
	resX = 1024;
	resY = 1024;
	traceDepth = 2;
}

void SceneClass::countScreenParams()
{
	int		fovX = 0, fovY = 0;
	camera.getFOV(&fovX, &fovY);

	float angleX = convertDegtoRad((float)fovX);
	float angleY = convertDegtoRad((float)fovY);

	float tanX = tan(angleX / 2), tanY = tan(angleY / 2);
	float depthX = (float)resX / tanX, depthY = (float)resY / tanY;

	depth = (depthX < depthY) ? depthX : depthY;
	screenWidth = (int)(depth * tanX);
	screenHeight = (int)(depth * tanY);
}

Color SceneClass::tracePath(Ray ray, int depth, int indObject)
{
	Vector3D	interPoint, resultPoint;
	Vector3D	interNorm, resultNorm;
	float		minDist = -10.f;
	Color		resultColor;
	int			objInd = 0;

	for (unsigned i = 0; i < activeObjects.size(); i++)
	{
		if ((i != indObject) && activeObjects[i]->findIntersection(ray, &interPoint, &interNorm, true))
		{
			float distance = dist(ray.start, interPoint);
			if (minDist == -10.f)
				minDist = distance;
			if (distance <= minDist)
			{
				resultPoint = interPoint;
				resultNorm = interNorm;
				minDist = distance;
				objInd = i;
			}
		}
	}

	if (minDist == -10.f)
		return resultColor;

	bool inside = false;
	if (dot(ray.direction, resultNorm) > 0)
		resultNorm = resultNorm.negate();

	if ((activeObjects[objInd]->isReflective || activeObjects[objInd]->isRefractive) && depth < traceDepth)
	{
		Ray		reflRay, refrRay;
		Color	reflColor, refrColor;
		float	temp = dot(ray.direction, resultNorm);

		float	tempFresn = powf(1 + temp, 3.f);
		float	fresnel = tempFresn * 0.9f + 0.1f;

		if (activeObjects[objInd]->isReflective)
		{
			reflRay.start = resultPoint;
			reflRay.direction = substract(ray.direction, resultNorm.multiplyNumber(2.f * temp));
			reflRay.direction.normalize();

			reflColor = tracePath(reflRay, depth + 1, objInd);
		}
		if (activeObjects[objInd]->isRefractive)
		{
			refrRay.start = resultPoint;

			float nu = (temp < 0) ? activeObjects[objInd]->refractCoeff : 1.f / activeObjects[objInd]->refractCoeff;
			float nl = -dot(resultNorm, ray.direction);
			float underSqrt = 1 - nu * nu * (1 - nl * nl);

			refrRay.direction = sum(ray.direction.multiplyNumber(nu), resultNorm.multiplyNumber(nu * nl - sqrt(underSqrt)));
			refrRay.direction.normalize();

			refrColor = tracePath(refrRay, depth + 1, objInd);
		}

		Color tempColor = reflColor.multNumber(fresnel);
		tempColor.plusColor(refrColor.multNumber((1 - fresnel) * activeObjects[objInd]->refractCoeff));
		tempColor = tempColor.multSecondColor(activeObjects[objInd]->color);
		resultColor.plusColor(tempColor);
	}
	
	Ray		shadowRay;
	shadowRay.start = resultPoint;
	for (unsigned i = 0; i < sceneLight.size(); i++)
	{
		if (sceneLight[i].typeLight == POINT)
			shadowRay.direction = substract(sceneLight[i].start, resultPoint);
		else
			shadowRay.direction = sceneLight[i].direction.negate();
		shadowRay.direction.normalize();
		float scalar = dot(shadowRay.direction, resultNorm);
		if (scalar < 0)
			continue;

		int numInter = 0;
		for (unsigned j = 0; j < activeObjects.size(); j++)
		{
			if (j != objInd && activeObjects[j]->findIntersection(shadowRay, &interPoint, &interNorm, true))
				numInter++;
		}
		
		sceneLight[i].color.normalize();
		activeObjects[objInd]->color.normalize();
		if (numInter == 0)
		{
			Color tmpColor = sceneLight[i].color.multNumber(scalar);
			tmpColor = tmpColor.multSecondColor(activeObjects[objInd]->color);
			resultColor.plusColor(tmpColor);
		}
		sceneLight[i].color.returnInterval();
		activeObjects[objInd]->color.returnInterval();
	}

	return resultColor;
}

void SceneClass::rayTracing(ImageClass* img)
{
	Ray		ray;
	Color	pixelsColor;

	int		fovX, fovY;
	camera.getFOV(&fovX, &fovY);

	for (unsigned i = 0; i < activeObjects.size(); i++)
		activeObjects[i]->setParams();

	for (int i = 0; i < screenWidth; i++)
	{
		for (int j = 0; j < screenHeight; j++)
		{
			ray.direction = Vector3D(i - screenWidth / 2.0f, j - screenHeight / 2.0f, -(float)depth);
			ray.direction = camera.getRotateMatrix().multiplyVectorFromRight(ray.direction);
			ray.direction.normalize();
			ray.start = Vector3D(0.0f, 0.0f, 0.0f);
			ray.start = camera.getTranslateMatrix().multiplyVectorFromRight(ray.start);

			pixelsColor = tracePath(ray, 0, -1);

			pixelsColor.returnInterval();

			pixelsColor.r = (pixelsColor.r > 255) ? 255 : pixelsColor.r;
			pixelsColor.g = (pixelsColor.g > 255) ? 255 : pixelsColor.g;
			pixelsColor.b = (pixelsColor.b > 255) ? 255 : pixelsColor.b;

			img->setPixelValue(i, j, pixelsColor);
		}
	}
}

SceneClass::~SceneClass()
{
}
