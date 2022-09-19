#include "game_window.h"
#include <iostream>

using namespace std;

GameWindow::GameWindow() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        terminate();
    }
    
    if (SDL_CreateWindowAndRenderer(320*DISPLAY_SCALE, 200*DISPLAY_SCALE, 0, &window, &renderer)) {
        cout << "SDL_CreateWindowAndRenderer Error: " << SDL_GetError() << endl;
        terminate();
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