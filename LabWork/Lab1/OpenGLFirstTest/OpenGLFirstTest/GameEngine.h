#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>


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

		float col1 = 1, col2 = 0, col3 = 0;
		float speeeeed = 0.0003;
		float col1Inc = speeeeed, col2Inc = 0.0000, col3Inc = 0.0000;
		
	};

	// helper function 

	void display_info_message(const char*);

}