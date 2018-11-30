#include <iostream>
#include <memory>
#include <SDL.h>
#include <string>
#include <list>
#include "cleanup.h"
#include "timer.hpp"

const int SPRITE_SIZE = 20;
const int SCREEN_SIZE = 20;
const int FRAMES_PER_SECOND = 15;
const int INITIAL_TAIL = 5;

const int SCREEN_WIDTH  = SPRITE_SIZE * SCREEN_SIZE;
const int SCREEN_HEIGHT = SPRITE_SIZE * SCREEN_SIZE;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

class Apple {
private:
    int x;
    int y;

public:
    void draw(SDL_Renderer* renderer) {
        //Render green filled quad
        SDL_Rect fillRect = { 30, 30, 20, 20 };
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF );
        SDL_RenderFillRect(renderer, &fillRect );//Update the screen
    }
};

struct Point {
public:
    int X;
    int Y;

    Point(int x, int y) 
        : X(x)
        , Y(y)
        {

        }
};

class Snake {
private:
    int x;
    int y;
    int dx;
    int dy;
    int tail;
    std::list<Point> trail;

public:

    Snake() {
        trail = std::list<Point>();
        tail = INITIAL_TAIL;
        x = 10;
        y = 10;
        dy = 1;
    }

    void update() {
        x += dx;
        y += dy;

        if(y >= SCREEN_SIZE) y = 0;

        trail.push_back(Point(x,y));
        while((int)trail.size() > tail) {
            trail.pop_front();
        }
    }

    void draw(SDL_Renderer* renderer) {
        //Render green filled quad
        SDL_Rect fillRect = { x * SPRITE_SIZE +1, y * SPRITE_SIZE + 1, SPRITE_SIZE - 1, SPRITE_SIZE - 1 };
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF );        
        SDL_RenderFillRect(renderer, &fillRect );//Update the screen
    }
};

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Snake* snake;
    Apple* apple;

public:
    Game() {
        snake = new Snake();
        apple = new Apple();        
    }

    ~Game() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        if(apple) delete(apple);
        if(snake) delete(snake);
    }
    
    bool init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
            logSDLError(std::cout, "SDL_Init");
            return false;
        }

        window = SDL_CreateWindow("Snake", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            logSDLError(std::cout, "CreateWindow");
            //SDL_Quit();
            return false;
        }
        renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr){
            logSDLError(std::cout, "CreateRenderer");
            cleanup(window);
            return false;
        }
        return true;
    }

    bool handleInput() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				return false;
			}
			if (e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return false;
                        break;
                    case SDLK_LEFT:
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_DOWN:
                        break;
                    case SDLK_UP:
                        break;
                    default:
                        break;
                }
			}
		}
        return true;
    }

    void update() {
        snake->update();
    }

    void draw() {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear(renderer);

        snake->draw(renderer);
        apple->draw(renderer);

        SDL_RenderPresent(renderer);
    }

    void run() {
        Timer fps;
        bool running = true;
        while (running) {
            //Start the frame timer
            fps.start();
            
            running = handleInput();
            update();
            draw();
    
            // we want to cap the frame rate
            if(fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
                //Sleep the remaining frame time
                SDL_Delay( ( 1000 / FRAMES_PER_SECOND) - fps.get_ticks() );
            }
        }
    }
};

int main(int, char**){
    Game game;
    if(!game.init()) {
        SDL_Quit();
        return 1;
    }

    game.run();

    SDL_Quit();
	return 0;
}