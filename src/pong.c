/*
    ISPPJ1 2023
    Study Case: Pong

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of the functions to init a pong game,
    update it, and render it.
*/

#include <stdio.h>

#include <allegro5/allegro_primitives.h>

#include "settings.h"
#include "fonts.h"
#include "pong.h"

void init_pong(struct Pong* pong, struct Sounds* sounds)
{
    init_paddle(&pong->player1, PADDLE_X_OFFSET, PADDLE_Y_OFFSET, PADDLE_WIDTH, PADDLE_HEIGHT);
    init_paddle(&pong->player2, TABLE_WIDTH - PADDLE_WIDTH - PADDLE_X_OFFSET, TABLE_HEIGHT - PADDLE_HEIGHT - PADDLE_Y_OFFSET, PADDLE_WIDTH, PADDLE_HEIGHT);
    init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);
    pong->state = START;
    pong->player1_score = 0;
    pong->player2_score = 0;
    pong->serving_player = 0;
    pong->winning_player = 0;
    pong->menu_selection = 0;
    pong->exit = 0;
    pong->sounds = sounds;
    srand(time(NULL));
}

void handle_player_movement(struct Paddle* player, ALLEGRO_KEYBOARD_STATE* state, const int down_key, const int up_key)
{
    if (al_key_down(state, down_key))
    {
        player->vy = PADDLE_SPEED;
    }
    else if (al_key_down(state, up_key))
    {
        player->vy = -PADDLE_SPEED;
    }
    else
    {
        player->vy = 0;
    }
}

void handle_cpu_movement(struct Pong *pong, struct Paddle *player)
{
    if (pong->ball.y > player->y)
    {
        player->vy = PADDLE_SPEED;
    }
    else if (pong->ball.y < player->y)
    {
        player->vy = -PADDLE_SPEED;
    }
    else
    {
        player->vy = 0;
    }
}

