
#include <cstdlib>

#include <SDL2/SDL.h>

#define SDL_ASSERT_LEVEL 2

#include "tetrominos.hpp"
#include "shmetris_math.hpp"


#ifdef SH_DEBUG
Uint64 sdl_ticks_xxx;

#define D_START (sdl_ticks_xxx = SDL_GetPerformanceCounter())
#define D_PRINT(X) (SDL_Log("%s: ticks: %u\n", (X), SDL_GetPerformanceCounter() - sdl_ticks_xxx))

#else

#define D_START
#define D_PRINT(X)

#endif

SDL_Window *win;
SDL_Renderer *rend;
Tetromino current_tetromino;
SDL_Texture *playfield_text;
Uint32 playfield[10*20] = {};
Uint8 timer;

bool handle() {
        bool quit = 0;
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                        quit = 1;
                }

                if (event.type == SDL_KEYDOWN) {
                        const SDL_Keycode pressed = event.key.keysym.sym;
                        if (pressed == SDLK_z)
                                current_tetromino.rotate_left(playfield);
                        else if (pressed == SDLK_x)
                                current_tetromino.rotate_right(playfield);
                        else if (pressed == SDLK_LEFT)
                                current_tetromino.move_left(playfield);
                        else if (pressed == SDLK_RIGHT)
                                current_tetromino.move_right(playfield);
                }
        }

        const Uint8 *state = SDL_GetKeyboardState(0);
        if (state[SDL_SCANCODE_DOWN]) timer = 0;

        return quit;
}

void update() {
        //move down
        if (!timer) {
                for (Uint8 i = 0; i < 4; ++i) {
                        if(current_tetromino.points[i].y < 0) continue;
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

        //line deletion
        for (Uint8 i = 0; i < 20; ++i) {
                bool b = 1;
                for (Uint8 j = 0; j < 10; ++j) {
                        if (!playfield[i * 10 + j]) {
                                b = 0;
                                break;
                        }
                }

                if (!b) continue;
                for (Uint8 j = i * 10 - 1; j < i * 10; --j) {
                        playfield[j + 10] = playfield[j];
                }
        }
}

void render() {
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        //despite what the SDL documentation states, updating seems faster than locking
        //playfield
        SDL_UpdateTexture(playfield_text, 0, (const void *) playfield, 4*10);
        SDL_RenderCopy(rend, playfield_text, 0, 0);
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

        srand(SDL_GetPerformanceCounter());

        current_tetromino.make_new();
        playfield_text = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, 10, 20);

        timer = 10;

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
