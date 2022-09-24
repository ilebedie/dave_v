#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <SDL.h>

struct GameInput {
    bool quit = false;
    int current_level = 0;
	bool try_left = false;
	bool try_right = false;
	bool try_jump = false;
	bool restart = false;

	void reset() {
		try_left = false;
		try_right = false;
		try_jump = false;
		restart = false;
	}
};

struct GameWindow {
    GameInput input;
	const unsigned char DISPLAY_SCALE = 3;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	GameWindow();
	~GameWindow();
	void checkInput();
	void fillBlack();
};
#endif