#include "SkyboxRenderer.h"
#include "ShaderUtils.h"
#include "Utils.h"
#include <SDL_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


namespace GE {

	struct CubeVertex{
		float x, y, z;

		CubeVertex() {
			x = 0.f;
			y = 0.f;
			z = 0.f;
		}

		CubeVertex(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}

	};

	const float SIDE = 1.0f;

	CubeVertex cube[] = {
		// Front face
		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),

		// Back face
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, SIDE),

		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, SIDE, SIDE),

		// Left face
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, -SIDE),

		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),

		// Right face
		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),

		// Top face
		CubeVertex(-SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE,  SIDE),

		// Bottom face
		CubeVertex(-SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),
	};

	void SkyboxRenderer::createCubemap(std::vector<std::string> filenames)
	{
		glGenTextures(1, &skyboxCubeMapName);

		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMapName);

		for (int facenum = 0; facenum < 6; facenum++) {
			SDL_Surface* surfaceImage = IMG_Load(filenames[facenum].c_str());

			if (surfaceImage == NULL) {
				std::cout << "cubemap image did not load" << std::endl;
				return;
			}

			GLenum format = surfaceImage->format->format;

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

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + facenum, 0, format, surfaceImage->w, surfaceImage->h,
				0, format, GL_UNSIGNED_BYTE, surfaceImage->pixels);

			SDL_FreeSurface(surfaceImage);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// adds clamping to the cube map to help with inaccuacies 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}

	void SkyboxRenderer::createCubeVBO()
	{
		glGenBuffers(1, &vboSkybox);
		glBindBuffer(GL_ARRAY_BUFFER, vboSkybox);

		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SkyboxRenderer::createSkyboxProgram()
	{
		//shader code
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"out vec3 texCoord;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main(){\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"v = projection * view * v;\n"
			"gl_Position = v;\n"
			"texCoord = vertexPos3D;\n"
			"}\n" };

		// Fragment shader source code
		const GLchar* F_ShaderCode[] = {
		  "#version 140\n"
		  "in vec3 texCoord;\n"
		  "uniform samplerCube sampler;\n"
		  "out vec4 fragmentColour;\n"
		  "void main()\n"
		  "{\n"
		  "fragmentColour = vec4(texture(sampler, texCoord).rgb, 1.0f);\n"
		  "}\n"
		};

		bool result = compileProgram(V_ShaderCode, F_ShaderCode, &skyboxProgram);
		if (!result) {
			std::cerr << "faild to create skyBox program" << std::endl;
			return;
		}
		vertexLocation = glGetAttribLocation(skyboxProgram, "vertexPos3D");
		if (vertexLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		viewUniformID = glGetUniformLocation(skyboxProgram, "view");
		projectionUniformID = glGetUniformLocation(skyboxProgram, "projection");
		samplerID = glGetUniformLocation(skyboxProgram, "sampler");

	}

	void GE::SkyboxRenderer::draw(Camera* cam)
	{
		bool isDepthTestEnable = glIsEnabled(GL_DEPTH_TEST);

		glDisable(GL_DEPTH_TEST);
		glm::mat4 camView = cam->getViewMatrix();
		glm::mat4 projection = cam->getProjectionMatrix();

		camView[3][0] = 0.0f;
		camView[3][1] = 0.0f;
		camView[3][2] = 0.0f;

		glUseProgram(skyboxProgram);
		glBindBuffer(GL_ARRAY_BUFFER, vboSkybox);

		glUniformMatrix4fv(viewUniformID, 1, GL_FALSE, glm::value_ptr(camView));
		glUniformMatrix4fv(projectionUniformID, 1, GL_FALSE, glm::value_ptr(projection));

		GLCALL(glEnableVertexAttribArray(vertexLocation));

		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)offsetof(CubeVertex, x));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerID, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMapName);

		glDrawArrays(GL_TRIANGLES, 0, sizeof(cube) / sizeof(CubeVertex));

		glDisableVertexAttribArray(vertexLocation);

		glUseProgram(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (isDepthTestEnable) {
			glEnable(GL_DEPTH_TEST);
		}
	}

	void GE::SkyboxRenderer::destroy()
	{
		glDeleteProgram(skyboxProgram);
		glDeleteBuffers(1, &vboSkybox);
		glDeleteTextures(1, &skyboxCubeMapName);
	}
}
