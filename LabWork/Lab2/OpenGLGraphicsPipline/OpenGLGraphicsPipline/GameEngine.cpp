#include "GameEngine.h"
#include <iostream>

namespace GE {
	GameEngine::GameEngine()
	{

	}

	GameEngine::~GameEngine()
	{

	}

	bool GameEngine::Init(bool enbVsync)
	{
		// initialises SDL video
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "unable to initialise SDL. SDL error: " << SDL_GetError() << std::endl;
			return false;
		}

		// set the openGL version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// creates the sdl window
		window = SDL_CreateWindow("SDL OpenGL", 50, 50, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		// if the window does not exists respond by sending an error
		if (window == nullptr) {
			std::cerr << "Unbale to create the SDL window. SDL erroe: " << SDL_GetError() << std::endl;
			return false;
		}

		// creates the gl context and links it to the window
		glContext = SDL_GL_CreateContext(window);

		// checks if glcontext exists
		if (glContext == nullptr) {
			std::cerr << "could not create gl contetxt. SDL error: " << SDL_GetError() << std::endl;
			return false;
		}

		// starts glew
		GLenum status = glewInit();

		//checks if glew exists
		if (status != GLEW_OK) {
			std::cerr << "could not create glew. glew erroe:  " << glewGetErrorString(status) << std::endl;
			return false;
		}

		if (SDL_GL_SetSwapInterval(enbVsync) != 0) {
			std::cerr << "could not set vsysnc. SDL error: " << SDL_GetError() << std::endl;
			return false;
		}
	
		
		// create the triangle renderer object variable
		triangle = new TriangleRenderer();

		// initialise the object
		triangle->Init();

		return true;
	}

	bool GameEngine::KeepRunning()
	{
		SDL_PumpEvents();

		SDL_Event evt;

		// check for quit event
		if (SDL_PeepEvents(&evt, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
			// if the user quits stop the program
			return false;
		}
		return true;
	}

	void GameEngine::Update()
	{

	}

	void GameEngine::Draw()
	{

		
		// clears the background
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		triangle->Draw();

		SDL_GL_SwapWindow(window);
	}

	void GameEngine::Shutdown()
	{
		triangle->Destroy();

		SDL_DestroyWindow(window);
		window = nullptr;
		
		SDL_Quit();
	}

	void GameEngine::SetWindowTitle(const char* newTitle)
	{
		SDL_SetWindowTitle(window, newTitle);
	}


	void display_info_message(const char* msg)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "basic game engine", msg, nullptr);
	}
}