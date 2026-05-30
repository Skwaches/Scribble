#include "init.h"
#include "machine.h"
#include <filesystem>

Input INPUTS;
Display SCREEN;
Grid GRID;
bool RUNNING = true;

void  destroy_Display(Display& display){
	SDL_DestroyWindow(display.window);
	SDL_DestroyRenderer(display.renderer);
}


void init(Display& screen){
	SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
	SDL_CHECK(SDL_CreateWindowAndRenderer(
				screen.title, screen.size.x, screen.size.y, 
				SDL_WINDOW_RESIZABLE, &screen.window,
				&screen.renderer));

	if(std::filesystem::exists(weightsFile))
		loadWeights(weightsFile);
	else {
		initMachine();
		trainer();
	}
}


void update(Input& inputs, Grid& grid){

	//Scribbling
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
	if(inputs.c.click)
		grid.clear(false);
	else if(inputs.f.click)
		grid.clear(true);

	//Guess number
	if(inputs.enter.click)
		classify(grid.intensity);

	//Train model.
	if(inputs.t.click)
		trainer();
	

}

void render(Display& screen, Grid& grid){
	SDL_CHECK(SDL_RenderClear(screen.renderer));
	grid.render(screen.renderer);
	SDL_CHECK(SDL_SetRenderDrawColor(screen.renderer, 20, 20, 20, 255));
	SDL_CHECK(SDL_RenderPresent(screen.renderer));
}

void quit(Display& screen){
	destroy_Display(screen);
	SDL_Quit();
}

int main( int argc, char** argv){
	init(SCREEN);
	while(RUNNING){
		input(INPUTS);
		update(INPUTS, GRID);
		render(SCREEN, GRID);
	}
	quit(SCREEN);
	return 0;
}
