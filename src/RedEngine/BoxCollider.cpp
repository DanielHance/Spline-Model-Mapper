#include "BoxCollider.h"
#include "Entity.h"
#include "Transform.h"
#include "Core.h"
#include <iostream>


namespace red
{

	BoxCollider::BoxCollider()
	{
	}


	BoxCollider::~BoxCollider()
	{
	}
	
	bool BoxCollider::isColliding(Collider& _other)
	{
		bool rtn = false;

		if (dynamic_cast<BoxCollider*>(&_other) != nullptr)
		{
			rtn = boxCollide(*_other.getEntity()->getComponent<BoxCollider>());
		}
		/*else
		{
			rtn = meshCollide(*_other.getEntity()->getComponent<MeshCollider>());
		}
		*/
		return rtn;
	}

	bool BoxCollider::boxCollide(BoxCollider& _other)
	{
		rend::vec3 a = getEntity()->getTransform()->getPos();
		rend::vec3 b = _other.getEntity()->getTransform()->getPos();
		rend::vec3 ahs = getSize() / 2.0f;
		rend::vec3 bhs = _other.getSize() / 2.0f;

		if (a.x > b.x)
		{
			if (b.x + bhs.x < a.x - ahs.x) return false;
		}
		else
		{
			if (a.x + ahs.x < b.x - bhs.x) return false;
		}
		if (a.y > b.y)
		{
			if (b.y + bhs.y < a.y - ahs.y) return false;
		}
		else
		{
			if (a.y + ahs.y < b.y - bhs.y) return false;
		}
		if (a.z > b.z)
		{
			if (b.z + bhs.z < a.z - ahs.z) return false;
		}
		else
		{
			if (a.z + ahs.z < b.z - bhs.z) return false;
		}

		return true;
	}

	bool BoxCollider::meshCollide(MeshCollider& _other)
	{
		return false;
	}
}