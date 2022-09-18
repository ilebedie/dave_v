#include "dave_v.h"
#include <iostream>
#include <string>
#include <fstream>

long long generateEid() {
    return 0;
}

Entities::Entities() {
    Tile tile = {10, 10, 20, 20, 18, true};
    long long eid = generateEid();
    tileComponent[eid] = tile;
    
    components["Tile"] = &tileComponent; 
    // Initialize components
}

SDL_Texture* RendererSystem::_getTexture(Tile t) {
    // Move to texture atlas to a component
    return gameAssets.graphics_tiles[t.tile_index];
}

void RendererSystem::render() {
    SDL_SetRenderDrawColor(gameWindow.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(gameWindow.renderer);

    // TODO: Tile is actually an archetype
    // It should be broken out into a separate components
    auto tilesComponent = world.components["Tile"];
    auto tiles = (TileComponent *)tilesComponent;
    for (const auto& [entity, tile] : *tiles) {
        if (!tile.visible) continue;

        SDL_Rect dest;
        dest.y = tile.py;
        dest.w = TILE_SIZE;
        dest.h = TILE_SIZE;
	 	dest.x = tile.px;
        auto tileTexture = _getTexture(tile);

        SDL_RenderCopy(gameWindow.renderer, tileTexture, NULL, &dest);
    }
    SDL_RenderPresent(gameWindow.renderer);
}

void GravitySystem::update() {
    auto tilesComponent = world.components["Tile"];
    auto tiles = (TileComponent *)tilesComponent;
    for (const auto& [entity, tile] : *tiles) {
        if (!tile.visible) continue;
        tile.py += 1;
    }
}

int main(int argc, char* argv[])
{
    GameWindow gameWindow;
    GameAssets assets(gameWindow);
    GameState game;
    Entities world;
    RendererSystem renderer(world, gameWindow, assets, game);

    while(!game.quit)
    {
        auto timer_begin = SDL_GetTicks();
        gameWindow.checkInput(game);
        renderer.render();
        // updateGame(game, gameWindow);
        auto timer_end = SDL_GetTicks();
        auto delay = 33 - (timer_end - timer_begin);
        // delay > 33 means that negative overflow happened
        delay = delay > 33 ? 0 : delay;
        SDL_Delay(delay);
    }
    return 0;

}


GameAssets::GameAssets(GameWindow &gameWindow) {
    for(int i = 0; i < 158; i++) {
        auto fileName = "assets/tiles/tile" + std::to_string(i) + ".bmp";
        auto surface = SDL_LoadBMP(fileName.c_str());
        graphics_tiles[i] = SDL_CreateTextureFromSurface(gameWindow.renderer, surface);
        SDL_FreeSurface(surface);
    }
}


GameWindow::GameWindow() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        std::terminate();
    }
    
    if (SDL_CreateWindowAndRenderer(320*DISPLAY_SCALE, 200*DISPLAY_SCALE, 0, &window, &renderer)) {
        std::cout << "SDL_CreateWindowAndRenderer Error: " << SDL_GetError() << std::endl;
        std::terminate();
    }
    SDL_RenderSetScale(renderer, DISPLAY_SCALE, DISPLAY_SCALE);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
}

void GameWindow::checkInput(GameState &game) {
	SDL_Event event;
	SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) game.quit = true;
    auto keyState = SDL_GetKeyboardState(NULL);
    // if (keyState[SDL_SCANCODE_UP]) game.try_jump = true;
    if (keyState[SDL_SCANCODE_ESCAPE]) game.quit = true;
}

GameWindow::~GameWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

