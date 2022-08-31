#ifndef DAVE_H
#define DAVE_H

#include <SDL.h>
/* Level format structure */
struct DaveLevel {
	unsigned char path[256];
	unsigned char tiles[1000];
	unsigned char padding[24];
};

struct GameState {
	bool quit = false;
	unsigned char current_level = 0;
	unsigned char view_x = 0;
	unsigned char view_y = 0;
	char scroll_x = 0;

	bool try_right;
	bool try_left;
	bool try_jump;
	bool try_down;

	DaveLevel levels[10];
	GameState();
	void loadLevels();
};


struct GameWindow {
	const unsigned char DISPLAY_SCALE = 3;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	GameWindow();
	~GameWindow();
	void checkInput(GameState &game);
};

struct GameAssets {
	GameAssets(GameWindow &gameWindow);
	SDL_Texture* graphics_tiles[158];
};

void render(const GameAssets &assets, const GameState &game, GameWindow &gameWindow);
void updateGame(GameState &game, GameWindow &gameWindow);

#endif