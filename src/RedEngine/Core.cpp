#include "Core.h"
#include "Window.h"
#include "Entity.h"
#include "Input.h"
#include "Transform.h"
#include "Resources.h"
#include "GUI.h"
#include "Camera.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace red
{

	Core::Core()
	{
		m_running = false;
		m_elapsedTime = 0;
		m_lastElapsedTime = 0;
		m_dT = 0;
	}


	Core::~Core()
	{
	}
	

	std::shared_ptr<Core> Core::initialize()
	{
		std::shared_ptr<Core> rtn = std::make_shared<Core>();

		rtn->m_self = rtn;
		rtn->m_running = false;

		//Add all architecture objects to core
		rtn->m_window = std::make_shared<Window>();
		rtn->m_input = std::make_shared<Input>();
		rtn->m_gui = std::make_shared<GUI>(rtn);
		rtn->m_resources = std::make_shared<Resources>();
		rtn->m_input->m_core = rtn;

		return rtn;
	}


	std::shared_ptr<Entity> Core::addEntity()
	{
		std::shared_ptr<Entity> rtn = std::make_shared<Entity>();

		rtn->m_core = m_self;
		rtn->m_self = rtn;

		rtn->m_transform = rtn->addComponent<Transform>(); //Adds a transform component to every entitys
		m_entities.push_back(rtn);

		return rtn;
	}


	void Core::start()
	{
		m_running = true;

		//Calls loop differently depending on build platform
		#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(loop, (void*)this, 0, 1);
		#else		
		while (m_running)
		{
			loop((void*)this);
		}
		#endif
	}


	void Core::stop()
	{
		m_running = false;

		//Stops loop on web build
		#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop;
		#endif
	}

	void Core::loop(void* _userData)
	{
		
		Core* self = (Core*)_userData;

		self->m_elapsedTime = SDL_GetTicks();

		rend::mat4 camMat(self->m_camera->getEntity()->getTransform()->getModelMat());
		alListener3f(AL_POSITION, camMat[0][3], camMat[1][3], camMat[2][3]);

		//Updates input 
		self->m_input->update();

		//Check if quit
		if (self->m_input->Quit) self->stop();

		//Calls tick on all entities
		for (size_t ei = 0; ei < self->m_entities.size(); ++ei)
		{
			self->m_entities.at(ei)->tick();
		}

		//Clear window
		SDL_Rend_ClearWindow(self->m_window->getWindow());

		//Calls display on all entities
		for (size_t ei = 0; ei < self->m_entities.size(); ++ei)
		{
			self->m_entities.at(ei)->display();
		}

		//Calls gui on all entities
		for (size_t ei = 0; ei < self->m_entities.size(); ++ei)
		{
			self->m_entities.at(ei)->gui();
		}

		//Updates window
		SDL_Rend_SwapWindow(self->m_window->getWindow());

		//Remove dead entities
		for (size_t ei = 0; ei < self->m_entities.size(); ++ei)
		{
			if (!self->m_entities.at(ei)->getAlive())
			{
				self->m_entities.erase(self->m_entities.begin() + ei);
			}
		}

		self->m_dT = (self->m_elapsedTime - self->m_lastElapsedTime) / 100;
		self->m_lastElapsedTime = self->m_elapsedTime;
	}
	
	void Core::setCamera(std::shared_ptr<Entity> _ent)
	{
		if (_ent->getComponent<Camera>())
		{
			m_camera = _ent->getComponent<Camera>();
		}
		else
		{
			std::cout << "WARNING --- Entity set to camera does not contain a camera component!" << std::endl;
		}
	}


	std::shared_ptr<Camera> Core::getCamera()
	{
		if (!m_camera)
		{
			throw std::runtime_error("Invalid Camera set (use setCamera() to link a camera with core)");
		}

		return m_camera;
	}
}