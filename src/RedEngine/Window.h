#include <iostream>
#include <vector>
#include <rend/rend.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl3.h>



namespace red
{
	struct Input;

	struct Window
	{
	public:
		Window();
		~Window();

		//Getters and Setters
		SDL_Window* getWindow() { return m_window; }
		rend::ivec2 getSize() { return m_size; }
		bool audio() { return m_audio; }

	private:
		//Need to be in this order so the destructors call in correct order
		friend struct Input;
		SDL_Window* m_window = NULL;
		SDL_GLContext m_context = NULL;
		ALCdevice* m_device = NULL;
		ALCcontext* m_soundContext = NULL;
		rend::ivec2 m_size;
		bool m_audio;
	};
}