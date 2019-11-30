

#include <SDL2/SDL.h>

#define SDL_ASSERT_LEVEL 2

#include "shmetris_math.hpp"
#include "tetrominos.hpp"


#ifdef SH_DEBUG
Uint64 sdl_ticks_xxx;

#define D_START (sdl_ticks_xxx = SDL_GetPerformanceCounter())
#define D_PRINT(X) (SDL_Log("%s: ticks: %u\n", (X), SDL_GetPerformanceCounter() - sdl_ticks_xxx))

#else

#define D_START
#define D_PRINT(X)

#endif

struct GameData {
        Tetromino tetromino;
        Uint32 playfield[10*20];
        Uint8 timer;
        Bag bag;
};

bool handle(GameData &data) {
        bool quit = 0;
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                        quit = 1;
                } else if (event.type == SDL_KEYDOWN) {
                        const SDL_Keycode pressed = event.key.keysym.sym;
                        switch (pressed) {
                                case SDLK_z:
                                        data.tetromino.rotate_left(data.playfield);
                                        break;
                                case SDLK_x:
                                        data.tetromino.rotate_right(data.playfield);
                                        break;
                                case SDLK_LEFT:
                                        data.tetromino.move_left(data.playfield);
                                        break;
                                case SDLK_RIGHT:
                                        data.tetromino.move_right(data.playfield);
                                        break;
                                case SDLK_DOWN:
                                        data.timer = 0;
                                        break;
                        }
                }
        }

        // const Uint8 *state = SDL_GetKeyboardState(0);
        // if (state[SDL_SCANCODE_DOWN]) timer = 0;

        return quit;
}

void update(GameData &data) {
        //move down
        if (!data.timer) {
                for (Uint8 i = 0; i < 4; ++i) {
                        if(data.tetromino.points[i].y < 0) continue;
                        const bool b = data.playfield[coord_to_index(data.tetromino.points[i].x, data.tetromino.points[i].y + 1)];
                        if (data.tetromino.points[i].y + 1 >= 20 || b) {
                                data.tetromino.blit_to_playfield(data.playfield);
                                data.tetromino.make_new(data.bag);
                                break;
                        }
                }
                for (Uint8 i = 0; i < 4; ++i)
                        ++data.tetromino.points[i].y;
                data.timer = 10;
        } else --data.timer;

        //line deletion
        for (Uint8 i = 0; i < 20; ++i) {
                bool b = 1;
                for (Uint8 j = 0; j < 10; ++j) {
                        if (!data.playfield[i * 10 + j]) {
                                b = 0;
                                break;
                        }
                }

                if (!b) continue;
                for (Uint8 j = i * 10 - 1; j < i * 10; --j) {
                        data.playfield[j + 10] = data.playfield[j];
                }
        }
}

void render(SDL_Renderer *rend, SDL_Texture *playfield_text, GameData &data) {
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        //despite what the SDL documentation states, updating seems faster than locking
        //playfield
        SDL_UpdateTexture(playfield_text, 0, (const void *) data.playfield, 4*10);
        SDL_RenderCopy(rend, playfield_text, 0, 0);
        //tetromino
        for (Uint8 i = 0; i < 4; ++i) {
                SDL_SetRenderDrawColor(rend, rgba_colors[data.tetromino.type], rgba_colors[data.tetromino.type] >> 8, rgba_colors[data.tetromino.type] >> 16, 255);
                const SDL_Rect rect = {data.tetromino.points[i].x, data.tetromino.points[i].y, 1, 1};
                SDL_RenderFillRect(rend, &rect);
                //SDL_Log("point %u: x = %d y = %d\n", i, current_tetromino.points[i].x, current_tetromino.points[i].y);
        }

        SDL_RenderPresent(rend);
        //TODO: cap the framerate properly
        //SDL_Delay();
}

int main() {
        SDL_Init(SDL_INIT_VIDEO);

        SDL_Window *win = SDL_CreateWindow("Shmetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 200, 400, 0);
        SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
        SDL_RenderSetLogicalSize(rend, 10, 20);

        GameData data = {};
        for (Uint8 i = 0; i < 7; ++i)
                data.bag.types[i] = i;
        data.bag.rng_state.a = SDL_GetPerformanceCounter();
        data.bag.shuffle();
        data.tetromino.make_new(data.bag);

        SDL_Texture *playfield_text = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, 10, 20);

        data.timer = 10;

        bool quit = 0;
        while(!quit) {
                quit = handle(data);
                update(data);
                render(rend, playfield_text, data);
        }

        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
}
