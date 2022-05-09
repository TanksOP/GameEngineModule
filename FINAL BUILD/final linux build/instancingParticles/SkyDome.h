#pragma once
#include "Model.h"
#include <SDL2/SDL_image.h>
#include <vector>
#include <math.h>


namespace GE {
	class SkyDome : public Model
	{
	public:
		SkyDome();
		GLuint getVbo() { return vbo; }
		GLuint getIbo() { return ibo; }
		GLuint getIndexCount() { return indexCount; }

	private:
		GLuint vbo, ibo, indexCount;
	};
}


