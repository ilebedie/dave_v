#ifndef DAVE_H
#define DAVE_H

#include <SDL.h>
/* Level format structure */
struct dave_level {
	uint8_t path[256];
	uint8_t tiles[1000];
	uint8_t padding[24];
};


#endif