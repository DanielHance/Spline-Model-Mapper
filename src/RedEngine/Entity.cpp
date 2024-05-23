#include "Entity.h"

#include "Component.h"

namespace red
{

Entity::Entity()
{
	m_alive = true;
}


Entity::~Entity()
{
}


void Entity::tick()
{
	for (size_t ci = 0; ci < m_components.size(); ++ci)
	{
		m_components.at(ci)->tick();
	}
}


void Entity::display()
{
	for (size_t ci = 0; ci < m_components.size(); ++ci)
	{
		m_components.at(ci)->display();
	}
}

void Entity::gui()
{
	for (size_t ci = 0; ci < m_components.size(); ++ci)
	{
		m_components.at(ci)->gui();
	}
}

void Entity::kill()
{
	if (!m_alive) return;

	m_alive = false;

	//Remove dead components
	for (size_t ci = 0; ci < m_components.size(); ++ci)
	{
		if (!m_components.at(ci)->getAlive())
		{
			m_components.erase(m_components.begin() + ci);
		}
	}	
}



}


