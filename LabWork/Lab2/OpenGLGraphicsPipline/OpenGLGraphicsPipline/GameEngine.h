#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "TriangleRenderer.h"


namespace GE {

	class GameEngine {
	public:
		GameEngine(); // constructor

		virtual ~GameEngine(); //Destuctor

		bool Init(bool);
		bool KeepRunning();
		void Update();
		void Draw(); 
		void Shutdown();

		void SetWindowTitle(const char*);

	private:
		SDL_Window* window;

		SDL_GLContext glContext;

		TriangleRenderer* triangle;
	
	};

	// helper function 

	void display_info_message(const char*);

}