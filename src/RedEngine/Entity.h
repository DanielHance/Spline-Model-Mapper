#include <iostream>
#include <vector>

namespace red
{

	struct Core;
	struct Component;
	struct Transform;

	struct Entity
	{
		Entity();
		~Entity();

		template <typename T>
		std::shared_ptr<T> addComponent();
		template <typename T>
		std::shared_ptr<T> getComponent();
		template <typename T>
		void getComponents(std::vector<std::weak_ptr<T>>& out);
		void kill();

		//Getters and Setters
		std::weak_ptr<Core> getCore() { return m_core; }
		bool getAlive() { return m_alive; }

		//ShortCuts
		std::shared_ptr<Transform> getTransform() { return m_transform.lock() ;}
	private:
		friend struct Core;

		void tick();
		void display();
		void gui();

		std::vector<std::shared_ptr<Component>> m_components;
		bool m_alive;
		std::weak_ptr<Core> m_core;
		std::weak_ptr<Entity> m_self;
		std::weak_ptr<Transform> m_transform;
	};
}


//Needs to be here as we want the code to be cut into the file that is calling it rather than ran within Entity cpp as Component.h can't be include as it would create multiple includes (THIS IS CURSED BUT IT WORKS!)
template <typename T>
std::shared_ptr<T> red::Entity::addComponent()
{
	std::shared_ptr<T> rtn = std::make_shared<T>();

	rtn->m_core = m_core;
	rtn->m_entity = m_self;
	rtn->m_self = rtn;

	m_components.push_back(rtn);
	rtn->onInit();	

	return rtn;
}

template <typename T>
std::shared_ptr<T> red::Entity::getComponent()
{
	std::shared_ptr<T> rtn;

	for (int i = 0; i < m_components.size(); i++)
	{
		std::shared_ptr<T> rtn = std::dynamic_pointer_cast<T>(m_components.at(i));

		if (rtn) return rtn;
	}

	return rtn;
}

template <typename T>
void red::Entity::getComponents(std::vector<std::weak_ptr<T>>& out)
{
	for (int i = 0; i < m_components.size(); i++)
	{
		std::shared_ptr<T> c = std::dynamic_pointer_cast<T>(m_components.at(i));

		if (c)
		{
			out.push_back(c);
		}
	}
}
