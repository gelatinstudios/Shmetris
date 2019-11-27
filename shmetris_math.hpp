
#pragma once
#include <cstdlib>

//TODO: replace rand
static inline Uint32 rng(Uint32 n) {
        return rand() % n;
}

static inline int coord_to_index(int x, int y) {
        return y * 10 + x;
}
