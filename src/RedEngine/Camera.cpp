#include "Camera.h"
#include "Entity.h"
#include "Transform.h"


namespace red
{
	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	rend::mat4 Camera::getView()
	{
		return rend::inverse(getEntity()->getTransform()->getModelMat());
	}
}