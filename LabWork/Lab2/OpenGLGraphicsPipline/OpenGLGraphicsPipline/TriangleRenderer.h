#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

namespace GE {


	class TriangleRenderer
	{
	public: 
		TriangleRenderer();
		virtual ~TriangleRenderer();

		void Init();

		void Update();

		void Draw();

		void Destroy();

	private:
		// stores the program object that contains the shaders
		GLuint programId;

		// store the attribute to select into the pipline
		// to link the triangle verticies
		GLint vertexPos2DLocation;

		//member stores triangle vertix buffer containing the verticies
		// transfered from this code to the graphics memory
		GLuint vboTriangle;
	};

}

