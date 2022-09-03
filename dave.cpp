#include "dave.h"
#include <iostream>
#include <string>
#include <fstream>


int main(int argc, char* argv[])
{
    GameWindow gameWindow;
    GameAssets assets(gameWindow);
    GameState game;
    startLevel(game);
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
    drawDBullet(assets, game, gameWindow);

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
    dest.x = game.dave_px - game.view_x * TILE_SIZE;
    dest.y = game.dave_py;
    dest.w = 20;
    dest.h = 16;

    SDL_RenderCopy(gameWindow.renderer, assets.graphics_tiles[getDaveState(game)], NULL, &dest);
}

int getDaveState(const GameState &game)
{
    const char STANDING_FRONT_DAVE = 56;
    char tile_index = STANDING_FRONT_DAVE;
    if (game.dave_jetpack)
		tile_index = game.direction >= 0 ? 77 : 80;
	else
	{
		if (game.dave_jump || !game.on_ground)
			tile_index = game.direction >= 0 ? 67 : 68;

	}
    return tile_index;
}

void updateGame(GameState &game, GameWindow &gameWindow)
{
    checkCollisions(game);
    verifyInput(game);
    moveDave(game);
    updateDBullet(game);
    pickUpItem(game);
    scrollScreen(game);
    applyGravity(game);
    updateLevel(game);
    clearInput(game);
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
    if (keyState[SDL_SCANCODE_UP]) game.try_jump = true;
    if (keyState[SDL_SCANCODE_LEFT]) game.try_left = true;
    if (keyState[SDL_SCANCODE_RIGHT]) game.try_right = true;
    if (keyState[SDL_SCANCODE_LCTRL]) game.try_fire = true;
    if (keyState[SDL_SCANCODE_LALT]) game.try_jetpack = true;
    if (keyState[SDL_SCANCODE_ESCAPE]) game.quit = true;
    if (keyState[SDL_SCANCODE_A]) game.try_up = true;
    if (keyState[SDL_SCANCODE_Z]) game.try_down = true;
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
    	/* Scroll right if Dave reaches view position 18 */
	if (game.dave_x - game.view_x >= 18)
		game.scroll_x = 15;

	/* Scroll left if Dave reaches view position 2 */
	if (game.dave_x - game.view_x < 2)
		game.scroll_x = -15;
    
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
    game.dave_x = game.dave_px / TILE_SIZE;
	game.dave_y = game.dave_py / TILE_SIZE;
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
    if (game.dave_down)
    {
        game.dave_py += 2;
        game.dave_down = false;
    }
    if (game.dave_up)
    {
        game.dave_py -= 2;
        game.dave_up = false;
    }
    	/* Jetpack usage cancels jump effects */
	if (game.dave_jetpack)
	{
		game.dave_jump = 0;
		game.jump_timer = 0;
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

	/* Fire Dave's gun */
	if (game.dave_fire)
	{
		game.dbullet_direction = game.direction;

		/* Bullet should match Dave's direction */
		if (!game.dbullet_direction)
			game.dbullet_direction = 1;

		/* Bullet should start in front of Dave */
		if (game.dbullet_direction == 1)
			game.dbullet_px = game.dave_px + TILE_SIZE + 2;

		if (game.dbullet_direction == -1)
			game.dbullet_px = game.dave_px - 8;

		game.dbullet_py = game.dave_py + 8;
		game.dave_fire = false;
	}
}

void verifyInput(GameState &game)
{
    if (game.try_left)
    {
        game.direction = -1;
    }

    if (game.try_right)
    {
        game.direction = 1;
    }

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

    if (game.try_fire && game.gun && !game.dbullet_px && !game.dbullet_py)
    {
        game.dave_fire = true;
    }

    if (game.try_jetpack && game.jetpack)
        game.dave_jetpack = !game.dave_jetpack;
    
    if (game.try_down && game.dave_jetpack && game.collisionPoints[4] && game.collisionPoints[5])
    {
        game.dave_down = true;
    }

    if (game.try_up && game.dave_jetpack && game.collisionPoints[0] && game.collisionPoints[1])
    {
        game.dave_up = true;
    }
}

void clearInput(GameState &game)
{
    game.try_jump = false;
    game.try_left = false;
    game.try_right = false;
    game.try_fire = false;
    game.try_jetpack = false;
    game.try_down = false;
    game.try_up = false;
}

bool isClear(GameState &game, int px, int py, bool is_dave)
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

    auto pickupTiles = {4,10,20,47,48,49,50,51,52};
    if ( std::any_of(pickupTiles.begin(),
                     pickupTiles.end(), 
                     [&type](int i) { return i == type; }
    ))
    {
        if (is_dave)
        {
            game.pickup_x = grid_x;
            game.pickup_y = grid_y;
        }
    }

    if (type == 2 && is_dave)
    {
        game.check_door = true;
    }
    return true;
}

