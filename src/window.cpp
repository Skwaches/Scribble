#include "init.h"
#include "render.h"

Input INPUTS;
Display SCREEN;
Grid GRID(
		{0, 0}, 
		{static_cast<float>(SCREEN.size.y), static_cast<float>(SCREEN.size.y)},
		{28, 28}, {10,10}); 

bool RUNNING = true;
Grid distribution(
		{GRID.position.x + GRID.size.x, GRID.position.y}, 
		{SCREEN.size.x - GRID.size.x - 300, static_cast<float>(SCREEN.size.y)},
		{1, 10}, {10,40}); 

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
