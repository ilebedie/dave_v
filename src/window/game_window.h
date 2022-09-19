#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <SDL.h>

// Legacy data structure / should be removed
struct GameState {
    bool quit = false;
    int current_level = 0;
};

struct GameWindow {
	const unsigned char DISPLAY_SCALE = 3;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	GameWindow();
	~GameWindow();
	void checkInput(GameState &game);
};
#endif