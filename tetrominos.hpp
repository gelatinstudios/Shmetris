
#pragma once

enum {TYPE_I, TYPE_O, TYPE_T, TYPE_J, TYPE_L, TYPE_S, TYPE_Z};

extern const Uint32 rgba_colors[];

struct Tetromino {
        SDL_Point points[4];
        Uint8 type;

        void make_new();
        void move_left();
        void move_right();
        void rotate_left();
        void rotate_right();
        void blit_to_playfield(Uint32 [10*20]);
};
