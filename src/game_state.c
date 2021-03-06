#include "game_state.h"

long frames;
long score;
bool debug;
bool paused = false;
bool night  = false;

int game_state = MAIN_MENU;

void must_init(bool test, char* description) {
    if (test) return;
    printf("can't initialize %s!\n", description);
    exit(1);
}

float between(float lower, float higher) {
    return lower + ((float) rand() / (float) RAND_MAX) * (higher - lower);
}

int sign(float x) {
    if (x < 0) return -1;
    if (x > 0) return 1;
    return 0;
}