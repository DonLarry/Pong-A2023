/*
    ISPPJ1 2023
    Study Case: Pong

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of a struct to load sounds and the
    declaratrion of functions to create them and destroy them.
*/

#pragma once

#include <allegro5/allegro_audio.h>

struct Sounds
{
    ALLEGRO_SAMPLE* background;
    ALLEGRO_SAMPLE* paddle_hit;
    ALLEGRO_SAMPLE* wall_hit;
    ALLEGRO_SAMPLE* score;
    ALLEGRO_SAMPLE* menu_move;
    ALLEGRO_SAMPLE* menu_select;
    ALLEGRO_SAMPLE* esc_key_press;
};

void create_sounds(struct Sounds* sounds);

void destroy_sounds(struct Sounds* sounds);
