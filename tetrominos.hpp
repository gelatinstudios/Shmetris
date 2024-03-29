
#pragma once

enum {TYPE_I, TYPE_O, TYPE_T, TYPE_J, TYPE_L, TYPE_S, TYPE_Z};

extern const Uint32 rgba_colors[];

struct Bag {
        Uint8 types[7];
        Uint8 index;
        xorshift32_state rng_state;

        void shuffle();
};

struct Tetromino {
        SDL_Point points[4];
        Uint8 type;
        Uint8 orientation;

        void make_new(Bag &);
        void move_left(const Uint32 playfield[10*20]);
        void move_right(const Uint32 playfield[10*20]);
        void rotate_left(const Uint32 playfield[10*20]);
        void rotate_right(const Uint32 playfield[10*20]);
        void blit_to_playfield(Uint32 [10*20]);
};
