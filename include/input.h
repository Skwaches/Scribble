#pragma once
typedef struct Key{ bool click = false, 
		 held = false;
}Key;
typedef struct Input{
	Key left_mouse,  
		enter,
		t,c, f,
		right_mouse;
	bool mouse_motion = false;
}Input;
void input(Input& inputs);
