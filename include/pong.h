/*
    ISPPJ1 2023
    Study Case: Pong

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of a pong game and the declaration
    of the functions to init it, update it, and render it.
*/

#include "paddle.h"
#include "ball.h"
#include "sounds.h"

enum PongState
{
    START,
    SERVE,
    PLAY,
    DONE
};

struct Pong
{
    struct Paddle player1;
    struct Paddle player2;
    struct Ball ball;

    enum PongState state;

    int player1_score;
    int player2_score;
    int serving_player;
    int winning_player;

    int menu_selection;

    struct Sounds* sounds;
};

void init_pong(struct Pong* pong, struct Sounds* sounds);

void handle_player_movement(struct Paddle* player, ALLEGRO_KEYBOARD_STATE* state, const int down_key, const int up_key);

void start_behavior_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state);
void serve_behavior_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state);
void play_behavior_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state);
void done_behavior_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state);

void handle_input_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state);

void update_pong(struct Pong* pong, double dt);

void render_pong(struct Pong* pong, struct Fonts* fonts);
