#include "Texture.h"
#include <iostream>

namespace GE {
	void Texture::loadTexture(std::string filename) {
		// load the txture data from a file
		SDL_Surface* surfaceImage = IMG_Load(filename.c_str());

		// check it was loaded
		if (surfaceImage == nullptr) {
			std::cerr << "Error: could not load texture" << std::endl;
			return;
		}

		// get the dimentions
		width = surfaceImage->w;
		height = surfaceImage->h;

		// get format
		format = surfaceImage->format->format;

		// switch statment to choose the OpenGL format based on the SDLformat

		switch (format)
		{
		case SDL_PIXELFORMAT_RGBA32: format = GL_RGBA;
			break;

		case SDL_PIXELFORMAT_RGB24: format = GL_RGB;
			break;

			// should add more casees for diffenrt format types of textures i cmight come accross --------------------------------------------------
		default:
			format = GL_RGB;
			break;
		}

		// now create the OpenGL Texture
		glGenTextures(1, &textureName);

		glBindTexture(GL_TEXTURE_2D, textureName);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surfaceImage->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// release the sdl surface as it is no longer needed
		SDL_FreeSurface(surfaceImage);

		return;
	}
		
}

