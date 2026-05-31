#include "init.h"
#include "read.h"
#include <filesystem>
#include "machine.h"

void  destroy_Display(Display& display){
	SDL_DestroyWindow(display.window);
	SDL_DestroyRenderer(display.renderer);
}

void update(Input& inputs, Grid& grid, Grid& distribution){
	//Scribbling
	bool scribble = true;
	if( inputs.left_mouse.held && inputs.mouse_motion ){
		SDL_FPoint mouse_position;
		SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
		grid.draw(mouse_position, true, 2);
	}
	else if(inputs.right_mouse.held && inputs.mouse_motion){
		SDL_FPoint mouse_position;
		SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
		grid.draw(mouse_position, false, 2);
	}
	//Clearing screen
	else if(inputs.c.click)
		grid.clear(false);

	else if(inputs.f.click)
		grid.clear(true);

	else 
		scribble = false;

	//Change has occured! update the distribution!
	if(scribble){
		distribution.intensity = distribution_Data(grid.intensity);
	}
	
	//Guess number
	if(inputs.enter.click)
		classify(grid.intensity);

	//Train model.
	if(inputs.t.click)
		trainer();
}


void init(Display& screen, Grid& grid, Grid& distribution){
	if(std::filesystem::exists(weightsFile)){
		loadWeights(weightsFile);
	}
	else {
		initMachine();
		for(int i = 0; i < 7; i++)
			trainer();
	}

	SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
	SDL_CHECK(SDL_CreateWindowAndRenderer(
				screen.title, screen.size.x, screen.size.y, 
				SDL_WINDOW_RESIZABLE, &screen.window,
				&screen.renderer));
	distribution.intensity = distribution_Data(grid.intensity);
}
void quit(Display& screen){
	destroy_Display(screen);
	SDL_Quit();
}
