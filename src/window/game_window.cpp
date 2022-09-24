#include "game_window.h"
#include <iostream>

using namespace std;

void GameWindow::fillBlack() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

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

void GameWindow::checkInput() {
	SDL_Event event;
	SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) input.quit = true;
    auto keyState = SDL_GetKeyboardState(NULL);
    // if (keyState[SDL_SCANCODE_UP]) game.try_jump = true;

    // TODO: should check for key pressed/released
    if (keyState[SDL_SCANCODE_UP]) input.try_jump = true;
    if (keyState[SDL_SCANCODE_LEFT]) input.try_left = true;
    if (keyState[SDL_SCANCODE_RIGHT]) input.try_right = true;
    if (keyState[SDL_SCANCODE_ESCAPE]) input.quit = true;
    if (keyState[SDL_SCANCODE_R]) input.restart = true;
}

GameWindow::~GameWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}