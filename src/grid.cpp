#include "init.h"
#include "grid.h"

Grid::Grid( SDL_FPoint position, SDL_FPoint size, SDL_Point dimensions, SDL_FPoint spacing):
	size(size), dimensions(dimensions), position(position), 
	intensity(1, dimensions.x * dimensions.y),
	rects(dimensions.x, std::vector<SDL_FRect>(dimensions.y, {0,0,0,0}) ),
	cell( {size.x/dimensions.x, size.y/dimensions.y} ){

	SDL_FPoint spaced_Size = { cell.x - spacing.x, cell.y - spacing.y};
	for(int i = 0; i < dimensions.x; i++)
		for(int j = 0; j < dimensions.y; j++){
			rects[i][j] = { 
				position.x + spacing.x/2.0f + cell.x * i,
				position.y + spacing.y/2.0f + cell.y * j,
				spaced_Size.x, spaced_Size.y };
		}
}

void Grid::render(SDL_Renderer* renderer){
	for(int i = 0; i < dimensions.x; i++)
		for(int j = 0; j < dimensions.y; j++){
			Uint8 color = intensity[0][j * dimensions.x + i] * 255;
			SDL_CHECK(SDL_SetRenderDrawColor(renderer, color, color,color, 255 ));
			SDL_CHECK(SDL_RenderFillRect(renderer, &rects[i][j]));
		}
}

void Grid::draw(SDL_FPoint mouse_position, bool state, int brush_radius){
	SDL_Point index = {
		static_cast<int>( (mouse_position.x - position.x)/cell.x ),
		static_cast<int>( (mouse_position.y - position.y)/cell.y )
	};
	if( index.x < 0 || index.x >= dimensions.x || index.y < 0 || index.y >= dimensions.y) 
		return;

	for (int i = index.x - brush_radius; i <= index.x + brush_radius; i++) {
		for (int j = index.y - brush_radius; j <= index.y + brush_radius; j++) {
			if (i < 0 || i >= dimensions.x  || j < 0 || j >= dimensions.y) continue;

			float distance = SDL_sqrt(SDL_pow(index.x - i, 2) + SDL_pow(index.y - j, 2));
			if (distance >= brush_radius) continue;

			float power = SDL_pow(1.0f - distance / brush_radius, 3);
			float& colour = intensity[0][j * dimensions.x + i];

			if (state){
				float before = colour;
				colour = std::min(colour + 50.0f/255.0f * power, 1.0f);
			}
			else
				colour = std::max(colour - 80.0f/255.0f * power, 0.0f);
		}
	}
}

void Grid::clear(bool state){
	intensity.replace([state](){ return state ? 0.8: 0;});
}
