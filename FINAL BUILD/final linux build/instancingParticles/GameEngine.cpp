#include "GameEngine.h"
#include <iostream>
#include <assimp/Importer.hpp>

namespace GE {
	GameEngine::GameEngine()
	{

	}

	GameEngine::~GameEngine()
	{

	}

	bool GameEngine::Init(bool enbVsync)
	{
		w = 900;
		h = 900;

		// initialises SDL video
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "unable to initialise SDL. SDL error: " << SDL_GetError() << std::endl;
			return false;
		}

		// set the openGL version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		// creates the sdl window
		window = SDL_CreateWindow("SDL OpenGL", 50, 50, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		// if the window does not exists respond by sending an error
		if (window == nullptr) {
			std::cerr << "Unbale to create the SDL window. SDL erroe: " << SDL_GetError() << std::endl;
			return false;
		}

		// creates the gl context and links it to the window
		glContext = SDL_GL_CreateContext(window);

		// checks if glcontext exists
		if (glContext == nullptr) {
			std::cerr << "could not create gl contetxt. SDL error: " << SDL_GetError() << std::endl;
			return false;
		}

		// starts glew
		GLenum status = glewInit();

		//checks if glew exists
		if (status != GLEW_OK) {
			std::cerr << "could not create glew. glew erroe:  " << glewGetErrorString(status) << std::endl;
			return false;
		}

		if (SDL_GL_SetSwapInterval(enbVsync) != 0) {
			std::cerr << "could not set vsysnc. SDL error: " << SDL_GetError() << std::endl;
			return false;
		}



		// crate the camera

		dist = glm::vec3(0.0f, 0.0f, -100.0f);

		cam = new Camera(glm::vec3(0.0f, 0.0f, 50.0f),//position
			glm::vec3(0.0f, 0.0f, 0.0f) + dist, // look at
			glm::vec3(0.0f, 1.0f, 0.0f), // up direction
			45.0f, // fov
			640.0f / 480.0f, //aspect ratio base on window dimentions
			0.1f, // near clip plane
			2000.0f); // far clip plane

		skybox = new SkyboxRenderer("front.jpg", "back.jpg",
			"right.jpg", "left.jpg",
			"top.jpg", "bottom.jpg");

		//// create the model
		//m = new Model();
		//// load verticlies from a file
		//bool result = m->loadFromFile(".\\space_ship_yay.obj");
		// //check it loaded
		//if (!result) {
		//	std::cerr << "Failed to load model" << std::endl;
		//}
		//shipTex = new Texture(".//space_ship_uv.jpg");
		//// create a model renderer for each space ship
		//for (int i = 0; i < 4; i++) {
		//	Spaceships.emplace_back(ModelRenderer());
		//}
		//for (int i = 0; i < Spaceships.size(); i++) {
		//	Spaceships[i].Init();
		//	Spaceships[i].setPos(i * 10, 0.0f, -100.0f);
		//	Spaceships[i].setMaterial(shipTex);
		//}

		// cactus1
		cactus1 = new Model();
		cactus1->loadFromFile("assets/cactus-pack/Cactus1.obj");
		cactus1Tex = new Texture ("assets/cactus-pack/textures/cactus_1__pbrs2a_diffuse.png");
		for (int i = 0; i < 4; i++) {
			cactus1Rend.emplace_back(ModelRenderer());
		}
		for (int i = 0; i < cactus1Rend.size(); i++) {
			cactus1Rend[i].Init();
			cactus1Rend[i].setPos(70 + i * 20, -20, 3.0f + i * 20);
			cactus1Rend[i].setMaterial(cactus1Tex);
			cactus1Rend[i].setScale(0.1, 0.1, 0.1);
			cactus1Rend[i].setRotation(0, i * 32, 0);
		}

		// cactus2
		cactus2 = new Model();
		cactus2->loadFromFile("assets/cactus-pack/Cactus4.obj");
		cactus2Tex = new Texture("assets/cactus-pack/textures/cactus_4__pbrs2a_diffuse.png");
		for (int i = 0; i < 2; i++) {
			cactus2Rend.emplace_back(ModelRenderer());
		}
		for (int i = 0; i < cactus2Rend.size(); i++) {
			cactus2Rend[i].Init();
			cactus2Rend[i].setPos(30, -20, 80 + i * 25);
			cactus2Rend[i].setMaterial(cactus2Tex);
			cactus2Rend[i].setScale(0.1, 0.1, 0.1);
			cactus2Rend[i].setRotation(0, i * 120, 0);
		}



		// cart
		cart = new Model();
		cart->loadFromFile("assets/cart.obj");
		cartTex = new Texture("assets/cart.png");
		cartRend = new ModelRenderer();
		cartRend -> Init();
		cartRend->setMaterial(cartTex);
		cartRend->setPos(85, -13, 80);
		cartRend->setScale(0.1, 0.1, 0.1);


		// horse
		horse = new Model();
		horse->loadFromFile("assets/horse_textured_low_poly/scene.gltf");
		horseTex = new Texture("assets/horse_textured_low_poly/textures/Material_baseColor.png");
		horseRend = new ModelRenderer();
		horseRend->Init();
		horseRend->setMaterial(horseTex);
		horseRend->setPos(50, -20, 70);
		horseRend->setScale(5, 5, 5);
		horseRend->setRotation(-90, 0, 0);

		// saloon
		saloon = new Model();
		saloon->loadFromFile("assets/saloon/scene.gltf");
		saloonTex = new Texture("assets/saloon/textures/saloon.001_baseColor.png");
		saloonRend = new ModelRenderer();
		saloonRend->Init();
		saloonRend->setMaterial(saloonTex);
		saloonRend->setPos(200, 30, 200);
		saloonRend->setScale(0.2, 0.2, 0.2);
		saloonRend->setRotation(90, 0, 0);

		//// terain
		terrainRenderer = new ModelRenderer();
		terrainRenderer->Init();
		terrain = new Terrain();

		terrainTexture = new Texture("terrain-texture.png");



		terrainRenderer->setMaterial(terrainTexture);


		terrainRenderer->setPos(0, -20, 0);
		terrainRenderer->setScale(4, 0.2, 4);
		//terrainRenderer->setRotation(45, 180, 0);

		// skydome
		skyDome = new SkyDome();
		//SkyDomeTex = new Texture(".\\sunsetSkyDome.png");
		SkyDomeTex = new Texture("clear-blue-sky-1.png");


		SkyDomeRenderer = new ModelRenderer();
		SkyDomeRenderer->Init();
		SkyDomeRenderer->setMaterial(SkyDomeTex);
		SkyDomeRenderer->setPos(0, 0, 0);
		SkyDomeRenderer->setScale(1, -0.75F, 1);
		SkyDomeRenderer->setRotation(180, 0, 0);

		//instancing
		/*for (int i = 0; i < 10; i++) {
			cactusInstance.push_back(modelInstance{ (float)(rand() % 100 + 1), 1.0f,(float)(rand() % 100 + 1),0.0f,0.0f,0.0f,0.1f,0.1f,0.1f, });
		}
		ir = new instancingRenderer;
		ir->Init();
		ir->instancedData(cactusInstance);
		ir->setMaterial(cactus1Tex);*/

		// UI
		//fog denstity slider
		Texture* slider = new Texture("assets/yellowBlock.png");
		sliderFogDensity = new GUIImage(752, 52, slider);

		Texture* sliderBackground = new Texture("assets/border.png");
		borderFogDensity = new GUIImage(750, 50, sliderBackground);

		//fogDesRend = new GUIRenderer();
		//fogDesRend->init(w, h);
		fpsTxt = new GUIText(0, 0, "FPS", "assets/font/arial.ttf");

		//fps
		fpsRend = new GUIRenderer();
		fpsRend->init(w, h);
		fpsTxt = new GUIText(0, 0, "FPS", "assets/font/arial.ttf");

		fogDenTxt = new GUIText(750, 20, "Fog Density", "assets/font/arial.ttf");

		// minimap
		Texture* miniMapTex = new Texture("terrain-texture.png");
		minimap = new GUIImage(0, 0, miniMapTex);

		Texture* miniMapPlayerTex = new Texture("assets/PlayerSprite.png");
		playerMiniMap = new GUIImage(playerMiniMapX, playerMiniMapY, miniMapPlayerTex);

		Texture* HudTex = new Texture("assets/hud2.png");
		Hud = new GUIImage(0, 0, HudTex);


		return true;
	}