void start_behavior_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state)
{
    if (al_key_down(state, ALLEGRO_KEY_ENTER))
    {
        al_play_sample(pong->sounds->wall_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        pong->state = SERVE;
        switch (pong->menu_selection)
        {
        case 0:
            pong->game_mode = PLAYER_VS_PLAYER;
            break;
        case 1:
            pong->game_mode = PLAYER_VS_CPU;
            break;
        case 2:
            pong->game_mode = CPU_VS_PLAYER;
            break;
        case 3:
            pong->game_mode = CPU_VS_CPU;
            break;
        case 4:
            pong->exit = 1;
            return;
        default:
            // Not expected
            pong->state = START;
            break;
        }
        pong->menu_selection = 0;
        pong->serving_player = rand() % 2 + 1;
    }
    else if (al_key_down(state, ALLEGRO_KEY_UP) || al_key_down(state, ALLEGRO_KEY_W))
    {
        pong->menu_selection = (pong->menu_selection - 1 + MENU_OPTIONS) % MENU_OPTIONS;
        al_play_sample(pong->sounds->paddle_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (al_key_down(state, ALLEGRO_KEY_DOWN) || al_key_down(state, ALLEGRO_KEY_S))
    {
        pong->menu_selection = (pong->menu_selection + 1) % MENU_OPTIONS;
        al_play_sample(pong->sounds->paddle_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

void serve_behavior_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state)
{
    if (al_key_down(state, ALLEGRO_KEY_ENTER))
    {
        pong->state = PLAY;

        pong->ball.vx = rand() % 60 + 140;

        if (pong->serving_player == 2)
        {
            pong->ball.vx *= -1;
        }

        pong->ball.vy = rand() % 100 - 50;
    }
}

void play_behavior_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state)
{
    if (pong->game_mode == PLAYER_VS_PLAYER)
    {
        handle_player_movement(&pong->player1, state, ALLEGRO_KEY_S, ALLEGRO_KEY_W);
        handle_player_movement(&pong->player2, state, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_UP);
    }
    else if (pong->game_mode == PLAYER_VS_CPU)
    {
        handle_player_movement(&pong->player1, state, ALLEGRO_KEY_S, ALLEGRO_KEY_W);
    }
    else if (pong->game_mode == CPU_VS_PLAYER)
    {
        handle_player_movement(&pong->player2, state, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_UP);
    }
}

void done_behavior_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state)
{
    if (al_key_down(state, ALLEGRO_KEY_ENTER))
    {
        pong->state = SERVE;
        init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);

        pong->player1_score = 0;
        pong->player2_score = 0;

        if (pong->winning_player == 1)
        {
            pong->serving_player = 2;
        }
        else
        {
            pong->serving_player = 1;
        }
    }
}

void handle_input_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state)
{
    if (al_key_down(state, ALLEGRO_KEY_ESCAPE))
    {
        al_play_sample(pong->sounds->wall_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        init_pong(pong, pong->sounds);
    }

    switch (pong->state)
    {
    case START:
        start_behavior_pong(pong, state);
        break;
    case SERVE:
        serve_behavior_pong(pong, state);
        break;
    case PLAY:
        play_behavior_pong(pong, state);
        break;
    case DONE:
        done_behavior_pong(pong, state);
        break;
    default: // Not expected state value
        done_behavior_pong(pong, state);
    }
}

void update_pong(struct Pong* pong, double dt)
{
    if (pong->state == PLAY)
    {
        update_paddle(&pong->player1, dt);
        update_paddle(&pong->player2, dt);
        update_ball(&pong->ball, dt);

        struct Hitbox ball_hitbox;
        build_ball_hitbox(pong->ball, &ball_hitbox);

        struct Hitbox player1_hitbox;
        build_paddle_hitbox(pong->player1, &player1_hitbox);

        struct Hitbox player2_hitbox;
        build_paddle_hitbox(pong->player2, &player2_hitbox);

        if (ball_hitbox.x1 > TABLE_WIDTH)
        {
            al_play_sample(pong->sounds->score, /* gain */ 1.0, /* center */ 1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            ++pong->player1_score;
            pong->serving_player = 2;

            if (pong->player1_score == MAX_POINTS)
            {
                pong->winning_player = 1;
                pong->state = DONE;
            }
            else
            {
                pong->state = SERVE;
                init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);
            }
        }
        else if (ball_hitbox.x2 < 0)
        {
            al_play_sample(pong->sounds->score, /* gain */ 1.0, /* center */ -1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            ++pong->player2_score;
            pong->serving_player = 1;

            if (pong->player2_score == MAX_POINTS)
            {
                pong->winning_player = 2;
                pong->state = DONE;
            }
            else
            {
                pong->state = SERVE;
                init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);
            }
        }

        if (ball_hitbox.y1 <= 0)
        {
            al_play_sample(pong->sounds->wall_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.y = 0;
            pong->ball.vy *= -1;
        }
        else if (ball_hitbox.y2 >= TABLE_HEIGHT)
        {
            al_play_sample(pong->sounds->wall_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.y = TABLE_HEIGHT - pong->ball.height;
            pong->ball.vy *= -1;
        }
        
        if (collides(ball_hitbox, player1_hitbox))
        {
            al_play_sample(pong->sounds->paddle_hit, /* gain */ 1.0, /* center */ -1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.x = player1_hitbox.x2;
            pong->ball.vx *= -1.03;

            if (pong->ball.vy < 0)
            {
                pong->ball.vy = -(rand() % 140 + 10);
            }
            else
            {
                pong->ball.vy = rand() % 140 + 10;
            }
        }
        else if (collides(ball_hitbox, player2_hitbox))
        {
            al_play_sample(pong->sounds->paddle_hit, /* gain */ 1.0, /* center */ 1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.x = player2_hitbox.x1 - pong->ball.width;
            pong->ball.vx *= -1.03;

            if (pong->ball.vy < 0)
            {
                pong->ball.vy = -(rand() % 140 + 10);
            }
            else
            {
                pong->ball.vy = rand() % 140 + 10;
            }
        }
    }
}

void render_pong(struct Pong* pong, struct Fonts* fonts)
{
    if (pong->state != START)
    {
        al_draw_filled_rectangle(
            TABLE_WIDTH / 2 - MID_LINE_WIDTH / 2, 0,
            TABLE_WIDTH / 2 + MID_LINE_WIDTH / 2, TABLE_HEIGHT,
            al_map_rgb(255, 255, 255)
        );
        render_paddle(pong->player1);
        render_paddle(pong->player2);
        render_ball(pong->ball);

        char score[3];
        sprintf(score, "%d", pong->player1_score);
        al_draw_text(fonts->score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, score);
        sprintf(score, "%d", pong->player2_score);
        al_draw_text(fonts->score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, score);
    }

    if (pong->state == START)
    {
        ALLEGRO_FONT* f = fonts->large_font;
        ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR selection_color = al_map_rgb(255, 255, 0);
        ALLEGRO_COLOR* colors[5] = {&white, &white, &white, &white, &white};
        colors[pong->menu_selection] = &selection_color;
        al_draw_text(f, white, TABLE_WIDTH / 2, TABLE_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "PONG!");
        al_draw_text(f, *colors[0], TABLE_WIDTH / 2, TABLE_HEIGHT / 4 + TABLE_HEIGHT/12*2, ALLEGRO_ALIGN_CENTER, "Player VS Player");
        al_draw_text(f, *colors[1], TABLE_WIDTH / 2, TABLE_HEIGHT / 4 + TABLE_HEIGHT/12*3, ALLEGRO_ALIGN_CENTER, "Player VS CPU");
        al_draw_text(f, *colors[2], TABLE_WIDTH / 2, TABLE_HEIGHT / 4 + TABLE_HEIGHT/12*4, ALLEGRO_ALIGN_CENTER, "CPU VS Player");
        al_draw_text(f, *colors[3], TABLE_WIDTH / 2, TABLE_HEIGHT / 4 + TABLE_HEIGHT/12*5, ALLEGRO_ALIGN_CENTER, "CPU VS CPU");
        al_draw_text(f, *colors[4], TABLE_WIDTH / 2, TABLE_HEIGHT / 4 + TABLE_HEIGHT/12*6, ALLEGRO_ALIGN_CENTER, "Exit");
    }
    else if (pong->state == SERVE)
    {
        al_draw_text(fonts->large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press enter to serve");
    }
    else if (pong->state == DONE)
    {
        char winner_message[14];
        sprintf(winner_message, "Player %d won!", pong->winning_player);
        al_draw_text(fonts->large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 3, ALLEGRO_ALIGN_CENTER, winner_message);
        al_draw_text(fonts->large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press enter to restart");
    }
}
