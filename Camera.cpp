#include "Camera.h"


Camera::Camera()
{
	fovX = 110;
	fovY = 80;
}

void Camera::setPosition(float _x, float _y, float _z)
{ 
	position = Vector3D(_x, _y, _z); 
	translateMatrix.setUnitMatrix();
	translateMatrix.translate(position.x, position.y, position.z);
};
void Camera::setDirection(float _x, float _y, float _z)
{ 
	direction = Vector3D(_x, _y, _z); 
	rotateMatrix.setUnitMatrix();
	rotateMatrix.rotateX(direction.x).rotateY(direction.y).rotateZ(direction.z);
};

Camera::~Camera()
{
}
