#include "vector.h"
#include "game_state.h"
#include "sounds.h"
#include "bomber.h"
#include "bullet.h"
#include "power_up.h"

#ifndef _ENEMY_FIGHTER
#define _ENEMY_FIGHTER

typedef struct ENEMY_FIGHTER_DATA {
    VECTOR position;
    ENGINE* target;
    int health;
    int gun;
    int cooldown;
    bool dead;
    int side; // 0 means left, 1 means right
    float angle;
} ENEMY_FIGHTER_DATA;

#define MAX_ENEMY_FIGHTER_HEALTH 15
#define MAX_ENEMY_FIGHTER_SPEED  1
#define ENEMY_FIGHTER_THRESHOLD  1
#define ENEMY_FIGHTER_COOLDOWN   40
#define ENEMY_FIGHTER_LEAN       ALLEGRO_PI / 600
#define ENEMY_FIGHTER_MAX_ANGLE  ALLEGRO_PI / 12
#define ENEMY_FIGHTER_POINTS     100

#endif

extern VECTOR FIGHTER_SIZE;
extern VECTOR FIGHTER_COLLISION_POSITION;
extern VECTOR FIGHTER_COLLISION_SIZE;

void update_enemy_fighter(ENEMY_FIGHTER_DATA* fighter);
void kill_enemy_fighter(ENEMY_FIGHTER_DATA* fighter);