#include "render.h"

void render(Display& screen, Grid& grid, Grid& distribution){
	SDL_CHECK(SDL_RenderClear(screen.renderer));
	grid.render(screen.renderer);
	distribution.render(screen.renderer);
	SDL_CHECK(SDL_SetRenderDrawColor(screen.renderer, 20, 20, 20, 255));
	SDL_CHECK(SDL_RenderPresent(screen.renderer));
}
