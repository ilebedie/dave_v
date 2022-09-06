#ifndef DAVEV_H
#define DAVEV_H

#include <SDL.h>
#include <unordered_map>

// Entities
using Entity = unsigned long long; 
using Archetype = unsigned long long; 
// Components
struct Position { char px, py; };

struct ArchetypeStorage {
    // hash of every component in this archetype
    unsigned long long hash;
    std::unordered_map<std::string, void *> components;
};

struct Entities {
    std::unordered_map<Archetype, ArchetypeStorage> archetypes;
    Entity entities[1000];
    Position positions[1000];
    int size = 0;
};

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