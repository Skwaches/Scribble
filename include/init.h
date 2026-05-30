#pragma once
#include "grid.h"
#include "input.h"

extern bool RUNNING;

#define SDL_CHECK(expression) do {\
	bool success= expression;\
	if(!success){\
		SDL_Log("Runtime Error! , %s:%i, %s\n", __FILE__, __LINE__, SDL_GetError());\
		RUNNING = false;\
	}\
} while(0)

typedef struct Display{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Point size = {800,800};
	const char* title = "Scribble";
}Display;

void destroy_Display(Display& display);
void update(Input& inputs, Grid& grid);
void init(Display& screen);
void quit(Display& screen);
void render(Display& screen, Grid& grid);
