#include "FollowCam.h"

FollowCam::FollowCam()
{
}

FollowCam::~FollowCam()
{
}

void FollowCam::setTarget(std::shared_ptr<Entity> _p)
{
	Player = _p;
}

void FollowCam::onTick()
{
	if (getCore().lock()->getInput()->getKey(4)) //a
	{
		getEntity()->getTransform()->Move(rend::vec3(-0.5, 0, 0) * getCore().lock()->getDelta()); 
		//std::cout << "a" << std::endl;
	}
	if (getCore().lock()->getInput()->getKey(7)) //d
	{
		getEntity()->getTransform()->Move(rend::vec3(0.5, 0, 0) * getCore().lock()->getDelta()); 
		//std::cout << "d" << std::endl;
	}
	if (getCore().lock()->getInput()->getKey(26)) //w
	{
		getEntity()->getTransform()->Move(rend::vec3(0, 0, -0.5) * getCore().lock()->getDelta()); 
		//std::cout << "w" << std::endl;
	}
	if (getCore().lock()->getInput()->getKey(22)) //s
	{
		getEntity()->getTransform()->Move(rend::vec3(0, 0, 0.5) * getCore().lock()->getDelta()); 
		//std::cout << "s" << std::endl;
	}
	if (getCore().lock()->getInput()->getKey(20)) //q
	{
		getEntity()->getTransform()->Move(rend::vec3(0, -0.5, 0) * getCore().lock()->getDelta()); 
		//std::cout << "q" << std::endl;
	}
	if (getCore().lock()->getInput()->getKey(8)) //e
	{
		getEntity()->getTransform()->Move(rend::vec3(0, 0.3, 0) * getCore().lock()->getDelta()); 
		//std::cout << "e" << std::endl;
	}
	if (getCore().lock()->getInput()->getKey(80)) //left arrow
	{
		getEntity()->getTransform()->Rotate(rend::vec3(0, 0.5, 0));
		//std::cout << "left arrow" << std::endl;
	}
	if (getCore().lock()->getInput()->getKey(79)) //right arrow
	{
		getEntity()->getTransform()->Rotate(rend::vec3(0, -0.5, 0));
		//std::cout << "right arrow" << std::endl;
	}
	if (getCore().lock()->getInput()->getMouseLock()) //mouse movement
	{
		getEntity()->getTransform()->Rotate(rend::vec3(0, (-getCore().lock()->getInput()->getMouseDelta().x / 8) * getCore().lock()->getDelta(), 0));
	}

	//std::cout << "x = " << getEntity()->getTransform()->getPos().x << " ,y = " << getEntity()->getTransform()->getPos().y << " ,z = " << getEntity()->getTransform()->getPos().z << std::endl;
}
