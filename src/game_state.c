#include "game_state.h"

long frames;
long score;
bool debug;

void must_init(bool test, char* description) {
    if (test) return;
    printf("can't initalize %s!\n", description);
    exit(1);
}