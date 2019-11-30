
#pragma once

struct xorshift32_state {
        Uint32 a;
};

static inline Uint32 xorshift32(xorshift32_state *state)
{
//from https://en.wikipedia.org/wiki/Xorshift
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	Uint32 x = state->a;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return state->a = x;
}

static inline Uint32 rng(const Uint32 n, xorshift32_state *state) {
        return xorshift32(state) % n;
}

static inline int coord_to_index(int x, int y) {
        return y * 10 + x;
}

static inline bool in_bounds(const SDL_Point p) {
        return p.x >= 0 && p.x < 10 && p.y < 19;
}
