#include "input.h"
#include "init.h"
#include <SDL3/SDL.h>
void input(Input& inputs){
	SDL_Event event;
	inputs.left_mouse.click = false;
	inputs.mouse_motion = false;
	inputs.enter.click = false;
	inputs.t.click = false;
	inputs.c.click = false;
	inputs.f.click = false;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_EVENT_QUIT:
				RUNNING = false;
				break;
			case SDL_EVENT_KEY_DOWN:
				switch(event.key.key){
					case SDLK_RETURN:
						inputs.enter = {true, true};
						break;
					case SDLK_T:
						inputs.t = {true, true};
						break;
					case SDLK_C:
						inputs.c = {true, true};
						break;
					case SDLK_F:
						inputs.f = {true, true};
						break;
					default:
						break;
				}
				break;

			case SDL_EVENT_KEY_UP:
				switch(event.key.key){
					case SDLK_RETURN:
						inputs.enter.held = false;
						break;
					case SDLK_T:
						inputs.t.held = false;
						break;
					case SDLK_C:
						inputs.c.held =  false;
						break;
					case SDLK_F:
						inputs.f.held = false;
						break;
					default:
						break;
				}
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				switch(event.button.button){
					case SDL_BUTTON_LEFT:
						inputs.left_mouse = {true, true};
						break;
					case SDL_BUTTON_RIGHT:
						inputs.right_mouse = {true, true};
						break;
				}
				break;

			case SDL_EVENT_MOUSE_BUTTON_UP:
				switch(event.button.button){
					case SDL_BUTTON_LEFT:
						inputs.left_mouse.held = false;
						break;
					case SDL_BUTTON_RIGHT:
						inputs.right_mouse.held = false;
						break;
				}
				break;

			case SDL_EVENT_MOUSE_MOTION:
				inputs.mouse_motion = true;
				break;

			default:
				break;
		}
	}

}

