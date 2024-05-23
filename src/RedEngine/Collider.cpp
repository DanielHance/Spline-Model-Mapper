#include "Collider.h"
#include "Entity.h"
#include "Core.h"
#include "Transform.h"
#include <iostream>


namespace red
{
	Collider::Collider()
	{
	}

	Collider::~Collider()
	{
	}


	void Collider::onInit()
	{
		getCore().lock()->m_colliders.push_back(getEntity()->getComponent<Collider>());
	}

	bool Collider::aabbTest(rend::vec3 aMax, rend:: vec3 aMin, rend::vec3 bMax, rend::vec3 bMin)
	{
		
		return (aMin.x <= bMax.x && aMax.x >= bMin.x) && (aMin.y <= bMax.y && aMax.y >= bMin.y) && (aMin.z <= bMax.z && aMax.z >= bMin.z);
	}
}