	bool GameEngine::KeepRunning()
	{
		SDL_PumpEvents();

		SDL_Event evt;

		// check for quit event
		if (SDL_PeepEvents(&evt, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
			// if the user quits stop the program
			return false;
		}
		return true;
	}

	void GameEngine::processInput()
	{


		// ---------------- make these variables more easerly changable
		// ----------------- also move this code so it is inside of camera
		const float camSpeed = 1.0f;
		const float mouseSens = 0.4f;

		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		float diffx = mouse_x - cam->getOldMouseX();
		float diffy = cam->getOldMouseY() - mouse_y;

		cam->setYaw((cam->getYaw() + diffx) * mouseSens);
		cam->setPitch((cam->getPitch() + diffy) * mouseSens);

		glm::vec3 direction;
		direction.x = cos(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		direction.y = sin(glm::radians(cam->getPitch()));
		direction.z = sin(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		cam->setTarget(glm::normalize(direction));

		// array for key states
		bool keyStates[9] = { 0,0,0,0,0,0,0,0 };
		int FORWARD = 0, BACKWARD = 1, LEFT = 2, RIGHT = 3, UP = 4, DOWN = 5, increaseFog = 6, decreaseFog = 7;

		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W:
					keyStates[FORWARD] = true;
					break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S:
					keyStates[BACKWARD] = true;
						break;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A:
					keyStates[LEFT] = true;
					break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D:
					keyStates[RIGHT] = true;
					break;
				case SDL_SCANCODE_E:
					keyStates[UP] = true;
					break;
				case SDL_SCANCODE_Q:
					keyStates[DOWN] = true;
					break;
				case SDL_SCANCODE_O:
					keyStates[decreaseFog] = true;
					break;
				case SDL_SCANCODE_P:
					keyStates[increaseFog] = true;
					break;
				default:
					break;
				}
			}
		}

		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
					keyStates[FORWARD] = false;
					break;
				case SDL_SCANCODE_DOWN:
					keyStates[BACKWARD] = false;
					break;
				case SDL_SCANCODE_LEFT:
					keyStates[LEFT] = false;
					break;
				case SDL_SCANCODE_RIGHT:
					keyStates[RIGHT] = false;
					break;
				case SDL_SCANCODE_E:
					keyStates[UP] = false;
					break;
				case SDL_SCANCODE_Q:
					keyStates[DOWN] = false;
					break;
				case SDL_SCANCODE_O:
					keyStates[decreaseFog] = false;
					break;
				case SDL_SCANCODE_P:
					keyStates[increaseFog] = false;
					break;
				default:
					break;
				}
			}
		}

		// camera movement
		if (keyStates[FORWARD]) {
			cam->setPos(cam->getPos() + cam->getTarget() * camSpeed);
		}
		if (keyStates[BACKWARD]) {
			cam->setPos(cam->getPos() - cam->getTarget() * camSpeed);
		}
		if (keyStates[LEFT]) {
			cam->setPos(cam->getPos() - glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed);
		}
		if (keyStates[RIGHT]) {
			cam->setPos(cam->getPos() + glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed);
		}
		if (keyStates[UP]) {
			cam->setPos(cam->getPos().x, cam->getPos().y +10 , cam->getPos().z);
		}
		if (keyStates[DOWN]) {
			cam->setPos(cam->getPos().x, cam->getPos().y - 10, cam->getPos().z);
		}

		// fog density
		NewFogDensity = SkyDomeRenderer->getFogDensity();

		if (keyStates[decreaseFog]) {

			NewFogDensity -= 0.0002;
			if (NewFogDensity <= 0) {
				NewFogDensity = 0;
			}

			cartRend->setFogDensity(NewFogDensity);
			horseRend->setFogDensity(NewFogDensity);
			saloonRend->setFogDensity(NewFogDensity);
			for (int i = 0; i < cactus1Rend.size(); i++) {
				cactus1Rend[i].setFogDensity(NewFogDensity);
			}
			for (int i = 0; i < cactus2Rend.size(); i++) {
				cactus2Rend[i].setFogDensity(NewFogDensity);
			}
			terrainRenderer->setFogDensity(NewFogDensity);
			SkyDomeRenderer->setFogDensity(NewFogDensity);
		}
		if (keyStates[increaseFog]) {

			NewFogDensity += 0.0002;
			if (NewFogDensity >= 0.1) {
				NewFogDensity = 0.1;
			}

			cartRend->setFogDensity(NewFogDensity);
			horseRend->setFogDensity(NewFogDensity);
			saloonRend->setFogDensity(NewFogDensity);
			for (int i = 0; i < cactus1Rend.size(); i++) {
				cactus1Rend[i].setFogDensity(NewFogDensity);
			}
			for (int i = 0; i < cactus2Rend.size(); i++) {
				cactus2Rend[i].setFogDensity(NewFogDensity);
			}
			terrainRenderer->setFogDensity(NewFogDensity);
			SkyDomeRenderer->setFogDensity(NewFogDensity);
		}

	}

	void GameEngine::Update()
	{
		// update fps counter
		float currentTime = SDL_GetTicks();
		if (currentTime - lastTime > 1000) {
			std::string msg;

			msg = "FPS= " + std::to_string(frameCount);
			fpsTxt->setText(msg);

			frameCount = 0;
			lastTime = currentTime;
		}
		frameCount++;

		//// rotate the camera
		//glm::mat4 cam_rot = glm::mat4(1.0f);
		//// rotation around the y axis
		//cam_rot = glm::rotate(cam_rot, glm::radians(0.01f), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::vec4 temp = glm::vec4(dist, 0.0f);
		//temp = temp * cam_rot;
		//dist = glm::vec3(temp.x, temp.y, temp.z);
		//cam->setTarget(dist);
		cam->updateCamMatrices();

		cam->setOldMouseX(w / 2);
		cam->setOldMouseY(h / 2);

		// update minimap
		float posX = cam->getPosX();
		float posY = cam->getPosZ();

		// my own clamp
		if (posX <= 0) {
			posX = 0;
		}
		else if (posX >= 200) {
			posX = 200;
		}

		if (posY <= 0) {
			posY = 0;
		}
		else if (posY >= 200) {
			posY = 200;
		}


		playerMiniMap->setX(posX);
		playerMiniMap->setY(posY);

		// updates sky dome
		if (cam->getPosX() - SkyDomeRenderer->getPosX() > 50 || cam->getPosX() - SkyDomeRenderer->getPosX() < -50) {
			SkyDomeRenderer->setPos(cam->getPosX(), SkyDomeRenderer->getPosY(), SkyDomeRenderer->getPosZ());
		}
		if (cam->getPosY() - SkyDomeRenderer->getPosY() > 50 || cam->getPosY() - SkyDomeRenderer->getPosY() < -50) {
			SkyDomeRenderer->setPos(SkyDomeRenderer->getPosX(), cam->getPosY(), SkyDomeRenderer->getPosZ());
		}
		if (cam->getPosZ() - SkyDomeRenderer->getPosZ() > 50 || cam->getPosZ() - SkyDomeRenderer->getPosZ() < -50) {
			SkyDomeRenderer->setPos(SkyDomeRenderer->getPosX(), SkyDomeRenderer->getPosY(), cam->getPosZ());
		}
	}

	void GameEngine::Draw()
	{
		// clears the background
		//glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// skybox renders first
		//skybox->draw(cam);

		// instances
		//ir->Draw(cam, cactus1, cactusInstance);

		cartRend->Draw(cam, cart);

		horseRend->Draw(cam, horse);

		saloonRend->Draw(cam, saloon);

		for (int i = 0; i < cactus1Rend.size(); i++) {
			cactus1Rend[i].Draw(cam, cactus1);
		}

		for (int i = 0; i < cactus2Rend.size(); i++) {
			cactus2Rend[i].Draw(cam, cactus2);
		}



		terrainRenderer->Draw(cam, terrain);

		SkyDomeRenderer->Draw(cam, skyDome);

		// ui
		glDisable(GL_DEPTH_TEST);


		fpsRend->drawImage(minimap,0.25,0.25);
		fpsRend->drawImage(playerMiniMap,0.1,0.1);
		fpsRend->drawText(fpsTxt);
		fpsRend->drawImage(Hud,1,1);

		//fog denstity
		fpsRend->drawImage(borderFogDensity, 0.1, .05);
		fpsRend->drawImage(sliderFogDensity, 0.097 * (NewFogDensity*10), .045);
		fpsRend->drawText(fogDenTxt);

		glEnable(GL_DEPTH_TEST);
		SDL_GL_SwapWindow(window);
	}

	void GameEngine::Shutdown()
	{
		skybox->destroy();
		delete skybox;
		delete skyDome;
		cartRend->Destroy();


		SDL_DestroyWindow(window);
		window = nullptr;

		SDL_Quit();
	}

	void GameEngine::SetWindowTitle(const char* newTitle)
	{
		SDL_SetWindowTitle(window, newTitle);
	}


	void display_info_message(const char* msg)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "basic game engine", msg, nullptr);
	}
}