void checkCollisions(GameState &game)
{
	game.collisionPoints[0] = isClear(game, game.dave_px+4,  game.dave_py-1,  true);
	game.collisionPoints[1] = isClear(game, game.dave_px+10, game.dave_py-1,  true);
	game.collisionPoints[2] = isClear(game, game.dave_px+11, game.dave_py+4,  true);
	game.collisionPoints[3] = isClear(game, game.dave_px+11, game.dave_py+12, true);
	game.collisionPoints[4] = isClear(game, game.dave_px+10, game.dave_py+16, true);
	game.collisionPoints[5] = isClear(game, game.dave_px+4,  game.dave_py+16, true);
	game.collisionPoints[6] = isClear(game, game.dave_px+3,  game.dave_py+12, true);
	game.collisionPoints[7] = isClear(game, game.dave_px+3,  game.dave_py+4,  true);

    game.on_ground = (!game.collisionPoints[4] || !game.collisionPoints[5]);
}

void applyGravity(GameState &game)
{
    if (!game.on_ground && game.jump_timer == 0 && !game.dave_jetpack)
    {
        game.dave_py += 2;
    }
}

inline void add_score(GameState &game, unsigned int new_score)
{
	if (game.score / 20000 != ((game.score+new_score) / 20000))
		game.lives++;

	game.score += new_score;
}

void pickUpItem(GameState &game)
{
    if (!game.pickup_x || !game.pickup_y) return;

    auto type = game.levels[game.current_level].tiles[game.pickup_y*100+game.pickup_x];

    switch (type)
    {
        case 4:
            game.jetpack = 0xFF;
            break;
        case 10:
            add_score(game,1000);
            game.trophy = true;
            break;
        case 20:
            game.gun = true;
            break;
		case 47: add_score(game,100); break;
		case 48: add_score(game,50); break;
		case 49: add_score(game,150); break;
		case 50: add_score(game,300); break;
		case 51: add_score(game,200); break;
		case 52: add_score(game,500); break;
        default:
            break;
    }

    game.levels[game.current_level].tiles[game.pickup_y*100+game.pickup_x] = 0;
    game.pickup_x = 0;
    game.pickup_y = 0;
}

void updateLevel(GameState &game)
{
    if (game.check_door)
    {
        if (game.trophy)
        {
            if (game.current_level < 8)
            {
                game.current_level++;
                startLevel(game);
            }
            else
            {
                std::cout << "You win with " << game.score <<" points!"<< std::endl;
                game.quit = true;
            }
        }
        game.check_door = false;
    }
}

void startLevel(GameState &game)
{
	switch (game.current_level)
	{
		case 0: game.dave_x = 2; game.dave_y = 8; break;
		case 1: game.dave_x = 1; game.dave_y = 8; break;
		case 2: game.dave_x = 2; game.dave_y = 5; break;
		case 3: game.dave_x = 1; game.dave_y = 5; break;
		case 4: game.dave_x = 2; game.dave_y = 8; break;
		case 5: game.dave_x = 2; game.dave_y = 8; break;
		case 6: game.dave_x = 1; game.dave_y = 2; break;
		case 7: game.dave_x = 2; game.dave_y = 8; break;
		case 8: game.dave_x = 6; game.dave_y = 1; break;
		case 9: game.dave_x = 2; game.dave_y = 8; break;
	}

	game.dave_px = game.dave_x * TILE_SIZE;
	game.dave_py = game.dave_y * TILE_SIZE;
    game.trophy = false;
    game.gun = false;
    game.jetpack = 0;
}

void updateDBullet(GameState &game)
{
    uint8_t i, grid_x, grid_y, mx, my;

	grid_x = game.dbullet_px / TILE_SIZE;
	grid_y = game.dbullet_py / TILE_SIZE;

	/* Not active */
	if (!game.dbullet_px || !game.dbullet_py)
		return;

	/* Bullet hit something - deactivate */
	if (!isClear(game, game.dbullet_px, game.dbullet_py, false))
		game.dbullet_px = game.dbullet_py = 0;

	/* Bullet left room - deactivate */
	if (grid_x-game.view_x < 1 || grid_x - game.view_x > 20)
		game.dbullet_px = game.dbullet_py = 0;

	if (game.dbullet_px)
	{
		game.dbullet_px += game.dbullet_direction * 4;
	}
}

void drawDBullet(const GameAssets &assets, const GameState &game, GameWindow &gameWindow)
{
    SDL_Rect dest;
	uint8_t tile_index;

	if (!game.dbullet_px || !game.dbullet_py)
		return;

	dest.x = game.dbullet_px - game.view_x * TILE_SIZE;
	dest.y = game.dbullet_py;
	dest.w = 12;
	dest.h = 3;
	tile_index = game.dbullet_direction > 0 ? 127 : 128;

	SDL_RenderCopy(gameWindow.renderer, assets.graphics_tiles[tile_index], NULL, &dest);
}

