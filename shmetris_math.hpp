
#pragma once
#include <cstdlib>

//TODO: replace rand
static inline Uint32 rng(Uint32 n) {
        return rand() % n;
}

static inline int coord_to_index(int x, int y) {
        return y * 10 + x;
}

static inline bool in_bounds(const SDL_Point p) {
        return p.x >= 0 && p.x < 10 && p.y < 19; 
}
