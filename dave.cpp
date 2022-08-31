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
        auto timer_begin = SDL_GetTicks();
        gameWindow.checkInput(game);
        updateGame(game, gameWindow);
        render(assets, game, gameWindow);
        auto timer_end = SDL_GetTicks();
        auto delay = 33 - (timer_end - timer_begin);
        // delay > 33 means that negative overflow happened
        delay = delay > 33 ? 0 : delay;
        SDL_Delay(delay);
    }
    return 0;
}

void render(const GameAssets &assets, const GameState &game, GameWindow &gameWindow)
{
    SDL_SetRenderDrawColor(gameWindow.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(gameWindow.renderer);

    drawWorld(assets, game, gameWindow);
    drawDave(assets, game, gameWindow);

    // swap the back buffer with the front buffer
    SDL_RenderPresent(gameWindow.renderer);
}

void drawWorld(const GameAssets &assets, const GameState &game, GameWindow &gameWindow)
{
    SDL_Rect dest;
	/* Draw each tile in row-major */
	for (int j=0; j < 10; j++)
	{
		dest.y = j * TILE_SIZE;
		dest.w = TILE_SIZE;
		dest.h = TILE_SIZE;
		for (int i=0; i < 20; i++)
		{
			dest.x = i * TILE_SIZE;
			int tile_index = game.levels[game.current_level].tiles[j*100+game.view_x+i];
			// tile_index = update_frame(game,tile_index,i);
			SDL_RenderCopy(gameWindow.renderer, assets.graphics_tiles[tile_index], NULL, &dest);
		}
	}
}

void drawDave(const GameAssets &assets, const GameState &game, GameWindow &gameWindow)
{
    SDL_Rect dest;
    dest.x = game.dave_px;
    dest.y = game.dave_py;
    dest.w = 20;
    dest.h = 16;

    SDL_RenderCopy(gameWindow.renderer, assets.graphics_tiles[getDaveState()], NULL, &dest);
}

inline int getDaveState()
{
    const auto STANDING_DAVE = 56;
    return STANDING_DAVE;
}

void updateGame(GameState &game, GameWindow &gameWindow)
{
    checkCollisions(game);
    verifyInput(game);
    moveDave(game);
    scrollScreen(game);
    applyGravity(game);
    clearInput(game);
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
    if (event.type == SDL_QUIT) game.quit = true;
    auto keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_UP]) game.try_jump = true;
    if (keyState[SDL_SCANCODE_LEFT]) game.try_left = true;
    if (keyState[SDL_SCANCODE_RIGHT]) game.try_right = true;
}

GameWindow::~GameWindow()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

GameState::GameState()
{
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

void scrollScreen(GameState &game)
{
    if (game.scroll_x > 0)
    {
        if (game.view_x == 80)
        {
            game.scroll_x = 0;
        }
        else
        {
            game.view_x++;
            game.scroll_x--;
        }
    }

    if (game.scroll_x < 0)
    {
        if (game.view_x == 0)
        {
            game.scroll_x = 0;
        }
        else
        {
            game.view_x--;
            game.scroll_x++;
        }
    }
}

void moveDave(GameState &game)
{
    if (game.dave_left)
    {
        game.dave_px -= 2;
        game.dave_left = false;
    }
    if (game.dave_right)
    {
        game.dave_px += 2;
        game.dave_right = false;
    }
    if (game.dave_jump)
    {
        if (game.jump_timer == 0) game.jump_timer = 20;        
        if (game.collisionPoints[0] && game.collisionPoints[1])
        {
            game.dave_py -= 2;
            game.jump_timer -= 1;
        }
        else
        {
            game.jump_timer = 0;
        }
        if (game.jump_timer == 0) game.dave_jump = false;
    }
}

void verifyInput(GameState &game)
{
    if (game.try_right && game.collisionPoints[2] && game.collisionPoints[3])
    {
        game.dave_right = true;
    }
    if (game.try_left && game.collisionPoints[6] && game.collisionPoints[7])
    {
        game.dave_left = true;
    }
    if (game.try_jump 
        && game.on_ground 
        && !game.dave_jump 
        && game.collisionPoints[0] 
        && game.collisionPoints[1]
    )
    {
        game.dave_jump = true;
    }
}

void clearInput(GameState &game)
{
    game.try_jump = false;
    game.try_left = false;
    game.try_right = false;
}

bool isClear(GameState &game, int px, int py, int whaat)
{
	unsigned char grid_x = px / TILE_SIZE;
	unsigned char grid_y = py / TILE_SIZE;

	auto type = game.levels[game.current_level].tiles[grid_y*100+grid_x];

    auto solidTiles = {1,3,5,15,16,17,18,19,21,22,23,24,29,30};
    if ( std::any_of(solidTiles.begin(),
                     solidTiles.end(), 
                     [&type](int i) { return i == type; }
    ))
    {
        return false;
    }

    return true;
}

void checkCollisions(GameState &game)
{
	game.collisionPoints[0] = isClear(game, game.dave_px+4,  game.dave_py-1,  1);
	game.collisionPoints[1] = isClear(game, game.dave_px+10, game.dave_py-1,  1);
	game.collisionPoints[2] = isClear(game, game.dave_px+11, game.dave_py+4,  1);
	game.collisionPoints[3] = isClear(game, game.dave_px+11, game.dave_py+12, 1);
	game.collisionPoints[4] = isClear(game, game.dave_px+10, game.dave_py+16, 1);
	game.collisionPoints[5] = isClear(game, game.dave_px+4,  game.dave_py+16, 1);
	game.collisionPoints[6] = isClear(game, game.dave_px+3,  game.dave_py+12, 1);
	game.collisionPoints[7] = isClear(game, game.dave_px+3,  game.dave_py+4,  1);

    game.on_ground = (!game.collisionPoints[4] || !game.collisionPoints[5]);
}

void applyGravity(GameState &game)
{
    if (!game.on_ground && game.jump_timer == 0)
    {
        game.dave_py += 2;
    }
}