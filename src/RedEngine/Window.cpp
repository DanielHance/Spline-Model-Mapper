#include "Window.h"

namespace red
{
	Window::Window()
	{
		//Set Size
		m_size = rend::ivec2(640, 480);

		//Graphics Set Up
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			throw std::runtime_error("Failed to initialize SDL!");
		}

		m_window = SDL_CreateWindow("RedEngine Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_size.x, m_size.y, SDL_WINDOW_RESIZABLE | SDL_WINDOW_REND);
		if (!m_window)
		{
			SDL_Quit();
			throw std::runtime_error("FAiled to create window!");
		}

		m_context = SDL_Rend_CreateContext(m_window);
		if (!m_context)
		{
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			throw std::runtime_error("Failed to create OpenGL context");
		}

		//Sound Set Up
		m_device = alcOpenDevice(NULL);
		if (!m_device)
		{
			SDL_GL_DeleteContext(m_context);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			throw std::runtime_error("Failed to create OpenAL device");
		}

		m_soundContext = alcCreateContext(m_device, NULL);
		if (!m_soundContext)
		{
			alcCloseDevice(m_device);
			SDL_GL_DeleteContext(m_context);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			throw std::runtime_error("Failed to create OpenAL context");
		}

		if (!alcMakeContextCurrent(m_soundContext))
		{
			alcDestroyContext(m_soundContext);
			alcCloseDevice(m_device);
			SDL_GL_DeleteContext(m_context);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			throw std::runtime_error("Failed to make context current");
		}
		//alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		const char* glslVersion = "#version 130";
		ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
		ImGui_ImplOpenGL3_Init(glslVersion);

	}

	Window::~Window()
	{
		alcDestroyContext(m_soundContext);
		alcCloseDevice(m_device);
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}
}