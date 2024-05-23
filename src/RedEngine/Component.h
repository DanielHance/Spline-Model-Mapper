#pragma once
#include <iostream>

namespace red
{
	struct Core;
	struct Entity;

	struct Component
	{
	public:
		Component();
		~Component();

		bool getAlive() { return m_alive; }

		//Getters and Setters
		std::weak_ptr<Core> getCore() { return m_core; }
		std::shared_ptr<Entity> getEntity() { return m_entity.lock(); }

	private:
		friend struct Entity;

		virtual void onInit();
		virtual void onBegin();
		void tick();
		virtual void onTick();
		void display();
		virtual void onDisplay();
		void gui();
		virtual void onGUI();


		std::weak_ptr<Component> m_self;
		std::weak_ptr<Entity> m_entity;
		std::weak_ptr<Core> m_core;
		bool m_alive;
	};
}