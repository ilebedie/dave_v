#ifndef DAVEV_H
#define DAVEV_H

#include <SDL.h>

// Entities
using Entity = unsigned long long; 
// Components
struct Position { char px, py; };
// Systems
struct RendererSystem {
    // Render entity e at position p
    void update(Entity e, Position p);
};

struct GameState {
    bool quit = false;
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

#endif