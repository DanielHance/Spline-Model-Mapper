#include <iostream>
#include <vector>
#include <rend/rend.h>

namespace red
{
	struct Entity;
	struct Component;
	struct Window;
	struct Input;
	struct Resources;
	struct GUI;
	struct Collider;
	struct Camera;

	struct Core
	{
	public:
		Core();
		~Core();

		static std::shared_ptr<Core> initialize();

		void start();
		void stop();
		std::shared_ptr<Entity> addEntity();

		//Getters and Setters
		void setCamera(std::shared_ptr<Entity> _cam);
		std::shared_ptr<Camera> getCamera();
		std::shared_ptr<Window> getWindow() { return m_window; }
		std::shared_ptr<Input> getInput() { return m_input; }
		std::shared_ptr<GUI> getGUI() { return m_gui; }
		std::shared_ptr<Resources> getResources() { return m_resources; }
		std::vector<std::weak_ptr<Collider>> getColliders() { return m_colliders; }
		float getDelta() { return m_dT; }

		float m_elapsedTime;
		float m_lastElapsedTime;
		float m_dT;


	private:
		friend struct Input;
		friend struct Collider;
		friend struct GUI;

		//These need to be in this order so the destructors call in the rigth order!!!
		std::shared_ptr<Window> m_window; 
		std::shared_ptr<Input> m_input;
		std::shared_ptr<GUI> m_gui;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<Resources> m_resources;
		std::vector<std::shared_ptr<Entity>> m_entities;
		std::vector<std::weak_ptr<Collider>> m_colliders;
		bool m_running;
		std::weak_ptr<Core> m_self;

		static void loop(void* _userData); //Main Game Loop

	};
}
