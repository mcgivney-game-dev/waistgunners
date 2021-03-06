#include "enemy.h"
#include "graphics.h"

ENEMY enemies[MAX_ENEMIES];

VECTOR get_enemy_position(ENEMY* enemy) {
    VECTOR v = { .x = 3000, .y = 3000 };
    // if you see these values in a print statement somewhere, something is wrong!

    switch (enemy->type) {
        case ENEMY_FIGHTER:
            // access the enemy fighter
            // no need for a break statement
            return enemy->data.fighter.position;
        case ENEMY_IMPOSTER:
            return enemy->data.imposter.position;
        case ENEMY_JET:
            return enemy->data.jet.position;
        case ENEMY_MISSILE:
            return enemy->data.missile.position;
        default:
            return v;
    }
}

void add_enemy(ENEMY_DATA data, ENEMY_TYPE type) {
    for (int c = 0; c < MAX_ENEMIES; c++) {
        ENEMY* e = &enemies[c];
        if (e->used) continue;
        e->data = data;
        e->type = type;
        e->used = true;
        return;
    }
}

void add_enemy_fighter() {
    float x = between(0, BUFFER_WIDTH);
    float y = 0; // for now

    VECTOR position                 = { .x = x, .y = -FIGHTER_SIZE.y };
    ENEMY_FIGHTER_DATA fighter_data = {
        .position = position,
        .target   = NULL,
        .health   = MAX_ENEMY_FIGHTER_HEALTH,
        .gun      = 0,
        .cooldown = 0,
        .dead     = false,
        .side     = 0,
        .angle    = 0,
    };

    ENEMY_DATA data = { .fighter = fighter_data };

    add_enemy(data, ENEMY_FIGHTER);
}

int imposter_countdown = DEFAULT_IMPOSTER_SPAWN_DELAY; //1000;
int enemy_imposters    = 0; // only one imposter can be on the screen at a time

ENEMY_IMPOSTER_ENGINE imposter_engines[ENGINES_PER_IMPOSTER];
ENEMY_IMPOSTER_GUNNER imposter_gunners[GUNNERS_PER_IMPOSTER];

void add_enemy_imposter() {
    if (enemy_imposters) return;

    enemy_imposters++;

    VECTOR imposter_position = { .x = formation.x, .y = BUFFER_HEIGHT };

    for (int c = 0; c < ENGINES_PER_IMPOSTER; c++) {
        VECTOR engine_position       = add(imposter_position, IMPOSTER_ENGINES[c]);
        ENEMY_IMPOSTER_ENGINE engine = {
            .position = engine_position,
            .health   = IMPOSTER_ENGINE_MAX_HEALTH,
            .dead     = false,
        };
        imposter_engines[c] = engine;
    }
    for (int c = 0; c < GUNNERS_PER_IMPOSTER; c++) {
        VECTOR gunner_position       = add(imposter_position, IMPOSTER_GUNNERS[c]);
        ENEMY_IMPOSTER_GUNNER gunner = {
            .position = gunner_position,
            .target   = NULL,
            .cooldown = 0,
            .reload   = 0,
            .shots    = IMPOSTER_GUNNER_MAX_SHOTS,
        };
        imposter_gunners[c] = gunner;
    }

    ENEMY_IMPOSTER_DATA imposter_data = {
        .position = imposter_position,
        .engines  = { &imposter_engines[0], &imposter_engines[1] },
        .gunners  = { &imposter_gunners[0], &imposter_gunners[1] },
        .target   = NULL,
        .down     = false,
    };
    ENEMY_DATA data = { .imposter = imposter_data };

    add_enemy(data, ENEMY_IMPOSTER);
}

void reset_imposter_countdown() {
    int seconds = (int) between(15, 50);
    imposter_countdown = (int) (seconds * FRAME_RATE);
}

void add_enemy_jet() {
    ENEMY_JET_DATA jet = create_enemy_jet();
    ENEMY_DATA data    = { .jet = jet };
    add_enemy(data, ENEMY_JET);
}

void add_enemy_missile(VECTOR pos) {
    ENEMY_MISSILE_DATA missile = create_missile(pos);
    ENEMY_DATA data            = { .missile = missile };
    add_enemy(data, ENEMY_MISSILE);
}

void update_enemies() {
    // also handles spawning of enemies, as well
    for (int c = 0; c < MAX_ENEMIES; c++) {
        ENEMY* e = &enemies[c];
        if (!e->used) continue;
        switch (e->type) {
            case ENEMY_FIGHTER:
                ;
                ENEMY_FIGHTER_DATA* fighter = &(e->data.fighter);
                update_enemy_fighter(fighter);
                if (fighter->dead && (
                    fighter->position.x < -FIGHTER_SIZE.x ||
                    fighter->position.x > BUFFER_WIDTH
                )) {
                    e->used = false;
                }
                break;
            case ENEMY_IMPOSTER:
                ;
                ENEMY_IMPOSTER_DATA* imposter = &(e->data.imposter);
                update_enemy_imposter(imposter);
                // check for a downed imposter, then set enemy_imposters as 0
                if (imposter->down && (imposter->position.y > BUFFER_HEIGHT)) {
                    enemy_imposters = 0;
                    e->used         = false;
                    reset_imposter_countdown();
                }
                break;
            case ENEMY_JET:
                ;
                ENEMY_JET_DATA* jet = &(e->data.jet);
                update_enemy_jet(jet);
                if (jet->direction == DOWN && !jet->fired && jet->position.y > BUFFER_HEIGHT / 8) {
                    jet->fired = true;
                    add_enemy_missile(jet->position);
                }
                if (jet->health <= 0 && (jet->position.y >= BUFFER_HEIGHT || jet->position.y <= -JET_SIZE.y)) {
                    e->used = false;
                }
                break;
            case ENEMY_MISSILE:
                ;
                ENEMY_MISSILE_DATA* missile = &(e->data.missile);
                update_enemy_missile(missile);
                if (missile->health <= 0) {
                    e->used = false;
                }
                break;
        }
    }

    if (game_state != PLAYING) return;

    // for generating enemies
    // add more as needed

    if (frames % 90 == 0 && rand() % 2 == 0) {
        add_enemy_fighter();
    }

    if (!(enemy_imposters || imposter_countdown)) {
        add_enemy_imposter();
    }

    if (score > 1000 && !(frames % 900) && rand() % 3) {
        add_enemy_jet();
    }

    if (!enemy_imposters) {
        imposter_countdown--;
    }
}

// KILL KILL KILL KILL KILL KILL KILL KILL KILL KILL
void kill_all_enemies() {
    for (int c = 0; c < MAX_ENEMIES; c++) {
        ENEMY* enemy = &enemies[c];
        if (!enemy->used) continue;
        switch (enemy->type) {
            case ENEMY_FIGHTER:
                ;
                ENEMY_FIGHTER_DATA* fighter = &(enemy->data.fighter);
                kill_enemy_fighter(fighter);
                break;
            case ENEMY_IMPOSTER:
                ;
                ENEMY_IMPOSTER_DATA* imposter = &(enemy->data.imposter);
                kill_enemy_imposter(imposter);
                break;
            case ENEMY_JET:
                ;
                ENEMY_JET_DATA* jet = &(enemy->data.jet);
                kill_enemy_jet(jet);
                break;
            case ENEMY_MISSILE:
                ;
                ENEMY_MISSILE_DATA* missile = &(enemy->data.missile);
                kill_enemy_missile(missile);
                break;
        }
    }
}