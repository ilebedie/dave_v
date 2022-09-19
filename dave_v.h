#ifndef DAVEV_H
#define DAVEV_H

#include <SDL.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

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

const short TILE_SIZE = (short)16;

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
using BoolComponent = unordered_set<Entity>;
using ArchetypeName = string; 
using ComponentName = string;

struct PixelPosition { 
    short px, py; 
    bool operator==(const PixelPosition& other) const {
        return px == other.px && py == other.py;
    }
};
using PixelPositionComponent = Component<PixelPosition>;

struct Visibility { bool visible; };
using VisibilityComponent = Component<Visibility>;

struct Tile { short tile_index; };
using TileComponent = Component<Tile>;

using NonFloatingTilesComponent = BoolComponent;

using ComponentGenericType = void *;
using ArchetypeStorage = ArrayHashMap<ComponentName, ComponentGenericType>;

// Indexes
// Custom hash function for PixelPosition
struct PixelPositionHash {
    size_t operator()(const PixelPosition& k) const {
        return ((hash<short>()(k.px) ^ (hash<short>()(k.py) << 1)) >> 1);
    }
};
// using TilePositionIndex = unordered_map<PixelPosition, Entity, hash_fn>;
using TilePositionIndex = unordered_map<PixelPosition, Entity, PixelPositionHash>;


// World / Entities
struct Entities {
    ArrayHashMap<ArchetypeName, ArchetypeStorage> archetypes;
    ArrayHashMap<ComponentName, ComponentGenericType> components;
    Entities();

    // This is here because of ComponentGenericType is void *
    // And someone should control lifetime of component
    PixelPositionComponent pixelPositionComponent;
    TileComponent tileComponent;
    VisibilityComponent visibilityComponent;
    NonFloatingTilesComponent nonFloatingTilesComponent;

    // Indexes
    TilePositionIndex tilesPositionIndex;
};

// Systems
struct RendererSystem {
    Entities &world;
    GameWindow &gameWindow;
    GameAssets &gameAssets;
    // Render entity e at position p
    RendererSystem(Entities &world, GameWindow &gameWindow, GameAssets &gameAssets) 
        : world(world), gameWindow(gameWindow), gameAssets(gameAssets) {}

    void render();
    SDL_Texture* _getTexture(int tile_index);
};

struct GravitySystem {
    Entities &world;
    // Apply gravity to non-floating entity e
    GravitySystem(Entities &world) 
        : world(world) {}

    void update();
};


#endif