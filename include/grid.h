#pragma once
#include <SDL3/SDL.h>
#include "matrix.h"
class Grid{
	private:
		SDL_FPoint size;
		SDL_FPoint position;
		SDL_Point dimensions;
		SDL_FPoint cell;
		std::vector< std::vector <SDL_FRect> > rects;
	public: 
		Matrix intensity;
		Grid(
			SDL_FPoint position = {0.0,0.0}, SDL_FPoint size = {800, 800}, 
			SDL_Point dimensions = {28, 28}, SDL_FPoint spacing = {1,1}); 
		void render(SDL_Renderer* renderer);
		void draw(SDL_FPoint mouse_position, bool state, int brush_radius);
		void clear(bool state);
};

