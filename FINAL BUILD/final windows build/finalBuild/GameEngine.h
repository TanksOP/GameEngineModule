#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include<iostream>
#include<algorithm>

#include "Camera.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "Terrain.h"
#include "SkyboxRenderer.h"
#include "SkyDome.h"

#include "GUIRenderer.h"
#include"FontRenderer.h"
#include "instancingRenderer.h"



namespace GE {

	class GameEngine {
	public:
		GameEngine(); // constructor

		virtual ~GameEngine(); //Destuctor

		bool Init(bool);
		bool KeepRunning();
		void processInput();
		void Update();
		void Draw(); 
		void Shutdown();

		void SetWindowTitle(const char*);


	private:
		SDL_Window* window;

		SDL_GLContext glContext;

		// camera
		Camera* cam;
		// for the distance/cam to look at
		glm::vec3 dist;

		// model objects
		/*Model* m;
		ModelRenderer* mr;
		Texture* shipTex;*/

		// cart model
		Model* cart;
		ModelRenderer* cartRend;
		Texture* cartTex;


		// horse model
		Model* horse;
		ModelRenderer* horseRend;
		Texture* horseTex;

		// saloon model
		Model* saloon;
		ModelRenderer* saloonRend;
		Texture* saloonTex;

		// cactus1 model
		Model* cactus1;
		std::vector<ModelRenderer> cactus1Rend;
		Texture* cactus1Tex;

		// cactus2 model
		Model* cactus2;
		std::vector<ModelRenderer> cactus2Rend;
		Texture* cactus2Tex;


		ModelRenderer* terrainRenderer;
		Texture* terrainTexture;
		Terrain* terrain;
		SkyboxRenderer* skybox;

		ModelRenderer* SkyDomeRenderer;
		Texture* SkyDomeTex;
		SkyDome* skyDome;

		
		// 
		//fog
		float NewFogDensity = 0;

		// UI
		float lastTime = 0;
		int frameCount;

		GUIRenderer* fpsRend;
		GUIText* fpsTxt;

		GUIImage* minimap;
		GUIImage* playerMiniMap;
		float playerMiniMapX = 0;
		float playerMiniMapY =0;

		GUIImage* Hud;

		GUIImage* borderFogDensity;
		GUIText* fogDenTxt;
		GUIImage* sliderFogDensity;

		// instancing
		instancingRenderer* ir;
		std::vector<modelInstance> cactusInstance;


		int w = 640;
		int h = 480;	
	};

	// helper function 

	void display_info_message(const char*);

}