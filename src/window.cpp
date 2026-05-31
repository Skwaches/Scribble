#include "init.h"
#include "render.h"

Input INPUTS;
Display SCREEN;
Grid GRID;
bool RUNNING = true;
Grid distribution(
		{GRID.position.x + GRID.size.x, GRID.position.y}, 
		{SCREEN.size.x - GRID.size.x, static_cast<float>(SCREEN.size.y)},
		{1, 10}, {1,1}); 

int main( int argc, char** argv){
	init(SCREEN, GRID, distribution);
	while(RUNNING){
		input(INPUTS);
		update(INPUTS, GRID, distribution);
		render(SCREEN, GRID, distribution);
	}
	quit(SCREEN);
	return 0;
}
