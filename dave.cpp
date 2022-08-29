#include "dave.h"
#include <iostream>
#include <string>
#include <fstream>


int main(int argc, char* argv[])
{
    GameWindow gameWindow;
    GameAssets assets(gameWindow);
    GameState game;
    while(!game.quit)
    {
        gameWindow.checkInput(game);
        updateGame();
        render(assets, game, gameWindow);
    }
    return 0;
}

void render(const GameAssets &assets, const GameState &game, GameWindow &gameWindow)
{
    SDL_Rect dest;
	/* Draw each tile in row-major */
	for (int j=0; j < 10; j++)
	{
		dest.y = j*16;
		dest.w = 16;
		dest.h = 16;
		for (int i=0; i < 20; i++)
		{
			dest.x = i * 16;
			int tile_index = game.levels[game.current_level].tiles[j*100+game.view_x+i];
			// tile_index = update_frame(game,tile_index,i);
			SDL_RenderCopy(gameWindow.renderer, assets.graphics_tiles[tile_index], NULL, &dest);
		}
	}
    SDL_RenderPresent(gameWindow.renderer);
}

void updateGame()
{

}

GameAssets::GameAssets(GameWindow &gameWindow)
{
    for(int i = 0; i < 158; i++)
    {
        auto fileName = "assets/tiles/tile" + std::to_string(i) + ".bmp";
        auto surface = SDL_LoadBMP(fileName.c_str());
        graphics_tiles[i] = SDL_CreateTextureFromSurface(gameWindow.renderer, surface);
    }
}

GameWindow::GameWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        std::terminate();
    }
    
    if (SDL_CreateWindowAndRenderer(320*DISPLAY_SCALE, 200*DISPLAY_SCALE, 0, &window, &renderer))
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
    loadLevels();
}

void GameState::loadLevels()
{
    for (int j = 0; j < 10; j++)
    {
        auto filePath = "assets/levels/level" + std::to_string(j) + ".dat";
        std::ifstream file(filePath, std::ios::binary);
        file.read((char *)&levels[j].path, sizeof(levels[j].path));
        file.read((char *)&levels[j].tiles, sizeof(levels[j].tiles));
        file.read((char *)&levels[j].padding, sizeof(levels[j].padding));
    }
}
