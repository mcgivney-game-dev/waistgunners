#include "bomber.h"
#include "graphics.h"
VECTOR formation = { .x = BUFFER_WIDTH / 2, .y = BUFFER_HEIGHT / 2 };

BOMBER bombers[MAX_BOMBERS + 1];
ENGINE engines[(MAX_BOMBERS + 1) * ENGINES_PER_BOMBER];

// also add an array for the gunners afterward

VECTOR BOMBER_SIZE            = { .x = 64, .y = 59 };
const VECTOR BOMBER_ENGINES[] = {
    { .x = 15, .y = 10 }, { .x = 41, .y = 10 }
};

const VECTOR BOMBER_GUNNERS[] = {
    { .x = 23, .y = 35 }, { .x = 39, .y = 35 }
};

void reset_bombers() {
    formation.x = BUFFER_WIDTH / 2, formation.y = BUFFER_HEIGHT / 2;
    // printf("formation: (%f, %f)\n", formation.x, formation.y);
    for (int c = 0; c < MAX_BOMBERS; c++) {
        // set the bomber in the formation
        float x = formation.x, y = formation.y;
        switch (c) {
            case 0:
                break;
            case 1:
                x -= 40;
                y += 40;
                break;
            case 2:
                x -= 80;
                y += 80;
                break;
            case 3:
                x += 40;
                y += 40;
                break;
        }
        VECTOR bomber_position = { .x = x, .y = y };

        // set the bomber's engines
        for (int d = 0; d < ENGINES_PER_BOMBER; d++) {
            ENGINE e = create_engine();
            e.position.x = x + BOMBER_ENGINES[d].x;
            e.position.y = y + BOMBER_ENGINES[d].y;
            engines[d + ENGINES_PER_BOMBER * c] = e;
        }

        // set the bomber's gunners

        BOMBER b = {
            .position = bomber_position,
            .engines = { // what the fuck?
                &engines[0 + ENGINES_PER_BOMBER * c],
                &engines[1 + ENGINES_PER_BOMBER * c]
            },
            .down = false };
        bombers[c] = b;
    }
}

void move_bombers(VECTOR motion) {
    if (frames % 2) return;
    for (int c = 0; c < MAX_BOMBERS; c++) {
        BOMBER* b = &bombers[c];
        if (b->down) continue;
        VECTOR future_position = add(b->position, motion);
        if (
            future_position.x < -BOMBER_MARGIN ||
            future_position.y < -BOMBER_MARGIN ||
            future_position.x + BOMBER_SIZE.x > BUFFER_WIDTH + BOMBER_MARGIN ||
            future_position.y + BOMBER_SIZE.y > BUFFER_HEIGHT + BOMBER_MARGIN
        ) {
            // cancel the movement
            return;
        }
    }

    for (int c = 0; c < MAX_BOMBERS; c++) {
        BOMBER* b = &bombers[c];
        if (b->down) continue;
        b->position = add(motion, b->position);
        for (int d = 0; d < ENGINES_PER_BOMBER; d++) {
            b->engines[d]->position = add(b->engines[d]->position, motion);
        }
        // activate this when gunners are added
        // for (int d = 0; d < GUNNERS_PER_BOMBER; d++) {
        //     b->gunners[d]->position = add(b->gunners[d]->position, motion);
        // }
    }
}

void update_bombers() {
    VECTOR motion = { .x = 0, .y = 0 };
    if (key_is_pressed(ALLEGRO_KEY_LEFT) || key_is_pressed(ALLEGRO_KEY_A)) {
        motion.x += -BOMBER_SPEED;
    }
    if (key_is_pressed(ALLEGRO_KEY_RIGHT) || key_is_pressed(ALLEGRO_KEY_D)) {
        motion.x += BOMBER_SPEED;
    }
    move_bombers(motion);
    motion.x = 0;
    // move x and y separately, to avoid "sticky" walls
    if (key_is_pressed(ALLEGRO_KEY_DOWN) || key_is_pressed(ALLEGRO_KEY_S)) {
        motion.y += BOMBER_SPEED;
    }
    if (key_is_pressed(ALLEGRO_KEY_UP) || key_is_pressed(ALLEGRO_KEY_W)) {
        motion.y += -BOMBER_SPEED;
    }
    move_bombers(motion);

    // check for downed bombers and dead engines
    for (int c = 0; c < MAX_BOMBERS; c++) {
        BOMBER* b = &bombers[c];
        if (b->down) {

        } else {

        }
    }
}
