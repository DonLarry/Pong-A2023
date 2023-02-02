/*
    ISPPJ1 2023
    Study Case: Pong

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of functions to create sounds and destroy them.
*/

#include <assert.h>

#include "settings.h"
#include "sounds.h"

void create_sounds(struct Sounds* sounds)
{
    sounds->background = al_load_sample(SOUNDS_FOLDER "/background.wav");
    sounds->paddle_hit = al_load_sample(SOUNDS_FOLDER "/paddle_hit.wav");
    sounds->wall_hit = al_load_sample(SOUNDS_FOLDER "/wall_hit.wav");
    sounds->score = al_load_sample(SOUNDS_FOLDER "/score.wav");
    sounds->menu_move = al_load_sample(SOUNDS_FOLDER "/menu_move.wav");
    sounds->menu_select = al_load_sample(SOUNDS_FOLDER "/menu_select.wav");
    sounds->esc_key_press = al_load_sample(SOUNDS_FOLDER "/esc_key_press.wav");
    al_reserve_samples(7);

    assert(sounds->background != NULL);
    assert(sounds->paddle_hit != NULL);
    assert(sounds->wall_hit != NULL);
    assert(sounds->score != NULL);
    assert(sounds->menu_move != NULL);
    assert(sounds->menu_select != NULL);
    assert(sounds->esc_key_press != NULL);
}

void destroy_sounds(struct Sounds* sounds)
{
    al_destroy_sample(sounds->background);
    al_destroy_sample(sounds->paddle_hit);
    al_destroy_sample(sounds->wall_hit);
    al_destroy_sample(sounds->score);
    al_destroy_sample(sounds->menu_move);
    al_destroy_sample(sounds->menu_select);
    al_destroy_sample(sounds->esc_key_press);
}
