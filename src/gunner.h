#include "vector.h"
#include "game_state.h"
#include "mouse.h"
#include "bullet.h"
#include "sounds.h"
#include "power_up.h"
#include <math.h>

#ifndef _GUNNER
#define _GUNNER

typedef struct GUNNER {
    VECTOR position;
} GUNNER;

// the values below probably need some balancing.
// our testers will see.

#define GUNNER_COOLDOWN  5

#define DEFAULT_GUNNER_MAX_SHOTS 7
#define DEFAULT_GUNNER_RELOAD    80

extern int GUNNER_RELOAD;
extern int GUNNER_MAX_SHOTS;

extern int gunner_reload;
extern int gunner_cooldown;
extern int gunner_shots;

GUNNER create_gunner();
void update_gunner(GUNNER* gunner);
void update_gunner_data();

#endif