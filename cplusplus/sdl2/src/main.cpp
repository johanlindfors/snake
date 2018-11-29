#include <iostream>
#include <SDL.h>

int main(int, char**){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
    for (int i = 0; i < 3; ++i){
        //First clear the renderer
        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear(ren);

        //Render red filled quad
        SDL_Rect fillRect = { 100, 100, 20, 20 };
        SDL_SetRenderDrawColor( ren, 0xFF, 0x00, 0x00, 0xFF );        
        SDL_RenderFillRect( ren, &fillRect );//Update the screen

        SDL_RenderPresent(ren);
        //Take a quick break after all that hard work
        SDL_Delay(1000);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}