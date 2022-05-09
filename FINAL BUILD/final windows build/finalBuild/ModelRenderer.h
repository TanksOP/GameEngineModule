#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Terrain.h"
#include "SkyDome.h"

namespace GE {


	class ModelRenderer
	{
	public: 
		ModelRenderer();
		virtual ~ModelRenderer();

		void Init();

		void Update();

		void Draw(Camera* cam, Model* model);

		void Draw(Camera* cam, Terrain* model);

		void Draw(Camera* cam, SkyDome* model);

		void Destroy();

		float getPosX() {
			return pos_x;
		}

		float getPosY() {
			return pos_y;
		}

		float getPosZ() {
			return pos_z;
		}

		float getRotX() {
			return rot_x;
		}

		float getRotY() {
			return rot_y;
		}

		float getRotZ() {
			return rot_z;
		}

		float getScaleX() {
			return scale_x;
		}

		float getScaleY() {
			return scale_y;
		}

		float getScaleZ() {
			return scale_z;
		}

		void setPos(float x, float y, float z) {
			pos_x = x;
			pos_y = y;
			pos_z = z;
		}

		void setRotation(float x, float y, float z) {
			rot_x = x;
			rot_y = y;
			rot_z = z;
		}

		void setScale(float x, float y, float z) {
			scale_x = x;
			scale_y = y;
			scale_z = z;
		}

		void setMaterial(Texture* mat) {
			material = mat;
		}

		void setFogDensity(float density) {
			fogDensityVal = density;
		}
		float getFogDensity() {
			return fogDensityVal;
		}


	private:
		// stores the program object that contains the shaders
		GLuint programId;

		// store the attribute to select into the pipline
		// to link the triangle verticies
		GLint vertexPos3DLocation;


		// link to vColour which gets a colour and passes it to fColour
		GLint vertexUVLocation;

		// link to the normal loactions
		GLint vetexNormalLocation;

		//member stores triangle vertix buffer containing the verticies
		// transfered from this code to the graphics memory


		//GLuint vboTriangle;

		float pos_x, pos_y, pos_z;
		float rot_x, rot_y, rot_z;
		float scale_x, scale_y, scale_z;

		GLuint transformUniformId;
		GLuint viewUninformId;
		GLuint projectionUniformId;
		GLuint samplerId;

		// light uniforms
		GLuint lightColourId;
		GLuint lightPositionId;
		GLuint viewPositionId;

		//fog uniforms
		GLuint fogColourId;
		GLuint fogStartId;
		GLuint fogRangeId;
		GLuint fogDensityId;

		float fogDensityVal = 0.006;
			

		// refernce to model
		Model* model;

		// material to add to th model
		Texture* material;

	};

}

