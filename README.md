As a part of my university course, I created a game engine in CPP, using some of the features from OpenGL and SDL2.

The game engine allows you to:
* Load models
* Add textures to the models
* Add a terrain
* Add a skybox or skydome
* Add fog
* Have a first-person camera that can move around the scene
* Display FPS
* Minimap
* Add a HUD
* Move any of the models through code

All the functions of the game engine such as adding in models has to be done through code.  
By using this game engine I created a wild-west inspired scene, which you can see below.

![image](https://user-images.githubusercontent.com/71771303/167316743-b29ab825-407e-4132-bda3-71015714717f.png)

The Game engine works on both Linux and Windows.  
In the FINAL BUILD  folder, you can choose the correct subfolder for the OS and then can open the final Build.sln to access all of the code.  

If you are trying to use the game engine the main file that needs changing is "GameEngine.cpp", inside the "Init" is how you create new models that can be added into the scene, both the "Update" and "Draw" functions are called repeatedly, and are used to move anything during runtime, and used to draw/show the game objects that were created in the "Init". 


