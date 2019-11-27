
#include <cstdlib>
#include <ctime>

#include <SDL2/SDL.h>

#define SDL_ASSERT_LEVEL 2

#include "tetrominos.hpp"
#include "shmetris_math.hpp"

SDL_Window *win;
SDL_Renderer *rend;
Tetromino current_tetromino;
Uint32 playfield[10*20] = {0};

bool handle() {
        bool quit = 0;
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                        quit = 1;
                }

                //TODO: use scancodes instead
                if (event.type == SDL_KEYDOWN) {
                        const SDL_Keycode pressed = event.key.keysym.sym;
                        if (pressed == SDLK_z)
                                current_tetromino.rotate_left();
                        else if (pressed == SDLK_x)
                                current_tetromino.rotate_right();
                        else if (pressed == SDLK_LEFT)
                                current_tetromino.move_left();
                        else if (pressed == SDLK_RIGHT)
                                current_tetromino.move_right();
                }
        }
        return quit;
}

void update() {
        static Uint8 timer = 10;
        if (!timer) {
                for (Uint8 i = 0; i < 4; ++i) {
                        const bool b = playfield[coord_to_index(current_tetromino.points[i].x, current_tetromino.points[i].y + 1)];
                        if (current_tetromino.points[i].y + 1 >= 20 || b) {
                                current_tetromino.blit_to_playfield(playfield);
                                current_tetromino.make_new();
                                break;
                        }
                }
                for (Uint8 i = 0; i < 4; ++i)
                        ++current_tetromino.points[i].y;
                timer = 10;
        } else --timer;
}

void render() {
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        //playfield
        SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormatFrom(playfield, 10, 20, 32, 10*4, SDL_PIXELFORMAT_RGBA32);
        SDL_Texture *playfield_text = SDL_CreateTextureFromSurface(rend, surf);
        SDL_FreeSurface(surf);
        SDL_RenderCopy(rend, playfield_text, 0, 0);
        //TODO: this shall go away
        SDL_DestroyTexture(playfield_text);

        //tetromino
        for (Uint8 i = 0; i < 4; ++i) {
                SDL_SetRenderDrawColor(rend, rgba_colors[current_tetromino.type], rgba_colors[current_tetromino.type] >> 8, rgba_colors[current_tetromino.type] >> 16, 255);
                const SDL_Rect rect = {current_tetromino.points[i].x, current_tetromino.points[i].y, 1, 1};
                SDL_RenderFillRect(rend, &rect);
                //SDL_Log("point %u: x = %d y = %d\n", i, current_tetromino.points[i].x, current_tetromino.points[i].y);
        }

        SDL_RenderPresent(rend);
        //TODO: cap the framerate properly
        //SDL_Delay();
}

int main() {
        win = SDL_CreateWindow("Shmetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 200, 400, 0);
        rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
        SDL_RenderSetLogicalSize(rend, 10, 20);

        srand(time(0));

        current_tetromino.make_new();

        bool quit = 0;
        while(!quit) {
                quit = handle();
                update();
                render();
        }

        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
}
