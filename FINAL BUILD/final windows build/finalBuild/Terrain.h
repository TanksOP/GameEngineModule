#pragma once
#include "Model.h"
#include <SDL_image.h>
#include <vector>

namespace GE {

	class Terrain : public Model // this means that Terrain will inherit from model
	{
	public:
		Terrain();
		GLuint getVbo() { return vbo; }
		GLuint getIbo() { return ibo; }
		GLuint getIndexCount() { return indexCount; }

	private:
		GLuint vbo, ibo, indexCount;
	};
}


