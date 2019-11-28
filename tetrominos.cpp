
#include <SDL2/SDL.h>
#include "shmetris_math.hpp"
#include "tetrominos.hpp"

const SDL_Point I[] = {{5, -1}, {3, -1}, {4, -1}, {6, -1}};
const SDL_Point O[] = {{4, -2}, {5, -2}, {4, -1}, {5, -1}};
const SDL_Point T[] = {{4, -1}, {3, -1}, {5, -1}, {4, -2}};
const SDL_Point J[] = {{4, -1}, {5, -1}, {3, -1}, {3, -2}};
const SDL_Point L[] = {{4, -1}, {5, -1}, {3, -1}, {5, -2}};
const SDL_Point S[] = {{4, -2}, {5, -2}, {3, -1}, {4, -1}};
const SDL_Point Z[] = {{4, -2}, {3, -2}, {4, -1}, {5, -1}};

const SDL_Point *shapes[] = {I, O, T, J, L, S, Z};

const Uint32 rgba_colors[] = {
        [TYPE_I] = 0xFFFFFF00, //light blue
        [TYPE_O] = 0xFF00FFFF, //yellow
        [TYPE_T] = 0xFFFF00A7, //purple
        [TYPE_J] = 0xFFFF0000, //blue
        [TYPE_L] = 0xFF0091FF, //orange
        [TYPE_S] = 0xFF00FF00, //green
        [TYPE_Z] = 0xFF0000FF //red
};

void Tetromino::make_new() {
        //TODO:  implement "bag" system
        type = rng(7);
        for (Uint8 i = 0; i < 4; ++i) {
                points[i].x = shapes[type][i].x;
                points[i].y = shapes[type][i].y;
        }
        orientation = 0;
}

void Tetromino::move_left(const Uint32 playfield[10*20]) {
        for (Uint8 i = 0; i < 4; ++i) {
                if (points[i].x - 1 < 0 || playfield[coord_to_index(points[i].x - 1, points[i].y)])
                        return;
        }
        for (Uint8 i = 0; i < 4; ++i) {
                --points[i].x;
        }
}


void Tetromino::move_right(const Uint32 playfield[10*20]) {
        for (Uint8 i = 0; i < 4; ++i) {
                if (points[i].x + 1 > 9 || playfield[coord_to_index(points[i].x + 1, points[i].y)])
                        return;
        }
        for (Uint8 i = 0; i < 4; ++i) {
                ++points[i].x;
        }
}

void Tetromino::rotate_left(const Uint32 playfield[10*20]) {
        if (type == TYPE_O) return;

        Tetromino result = *this;
        result.type = type;

        for (Uint8 i = 1; i < 4; ++i) {
                const int x = points[0].x - points[i].x;
                const int y = points[0].y - points[i].y;

                result.points[i].x = points[0].x - y;
                result.points[i].y = points[0].y + x;

                if(!in_bounds(result.points[i]) || playfield[coord_to_index(result.points[i].x, result.points[i].y)])
                        return;
        }
        result.orientation = (orientation - 1 + 4) % 4;

        *this = result;
}

void Tetromino::rotate_right(const Uint32 playfield[10*20]) {
        if (type == TYPE_O) return;

        Tetromino result = *this;
        result.type = type;

        for (Uint8 i = 1; i < 4; ++i) {
                const int x = points[0].x - points[i].x;
                const int y = points[0].y - points[i].y;

                result.points[i].x = points[0].x + y;
                result.points[i].y = points[0].y - x;

                if(!in_bounds(result.points[i]) || playfield[coord_to_index(result.points[i].x, result.points[i].y)])
                        return;
        }
        result.orientation = (orientation + 1) % 4;

        *this = result;
}

void Tetromino::blit_to_playfield(Uint32 playfield[10*20]) {
        for (Uint8 i = 0; i < 4; ++i) {
                const int n = coord_to_index(points[i].x, points[i].y);
                SDL_assert(n > 0);
                playfield[n] = rgba_colors[type];
        }
}
