#include "RigidBody.h"
#include "Entity.h"
#include "Transform.h"
#include "Collider.h"
#include "Core.h"

#include <iostream>

namespace red
{
	RigidBody::RigidBody()
	{
	}

	RigidBody::~RigidBody()
	{
	}

	void RigidBody::onTick()
	{
		std::vector<std::weak_ptr<Collider>> colliders = getCore().lock()->getColliders();

		std::vector<std::weak_ptr<Collider>> m_colliders;		
		getEntity()->getComponents(m_colliders);
		
		for(int i = 0; i < colliders.size(); i++)
		{
			std::shared_ptr<Collider> other = colliders[i].lock();
			if (getEntity() != other->getEntity())
			{
				
				for (int j = 0; j < m_colliders.size(); j++)
				{
					if (m_colliders[j].lock()->isColliding(*other))
					{
						//Collison Responce 
						std::shared_ptr<Collider> _self = m_colliders[j].lock();
						float amount = 0.05f;
						float step = 0.1f;
						
						while (true)
						{
							//x
							if (!_self->isColliding(*other)) break;
							getEntity()->getTransform()->Move(rend::vec3(amount, 0, 0));
							if (!_self->isColliding(*other)) break;
							getEntity()->getTransform()->Move(rend::vec3(-amount * 2, 0, 0));
							//z
							if (!_self->isColliding(*other)) break;
							getEntity()->getTransform()->Move(rend::vec3(0, 0, amount));
							if (!_self->isColliding(*other)) break;
							getEntity()->getTransform()->Move(rend::vec3(0, 0, -amount * 2));
							//y
							if (!_self->isColliding(*other)) break;
							getEntity()->getTransform()->Move(rend::vec3(0, amount, 0));
							if (!_self->isColliding(*other)) break;
							getEntity()->getTransform()->Move(rend::vec3(0, -amount * 2, 0));
							amount += step;
						}
					}
				}
			}
		}
	}
}