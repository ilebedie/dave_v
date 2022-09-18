#ifndef DAVEV_H
#define DAVEV_H

#include <SDL.h>
#include <string>
#include <unordered_map>

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

const int TILE_SIZE = 16;

/* Level format structure */
struct DaveLevel {
	unsigned char tiles[1000];
};

struct GameAssets {
	GameAssets(GameWindow &gameWindow);
    void loadLevels();
	SDL_Texture* graphics_tiles[158];
    DaveLevel levels[10];
};

using namespace std;
template<class K, class V> using ArrayHashMap = unordered_map<K, V>;

// Entities
using Entity = unsigned long long; 
// Components
template<class ComponentType> using Component = ArrayHashMap<Entity, ComponentType>;
using ArchetypeName = string; 
using ComponentName = string;
// struct PixelPosition { short px, py; };
// struct TilePosition { short x, y; };
struct Tile
{
    short x, y;
    short px, py;
    short tile_index;
    bool visible;
};
using TileComponent = Component<Tile>;
using ComponentGenericType = void *;
using ArchetypeStorage = ArrayHashMap<ComponentName, ComponentGenericType>;

// World / Entities
struct Entities {
    ArrayHashMap<ArchetypeName, ArchetypeStorage> archetypes;
    ArrayHashMap<ComponentName, ComponentGenericType> components;
    Entities();

    // This is here because of ComponentGenericType is void *
    // And someone should control lifetime of component
    TileComponent tileComponent;
};

// Systems
struct RendererSystem {
    Entities &world;
    GameWindow &gameWindow;
    GameAssets &gameAssets;
    GameState &game;
    // Render entity e at position p
    RendererSystem(Entities &world, GameWindow &gameWindow, GameAssets &gameAssets, GameState &game) 
        : world(world), gameWindow(gameWindow), gameAssets(gameAssets), game(game) {}

    void render();
    SDL_Texture* _getTexture(Tile tile);
};


#endif