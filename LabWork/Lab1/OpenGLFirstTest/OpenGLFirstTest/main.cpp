#define SDL_MAIN_HANDLED
#include "GameEngine.h"

#include <sstream>

using namespace GE;

int main() 
{
	GameEngine ge;

	if (!ge.Init(false)) {
		display_info_message("Could not start SDL.. Check console for details");
		return - 1;
	}

	// creates variables to get the fps
	Uint32 last_time = SDL_GetTicks(), current_time = 0;

	int frame_count = 0;


	while (ge.KeepRunning())
	{
		ge.Update();

		ge.Draw();

		frame_count++;
		current_time = SDL_GetTicks();

		if (current_time - last_time > 1000) {			

			std::ostringstream msg;

			msg << "FPS = " << frame_count;

			ge.SetWindowTitle(msg.str().c_str());

			frame_count = 0;
			last_time = current_time;

			
		}
	}

	ge.Shutdown();
	return 0;
}