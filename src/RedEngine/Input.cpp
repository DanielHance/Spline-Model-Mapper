#include "Input.h"
#include "Core.h"
#include "Window.h"

namespace red
{

	Input::Input()
	{
		Quit = false;
		m_mouseLock = true;
		mouseDownPos = rend::vec2(0);
		SDL_ShowCursor(0);
	}

	Input::~Input()
	{
	}

	rend::vec2 Input::getMouseDelta()
	{
		return mouseDelta;
	}

	rend::vec2 Input::getMousePos()
	{
		return rend::vec2(mouse_x, mouse_y);
	}

	rend::vec2 Input::getMouseDownPos()
	{
		return mouseDownPos;
	}

	void Input::update()
	{
		mouseDelta.x = 0;
		mouseDelta.y = 0;

		rend::ivec2 winSize(m_core.lock()->m_window->getSize());

		//Clears arrays
		downKeys.clear();
		downKeysM.clear();
		upKeys.clear();
		upKeysM.clear();


		while (SDL_PollEvent(&eventQueue) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&eventQueue);  //    <-----------------------------------------ImGui

			if (eventQueue.type == SDL_QUIT)
			{
				Quit = 1;
			}

			//Mosue Movement
			else if (eventQueue.type == SDL_MOUSEMOTION)
			{

				mouse_x = eventQueue.motion.x;
				mouse_y = eventQueue.motion.y;

				mouseDelta.x += mouse_x - winSize.x / 2;
				mouseDelta.y += mouse_y - winSize.y / 2;

			}

			//Mouse Keys
			else if (eventQueue.type == SDL_MOUSEBUTTONDOWN)
			{
				downKeysM.push_back(eventQueue.button.button);
				keysM.push_back(eventQueue.button.button);
				mouseDownPos = rend::vec2(mouse_x, mouse_y);
			}
			else if (eventQueue.type == SDL_MOUSEBUTTONUP)
			{
				upKeysM.push_back(eventQueue.button.button);
				for (int i = 0; i < keysM.size(); i++)
				{
					if (keysM[i] == eventQueue.button.button)
					{
						keysM.erase(keysM.begin() + i);
						--i;
					}
				}
			}

			//Keybaord Keys
			else if (eventQueue.type == SDL_KEYDOWN)
			{
				downKeys.push_back(eventQueue.button.button);
				keys.push_back(eventQueue.button.button); //   <-------------Break point this line to find key codes

				if (eventQueue.key.keysym.sym == SDLK_ESCAPE)
				{
					Quit = true;
				}
			}
			else if (eventQueue.type == SDL_KEYUP)
			{
				upKeys.push_back(eventQueue.button.button);
				for (int i = 0; i < keys.size(); i++)
				{
					if (keys[i] == eventQueue.button.button)
					{
						keys.erase(keys.begin() + i);
						--i;
					}
				}
			}
		}
		
		//Moves mouse to middel of the screen
		if (m_mouseLock)
		{
			SDL_WarpMouseInWindow(NULL, winSize.x / 2, winSize.y / 2);
		}

	}

	//Keyboard keys
	bool Input::getKey(int keyCode)
	{

		for (int i = 0; i < keys.size(); i++)
		{
			if (keys.at(i) == keyCode)
			{
				return true;
			}
		}

		return false;
	}

	//Keyboard keys down
	bool Input::getKeyDown(int keyCode)
	{

		for (int i = 0; i < downKeys.size(); i++)
		{
			if (downKeys.at(i) == keyCode)
			{
				return true;
			}
		}

		return false;

	}

	//Keyboard keys up
	bool Input::getKeyUp(int keyCode)
	{

		for (int i = 0; i < upKeys.size(); i++)
		{
			if (upKeys.at(i) == keyCode)
			{
				return true;
			}
		}

		return false;
	}

	//Mouse keys
	bool Input::getKeyM(int keyCode)
	{

		for (int i = 0; i < keysM.size(); i++)
		{
			if (keysM.at(i) == keyCode)
			{
				return true;
			}
		}

		return false;
	}

	//Mouse keys down
	bool Input::getKeyDownM(int keyCode)
	{

		for (int i = 0; i < downKeysM.size(); i++)
		{
			if (downKeysM.at(i) == keyCode)
			{
				return true;
			}
		}

		return false;

	}

	//Mouse keys up
	bool Input::getKeyUpM(int keyCode)
	{

		for (int i = 0; i < upKeysM.size(); i++)
		{
			if (upKeysM.at(i) == keyCode)
			{
				return true;
			}
		}

		return false;
	}

	void Input::setMouseLock(bool state)
	{
		if (state)
		{
			SDL_ShowCursor(0);
		}
		else
		{
			SDL_ShowCursor(1);
		}
		m_mouseLock = state;
	}
}