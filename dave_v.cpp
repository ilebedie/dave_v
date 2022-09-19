#include "dave_v.h"
#include <iostream>
#include <string>
#include <fstream>

long long generateEid() {
    return 0;
}

Entities::Entities() {
    Entity eid = 0;// generateEid();
    Tile tile = {18}; // visible tile, hangs in the air
    tileComponent[eid] = tile;
    Visibility visibility = {true};
    visibilityComponent[eid] = visibility;
    PixelPosition pixelPosition = {20, 20};
    pixelPositionComponent[eid] = pixelPosition;
    nonFloatingTilesComponent.insert(eid);
    tilesPositionIndex[pixelPosition] = eid;

    Entity eid1 = 1;// generateEid();
    Tile tile1 = {17};
    tileComponent[eid1] = tile1;
    Visibility visibility1 = {false};
    visibilityComponent[eid1] = visibility1;
    PixelPosition pixelPosition1 = {40, 40};
    pixelPositionComponent[eid1] = pixelPosition1;
    tilesPositionIndex[pixelPosition1] = eid1;

    Entity eid2 = 2;// generateEid();
    Tile tile2 = {20}; // visible tile, falls down
    tileComponent[eid2] = tile2;
    Visibility visibility2 = {true};
    visibilityComponent[eid2] = visibility2;
    PixelPosition pixelPosition2 = {60, 60};
    pixelPositionComponent[eid2] = pixelPosition2;
    tilesPositionIndex[pixelPosition2] = eid2;

    Entity eid3 = 3;// generateEid();
    Tile tile3 = {19}; // visible tile, falls down
    tileComponent[eid3] = tile3;
    Visibility visibility3 = {true};
    visibilityComponent[eid3] = visibility3;
    PixelPosition pixelPosition3 = {20, 80};
    pixelPositionComponent[eid3] = pixelPosition3;
    tilesPositionIndex[pixelPosition3] = eid3;
    
    components["Tile"] = &tileComponent; 
    components["Visibility"] = &visibilityComponent;
    components["PixelPosition"] = &pixelPositionComponent;
    components["NonFloatingTiles"] = &nonFloatingTilesComponent;
}

SDL_Texture* RendererSystem::_getTexture(int tile_index) {
    // Move to texture atlas to a component
    return gameAssets.graphics_tiles[tile_index];
}

void RendererSystem::render() {
    SDL_SetRenderDrawColor(gameWindow.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(gameWindow.renderer);

    // TODO: combine these components into archetype
    auto tilesComponent = world.components["Tile"];
    auto tiles = (TileComponent *)tilesComponent;
    auto visibilityComponent = world.components["Visibility"];
    auto visibilityP = (VisibilityComponent *)visibilityComponent;
    auto pixelPositionComponent = world.components["PixelPosition"];
    auto pixelPositionP = (PixelPositionComponent *)pixelPositionComponent;

    for (auto it = tiles->begin(); it != tiles->end(); ++it) {
        auto eid = it->first;
        auto tile = it->second;
        auto visibility = (*visibilityP)[eid];
        auto pixelPosition = (*pixelPositionP)[eid];
        if (visibility.visible) {
            SDL_Rect dstrect = {pixelPosition.px, pixelPosition.py, TILE_SIZE, TILE_SIZE};
            SDL_RenderCopy(gameWindow.renderer, _getTexture(tile.tile_index), NULL, &dstrect);
        }
    }

    SDL_RenderPresent(gameWindow.renderer);
}

void GravitySystem::update() {
    auto tilesComponent = world.components["Tile"];
    auto tilesP = (TileComponent *)tilesComponent;
    auto nonFloatingTilesComponent = world.components["NonFloatingTiles"];
    auto nonFloatingTilesP = (NonFloatingTilesComponent *)nonFloatingTilesComponent;
    auto tilesPositionIndexP = &world.tilesPositionIndex;
    for (auto eid = nonFloatingTilesP->begin(); eid != nonFloatingTilesP->end(); ++eid) {
        auto& pixelPosition = world.pixelPositionComponent[*eid];

        short tileBelowY = pixelPosition.py + TILE_SIZE;
        PixelPosition tileBelow {pixelPosition.px, tileBelowY};

        if (tilesPositionIndexP->find(tileBelow) == tilesPositionIndexP->end()) {
            tilesPositionIndexP->erase(pixelPosition);
            pixelPosition.py += 1;
            world.pixelPositionComponent[*eid] = pixelPosition;
            tilesPositionIndexP->insert({pixelPosition, *eid});
        }
    }
}

int main(int argc, char* argv[])
{
    GameWindow gameWindow;
    GameAssets assets(gameWindow);
    GameState game;
    Entities world;
    RendererSystem renderer(world, gameWindow, assets);
    GravitySystem gravitySystem(world);

    while(!game.quit)
    {
        auto timer_begin = SDL_GetTicks();
        gameWindow.checkInput(game);
        renderer.render();
        gravitySystem.update();
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

