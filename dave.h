#ifndef DAVE_H
#define DAVE_H

#include <SDL.h>

const int TILE_SIZE = 16;

/* Level format structure */
struct DaveLevel {
	unsigned char path[256];
	unsigned char tiles[1000];
	unsigned char padding[24];
};

struct GameState {
	bool quit = false;
	unsigned char current_level = 2;
	unsigned char view_x = 0;
	unsigned char view_y = 0;
	char scroll_x = 0;

	// Dave
	// Tile position
	unsigned char dave_x = 2;
	unsigned char dave_y = 8;
	// Pixel position
	unsigned short dave_px = dave_x * TILE_SIZE;
	unsigned short dave_py = dave_y * TILE_SIZE;
	// Try to move
	bool try_right = false;
	bool try_left = false;
	bool try_jump = false;
	bool try_fire = false;
	bool try_jetpack = false;
	bool try_down = false;	
	bool try_up = false;
	bool dave_right = false;
	bool dave_left = false;
	bool dave_jump = false;
	bool dave_fire = false;
	bool dave_jetpack = false;
	bool dave_up = false;
	bool dave_down = false;

	bool on_ground = true;
	unsigned char collisionPoints[8];
	unsigned char jump_timer = 0;
	unsigned char lives = 3;
	char direction = 0;

	// Bullets
	unsigned short dbullet_px = 0;
	unsigned short dbullet_py = 0;
	char dbullet_direction = 0;

	// Pick ups
	unsigned char pickup_x = 0;
	unsigned char pickup_y = 0;
	// Door
	bool check_door = false;

	unsigned char jetpack = 0;
	bool trophy = false;
	unsigned int score = 0;
	bool gun = false;

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

void drawDave(const GameAssets &assets, const GameState &game, GameWindow &gameWindow);

void drawWorld(const GameAssets &assets, const GameState &game, GameWindow &gameWindow);
void scrollScreen(GameState &game);
void moveDave(GameState &game);
void verifyInput(GameState &game);

int getDaveState(const GameState &game);
void clearInput(GameState &game);
void checkCollisions(GameState &game);
void applyGravity(GameState &game);
void pickUpItem(GameState &game);
void updateLevel(GameState &game);
void startLevel(GameState &game);
void updateDBullet(GameState &game);
void drawDBullet(const GameAssets &assets, const GameState &game, GameWindow &gameWindow);

#endif