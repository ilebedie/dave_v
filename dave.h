#ifndef DAVE_H
#define DAVE_H

#include <SDL.h>
/* Level format structure */
struct dave_level {
	uint8_t path[256];
	uint8_t tiles[1000];
	uint8_t padding[24];
};

struct GameState {
	bool quit;
	char current_level;
	unsigned char view_x;
	unsigned char view_y;

	bool try_right;
	bool try_left;
	bool try_jump;
	bool try_down;

	dave_level levels[10];
	GameState();
};


struct GameWindow {
	const uint8_t DISPLAY_SCALE = 3;
	SDL_Window* window;
	SDL_Renderer* renderer;
	GameWindow();
	~GameWindow();
	void checkInput(GameState &game);
};

struct GameAssets {
	GameAssets(GameWindow gameWindow);
	SDL_Texture* graphics_tiles[158];
};

void render();
void updateGame();

#endif