#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "../window/game_window.h"

// Entities
using Entity = unsigned long long; 

using namespace std;
template<class K, class V> using ArrayHashMap = unordered_map<K, V>;

struct PixelPosition { 
    short px, py; 
    bool operator==(const PixelPosition& other) const {
        return px == other.px && py == other.py;
    }
};
struct Tile { short tile_index; };

struct GameAssets {
	SDL_Texture* graphics_tiles[158];

	GameAssets(GameWindow &gameWindow);
};

#endif