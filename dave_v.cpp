#include "dave_v.h"
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char* argv[])
{
    GameWindow gameWindow;
    GameAssets assets(gameWindow);
    GameState game;
    Entities world;

    while(!game.quit)
    {
        auto timer_begin = SDL_GetTicks();
        gameWindow.checkInput(game);
        // updateGame(game, gameWindow);
        // render(assets, game, gameWindow);
        auto timer_end = SDL_GetTicks();
        auto delay = 33 - (timer_end - timer_begin);
        // delay > 33 means that negative overflow happened
        delay = delay > 33 ? 0 : delay;
        SDL_Delay(delay);
    }
    return 0;

}


void RendererSystem::update(Entity e, Position p) {
    // Render entity e at position p
}

GameAssets::GameAssets(GameWindow &gameWindow)
{
    for(int i = 0; i < 158; i++)
    {
        auto fileName = "assets/tiles/tile" + std::to_string(i) + ".bmp";
        		/* Handle Dave tile masks */
		if ((i >= 53 && i <= 59) || i == 67 || i == 68 || (i >= 71 && i <= 73) || (i >= 77 && i <= 82))
		{
            // In sprite sheet masks should be stored next to the tile
            unsigned char mask_offset = 0;
			if (i >= 53 && i <= 59)
				mask_offset = 7;
			if (i >= 67 && i <= 68)
				mask_offset = 2;
			if (i >= 71 && i <= 73)
				mask_offset = 3;
			if (i >= 77 && i <= 82)
				mask_offset = 6;
            auto maskFileName = "assets/tiles/tile" + std::to_string(i+mask_offset) + ".bmp";

			auto surface = SDL_LoadBMP(fileName.c_str());
			auto mask = SDL_LoadBMP(maskFileName.c_str());

			auto surf_p = (uint8_t*) surface->pixels;
			auto mask_p = (uint8_t*) mask->pixels;

			/* Write mask white background to dave tile */
			for (int j=0; j<(mask->pitch*mask->h); j++)
				surf_p[j] = mask_p[j] ? 0xFF : surf_p[j];

			/* Make white mask transparent */
			SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
			graphics_tiles[i] = SDL_CreateTextureFromSurface(gameWindow.renderer, surface);
			SDL_FreeSurface(surface);
			SDL_FreeSurface(mask);
		}
        else 
        {
			auto surface = SDL_LoadBMP(fileName.c_str());

			/* Monster tiles should use black transparency */
			if ((i >= 89 && i <= 120 ) || (i >= 129 && i <= 132 ))
				SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
			graphics_tiles[i] = SDL_CreateTextureFromSurface(gameWindow.renderer, surface);
			SDL_FreeSurface(surface);
        }
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
    if (event.type == SDL_QUIT) game.quit = true;
    auto keyState = SDL_GetKeyboardState(NULL);
    // if (keyState[SDL_SCANCODE_UP]) game.try_jump = true;
    if (keyState[SDL_SCANCODE_ESCAPE]) game.quit = true;
}

GameWindow::~GameWindow()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

