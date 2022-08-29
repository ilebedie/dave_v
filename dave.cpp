#include "dave.h"
#include <iostream>


int main(int argc, char* argv[])
{
    GameWindow gameWindow;
    GameAssets assets(gameWindow);
    GameState game;
    while(!game.quit)
    {
        gameWindow.checkInput(game);
        updateGame();
        render();
    }
    return 0;
}

void render()
{

}

void updateGame()
{

}

GameAssets::GameAssets(GameWindow gameWindow)
{

}

GameWindow::GameWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        std::terminate();
    }
    
    if (SDL_CreateWindowAndRenderer(320*DISPLAY_SCALE, 240*DISPLAY_SCALE, 0, &window, &renderer))
    {
        std::cout << "SDL_CreateWindowAndRenderer Error: " << SDL_GetError() << std::endl;
        std::terminate();
    }
    SDL_RenderSetScale(renderer, DISPLAY_SCALE, DISPLAY_SCALE);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
}

void GameWindow::checkInput(GameState &game)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	const uint8_t *keystate = SDL_GetKeyboardState(NULL);
	if ( keystate[SDL_SCANCODE_RIGHT] )
		game.try_right = true;
	if ( keystate[SDL_SCANCODE_LEFT] )
		game.try_left = true;
	if ( keystate[SDL_SCANCODE_UP] )
		game.try_jump = true;
	if ( keystate[SDL_SCANCODE_DOWN] )
		game.try_down = true;
	if (event.type == SDL_QUIT)
		game.quit = true;
}

GameWindow::~GameWindow()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

GameState::GameState()
{
    quit = false;
    current_level = 0;
    view_x = 0;
    view_y = 0;
}