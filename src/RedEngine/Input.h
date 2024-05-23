#include <SDL2/SDL.h>
#include <rend/rend.h>
#include <vector>
#include <iostream>


namespace red
{

	struct Core;

	struct Input
	{

	public:
		Input();
		~Input();

		void update(); //Handles all inputs

		bool Quit;

		int mouse_x, mouse_y;

		bool once;

		//Getters and Setters
		rend::vec2 getMouseDelta();
		rend::vec2 getMousePos();
		rend::vec2 getMouseDownPos();
		bool getKey(int keyCode);
		bool getKeyDown(int keyCode); 
		bool getKeyUp(int keyCode);
		bool getKeyM(int keyCode);
		bool getKeyDownM(int keyCode); 
		bool getKeyUpM(int keyCode);
		bool getMouseLock() { return m_mouseLock; }
		void setMouseLock(bool state);

	private:
		friend struct Core;

		SDL_Event eventQueue;
		rend::vec2 mouseDelta;
		rend::vec2 mouseDownPos;

		//Keyboard vectors
		std::vector<int> keys;
		std::vector<int> upKeys;
		std::vector<int> downKeys;

		//Mouse vectors
		std::vector<int> keysM;
		std::vector<int> upKeysM;
		std::vector<int> downKeysM;

		bool m_mouseLock;

		std::weak_ptr<Core> m_core;
	};
}