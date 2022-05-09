#pragma once
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <string>
#include "Camera.h"

namespace GE {
	class SkyboxRenderer
	{
	public:
		SkyboxRenderer(std::string front, std::string back, std::string right, std::string left, std::string top, std::string bottom) {
			std::vector<std::string> filenames;

			filenames.emplace_back(right);
			filenames.emplace_back(left);
			filenames.emplace_back(top);
			filenames.emplace_back(bottom);
			filenames.emplace_back(front);
			filenames.emplace_back(back);

			createCubemap(filenames);
			createCubeVBO();
			createSkyboxProgram();
		}

		~SkyboxRenderer(){}

		void draw(Camera* cam);

		void destroy();

	private:
		void createCubemap(std::vector<std::string> filenames);
		void createCubeVBO();
		void createSkyboxProgram();

		GLuint skyboxCubeMapName;
		GLuint skyboxProgram;
		GLuint vertexLocation;
		GLuint vboSkybox;
		GLuint viewUniformID;
		GLuint projectionUniformID;
		GLuint samplerID;

	};

}


