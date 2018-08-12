#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

// Ported by @valt to SDL from https://www.youtube.com/user/FamTrinli doodle jump tutorial in SFML

enum {BACKGROUND = 0, DOODLE, PLATFORM};
const int platformsCnt = 10;
const int sizeX = 400, sizeY = 533;
const int FPS = 60;
const int ANIMATION_DELAY = 1000;
SDL_Rect boardRect = { 0, 0, sizeX, sizeY };
Uint32 start;

int main(int argc, char ** argv) {
	// Error checks
	std::cout << "SDL_Init\n";
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *win = SDL_CreateWindow("Doodle Jump", 100, 100, sizeX, sizeY, 0);
	if (win == nullptr) {
		std::cout << "SDL_CreateWindow error\n";
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cout << "SDL_CreateRenderer error\n";
	}
	// Load bitmaps
	std::vector<std::string> Bitmaps;
	Bitmaps.push_back("img/background.png");
	Bitmaps.push_back("img/doodle.png");
	Bitmaps.push_back("img/platform.png");
	// Create textures from bitmaps
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "SDL_image could not initialize! SDL_image Error:" << IMG_GetError() << std::endl;
	}
	std::vector<SDL_Texture *> Textures;
	for (auto bitmap : Bitmaps) {
		Textures.push_back(IMG_LoadTexture(renderer, bitmap.c_str()));
		if (Textures.back() == nullptr) {
			std::cout << bitmap.c_str() << " SDL_CreateTextureFromSurface error\n";
		}
		else
			std::cout << bitmap.c_str() << " SDL_CreateTextureFromSurface OK\n";
	}
	SDL_Point plat[platformsCnt];
	// generate platforms
	for (int i = 0; i<platformsCnt; i++)
	{
		plat[i].x = rand() % 400;
		plat[i].y = rand() % 533;
	}
	int x = 100, y = 100, h = 200;
	float dx = 0, dy = 0;

	while (1) {
		start = SDL_GetTicks();
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
			/* Look for a keypress */
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_LEFT:
					x -= 3;
					break;
				case SDLK_RIGHT:
					x += 3;
					break;
				case SDLK_UP:
					break;
				case SDLK_DOWN:
					break;
				default:
					std::cout << "Key not supported" << std::endl;
					break;
				}
			}
		}
		SDL_RenderClear(renderer);
		// board texture
		SDL_RenderCopy(renderer, Textures[BACKGROUND], nullptr, &boardRect);
		// platforms
		for (int i = 0; i < platformsCnt; i++) {
			SDL_Rect platRect = {plat[i].x, plat[i].y, 68, 16};
			SDL_RenderCopy(renderer, Textures[PLATFORM], nullptr, &platRect);
		}
		// doodle
		SDL_Rect doodleRect = { x, y, 80, 80 };
		SDL_RenderCopy(renderer, Textures[DOODLE], nullptr, &doodleRect);
		SDL_RenderPresent(renderer);
		dy += 0.2;
		y += dy;
		// jump from bottom
		if (y > 500)  
			dy = -10;
		if (y < h)
		for (int i = 0; i < platformsCnt; i++)	{
			y = h;
			plat[i].y = plat[i].y - dy;
			// generate new position for platform if out of screen
			if (plat[i].y > 533) { 
				plat[i].y = 0; 
				plat[i].x = rand() % 400; 
			}
		}
		// jump from the platform ?
		for (int i = 0; i < platformsCnt; i++)
			if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68)
				&& (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0))  
				dy = -10;
		// Animation delay
		if (ANIMATION_DELAY / FPS > SDL_GetTicks() - start)
			SDL_Delay(ANIMATION_DELAY / FPS - (SDL_GetTicks() - start));
	}
	IMG_Quit();
	for (auto texture : Textures)
		SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	std::cout << "SDL_Quit\n";
	return 0;
}