#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include <stdlib.h>
#include "menu.h"
#include "maps.h"



const float FPS = 50;
const int SCREEN_W = 700;
const int SCREEN_H = 640;
const int PACMAN_SIZE = 20;
const int NUMBER_OF_GHOSTS = 4;
const int WALL_SIZE = 20;
const int MAZE_SIZE_W = 19;
const int MAZE_SIZE_H = 21;
const int MAZE_START = (SCREEN_H - MAZE_SIZE_W * WALL_SIZE) / 2;

enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

void false_all_except(bool array[], int exception) {
    for (int i = 0; i < 4; i++) {
        array[i] = false;
    }
    if (exception != -1) {
        array[exception] = true;
    }
}


struct sprite {
    float x;
    float y;
    float speed;
    float start_x;
    float start_y;
    int mode;
    double mode_start;
    int hit;
};

#include "moves.h"


// sprawdza, czy Pacman nie wpada na sciane (1 - nie wpada, 0 - wpada)
int no_collision(int map[][MAZE_SIZE_W], sprite pacman, int direction) {
    float changed_y = pacman.y + pacman.speed * (direction <= 1) * pow((-1), direction + 1);
    float changed_x = pacman.x + pacman.speed * (direction > 1) * pow((-1), direction + 1);
    if (changed_x < MAZE_START || changed_x >= MAZE_START + WALL_SIZE * (MAZE_SIZE_W - 1)) {
        if (direction > 1){
            return 1;
        }
        else {
            return 0;
        }
    }
    int first, second;
    int values[4];
    values[0] = int(floor((changed_y - MAZE_START) / WALL_SIZE));
    values[1] = int(ceil((changed_y - MAZE_START) / WALL_SIZE));
    values[2] = int(floor((changed_x - MAZE_START) / WALL_SIZE));
    values[3] = int(ceil((changed_x - MAZE_START) / WALL_SIZE));
    if (direction <= 1) {
        first = map[values[direction]][values[2]];
        second = map[values[direction]][values[3]];
    }
    else {
        first = map[values[0]][values[direction]];
        second = map[values[1]][values[direction]];
    }
    if (first == 1 || first == 4 || second == 1 || second == 4) {
        return 0;
    }
    else {
        return 1;
    }
}

// sprawdza, czy Pacman zostaje zlapany przez duszka / lapie duszka
int ghost_collision(sprite ghosts[], int i, sprite pacman) {
    if (pacman.x == ghosts[i].x) {
        if ((pacman.y > ghosts[i].y && pacman.y + 5 < ghosts[i].y + PACMAN_SIZE) || (pacman.y + PACMAN_SIZE - 5 > ghosts[i].y && pacman.y < ghosts[i].y)) {
            return 1;
        }
    }
    else if (pacman.y == ghosts[i].y) {
        if ((pacman.x > ghosts[i].x && pacman.x + 5 < ghosts[i].x + PACMAN_SIZE) || (pacman.x + PACMAN_SIZE - 5 > ghosts[i].x && pacman.x < ghosts[i].x)) {
            return 1;
        }
    }
    return 0;
}

// przenosi Pacmana/duszka na druga strone labiryntu, gdy postac wejdzie do teleportu
void teleports(struct sprite* character) {
    if (character->x == MAZE_START - WALL_SIZE) {
        character->x = MAZE_START + WALL_SIZE * MAZE_SIZE_W - character->speed;
    }
    else if (character->x == MAZE_START + WALL_SIZE * MAZE_SIZE_W) {
        character->x = MAZE_START - WALL_SIZE + character->speed;
    }
}


int main(int argc, char **argv) {
    srand(time(NULL));
    sprite pacman;
    sprite ghosts[NUMBER_OF_GHOSTS];
    int MAP[MAZE_SIZE_H][MAZE_SIZE_W];
    // made, moving i killed do obslugi duszkow, ready do odliczania przed startem
    int made, moving, ready, killed = 0;
    pacman.x = SCREEN_W / 2.0 - PACMAN_SIZE / 2.0;
    pacman.y = SCREEN_H / 2.0 - PACMAN_SIZE / 2.0;
    pacman.speed = 2.5;
    int turn; // kierunek Pacmana
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *pacman_img = NULL;
    ALLEGRO_BITMAP *ghost_img[NUMBER_OF_GHOSTS + 1];
    ALLEGRO_BITMAP *wall = NULL;
    ALLEGRO_BITMAP *block = NULL;
    ALLEGRO_BITMAP *teleport = NULL;
    bool key[4];
    bool direction[4];
    int ghost_moves[NUMBER_OF_GHOSTS][2];
    bool redraw = true;
    bool doexit = false;
    bool playgame = false;
    bool no_coins = false;
    bool power_up = false;
    int score, lives, start, current;
    int modes[3] = {20, 7, 7};
    int power_up_modes[NUMBER_OF_GHOSTS];
    double power_up_times[NUMBER_OF_GHOSTS];
    double beginning;
    double power_up_time;
    char str_score[10];
    char str_ready[4];

    al_init();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    timer = al_create_timer(1.0 / FPS);
    display = al_create_display(SCREEN_W, SCREEN_H);
    pacman_img = al_load_bitmap("pacman.png");
    ghost_img[0] = al_load_bitmap("red.png");
    ghost_img[1] = al_load_bitmap("pink.png");
    ghost_img[2] = al_load_bitmap("blue.png");
    ghost_img[3] = al_load_bitmap("yellow.png");
    ghost_img[4] = al_load_bitmap("scared.png");
    ALLEGRO_FONT *font1 = al_load_ttf_font("ComputerAmok.ttf", 72, 0);
    ALLEGRO_FONT *font2 = al_load_ttf_font("COMPUTERRobot.ttf", 50, 0);
    ALLEGRO_FONT *font3 = al_load_ttf_font("COMPUTERRobot.ttf", 40, 0);


    wall = al_create_bitmap(WALL_SIZE, WALL_SIZE);
    al_set_target_bitmap(wall);
    al_clear_to_color(al_map_rgb(103,117,234));
    teleport = al_create_bitmap(WALL_SIZE, WALL_SIZE);
    al_set_target_bitmap(teleport);
    al_clear_to_color(al_map_rgb(7, 13, 64));
    block = al_create_bitmap(WALL_SIZE, WALL_SIZE);
    al_set_target_bitmap(block);
    al_clear_to_color(al_map_rgb(255, 237, 229));
    al_set_target_bitmap(al_get_backbuffer(display));

    event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while (!doexit) {
        menu(font1, font2, event_queue, &doexit, &playgame);
        score = 0;
        al_start_timer(timer);
        lives = 3;
        while(playgame) {
            random_map(MAP);
            turn = 0;
            moving = 0;
            made = 0;
            start = 0;
            ready = 4;
            false_all_except(direction, -1);
            false_all_except(key, -1);
            // ustawienie Pacmana i duszkow na pozycjach startowych
            for (int j = 0; j < MAZE_SIZE_H; j++) {
                for (int i = 0; i < MAZE_SIZE_W; i++) {
                    if (MAP[j][i] == 2) {
                        pacman.start_y = pacman.y = j * WALL_SIZE + MAZE_START;
                        pacman.start_x = pacman.x = i * WALL_SIZE + MAZE_START;
                        MAP[j][i] = 0;
                    }
                    if (MAP[j][i] == 4) {
                        ghosts[made].start_y = ghosts[made].y = j * WALL_SIZE + MAZE_START;
                        ghosts[made].start_x = ghosts[made].x = i * WALL_SIZE + MAZE_START;
                        ghosts[made].speed = 2;
                        ghosts[made].mode = 1;
                        ghosts[made].hit = 0;
                        ghost_moves[made][0] = 0;
                        ghost_moves[made][1] = 0;
                        made += 1;
                    }
                }
            }

            while(lives) {
                if (!ready) {
                    ALLEGRO_EVENT ev;
                    al_wait_for_event(event_queue, &ev);
                    if(ev.type == ALLEGRO_EVENT_TIMER) {
                        // nie wszystkie duszki ruszaja na raz, liczba ruszajacych duszkow sie zwieksza
                        if (moving < NUMBER_OF_GHOSTS) {
                            if (al_get_time() - beginning >= start) {
                                if (ghosts[moving].mode == 2) {
                                    power_up_times[moving] = al_get_time() - ghosts[moving].mode_start;
                                }
                                else {
                                    ghosts[moving].mode_start = al_get_time();
                                }
                                moving += 1;
                                start += 4;
                            }
                        }
                        // zmienianie stanow duszkow (krecacego sie, lapiacego Pacmana i uciekajacego)
                        for (int i = 0; i < NUMBER_OF_GHOSTS; i++) {
                            if (al_get_time() - ghosts[i].mode_start >= modes[ghosts[i].mode]) {
                                // zmienia stan z uciekajacego na poprzedni
                                if (ghosts[i].mode == 2 && !(ghosts[i].hit)) {
                                    // zapewnia ze duch nie wpadnie na sciane
                                    if (!(int(ghosts[i].x) % 2) && !(int(ghosts[i].y) % 2)) {
                                        ghosts[i].mode_start = al_get_time() - power_up_times[i];
                                        ghosts[i].mode = power_up_modes[i];
                                        ghosts[i].speed = 2;
                                    }
                                }
                                // zmienia stan - przelaczenie miedzy stanem krecacym sie a lapiacym
                                else if (i < moving && ghosts[i].mode != 2) {
                                    ghosts[i].mode_start = al_get_time();
                                    ghosts[i].mode = (ghosts[i].mode + 1) % 2;
                                }
                            }
                        }
                        for (int i = 0; i < 4; i++) {
                            if (key[i] && no_collision(MAP, pacman, i)) {
                                false_all_except(direction, i);
                            }
                        }
                        if (!(int(pacman.y - MAZE_START) % WALL_SIZE) && !(int(pacman.x - MAZE_START) % WALL_SIZE)) {
                            // Pacman zdobywa monete
                            if (MAP[int((pacman.y - MAZE_START) / 20)][int((pacman.x - MAZE_START) / 20)] == 5) {
                                MAP[int((pacman.y - MAZE_START) / WALL_SIZE)][int((pacman.x - MAZE_START) / WALL_SIZE)] = 0;
                                score += 10;
                            }
                            // Pacman zdobywa power-up i moze lapac duszki, duszki zaczynaja uciekac, ich stan jest zapisywany do odzysku
                            else if (MAP[int((pacman.y - MAZE_START) / 20)][int((pacman.x - MAZE_START) / 20)] == 7) {
                                MAP[int((pacman.y - MAZE_START) / WALL_SIZE)][int((pacman.x - MAZE_START) / WALL_SIZE)] = 0;
                                power_up_time = al_get_time();
                                score += 50;
                                killed = 0;
                                for (int i = 0; i < NUMBER_OF_GHOSTS; i++) {
                                    if (!(ghosts[i].hit)) {
                                        power_up_modes[i] = ghosts[i].mode % 2;
                                        ghosts[i].mode = 2;
                                        power_up_times[i] = al_get_time() - ghosts[i].mode_start;
                                        ghosts[i].mode_start = al_get_time();
                                        ghost_moves[i][0] *= (-1);
                                        ghost_moves[i][1] *= (-1);
                                        ghosts[i].speed = 1;
                                    }
                                }
                            }
                        }
                        // kolizja duszka z Pacmanem
                        for (int i = 0; i < NUMBER_OF_GHOSTS; i++) {
                            if (ghost_collision(ghosts, i, pacman)) {
                                // jesli duszek jest w trybie uciekania
                                if (ghosts[i].mode == 2) {
                                    // i nie zostal jeszcze zlapany, to dostajemy za niego punkty
                                    if (!(ghosts[i].hit)) {
                                        score += 200 * pow(2, killed);
                                        killed += 1;
                                        ghosts[i].hit = 1;

                                    }
                                }
                                // Pacman traci zycie i postacie startuja z pierwotnych pozycji
                                else {
                                    lives -= 1;
                                    pacman.x = pacman.start_x;
                                    pacman.y = pacman.start_y;
                                    turn = 0;
                                    moving = 0;
                                    start = 0;
                                    if (lives) {
                                        ready = 4;
                                    }
                                    false_all_except(direction, -1);
                                    false_all_except(key, -1);
                                    for (int j = 0; j < NUMBER_OF_GHOSTS; j++) {
                                        ghosts[j].x = ghosts[j].start_x;
                                        ghosts[j].y = ghosts[j].start_y;
                                        ghosts[j].mode = 1;
                                    }
                                }
                            }
                        }
                        // jesli duszek zostal zlapany wraca do miejsca startowego, jak tam wroci, wraca do poprzedniego stanu
                        for (int i = 0; i < NUMBER_OF_GHOSTS; i++) {
                            if (ghosts[i].hit && ghosts[i].x == ghosts[i].start_x && ghosts[i].y == ghosts[i].start_y) {
                                ghosts[i].hit = 0;
                                ghosts[i].mode_start = al_get_time() - power_up_times[i];
                                ghosts[i].mode = power_up_modes[i];
                                ghosts[i].speed = 2;
                            }
                        }

                        teleports(&pacman);
                        for (int i = 0; i < moving; i++) {
                            teleports(&ghosts[i]);
                        }
                        // ruch Pacmana
                        for (int i = 0; i < 4; i++) {
                            if (direction[i] and no_collision(MAP, pacman, i)) {
                                turn = ((i <= 1) + 2 * !(i % 2)); //0, 1, 2, 3 ->> 3, 1, 2, 0, potrzebne do dobrego obrotu obrazka Pacmana
                                pacman.x += pacman.speed * pow((-1), i + 1) * (i > 1);
                                pacman.y += pacman.speed * pow((-1), i + 1) * (i <= 1);
                            }
                            else {
                                direction[i] = false;
                            }
                        }
                        // ruch duszkow
                        for (int i = 0; i < moving; i++) {
                            if (!(int(ghosts[i].y - MAZE_START) % WALL_SIZE) && !(int(ghosts[i].x - MAZE_START) % WALL_SIZE)) {
                                if (ghosts[i].hit) {
                                    ghosts[i].speed = 5;
                                }
                                make_move(MAP, ghosts, pacman, i, ghost_moves[i], turn);
                            }
                            ghosts[i].y += ghosts[i].speed * ghost_moves[i][0];
                            ghosts[i].x += ghosts[i].speed * ghost_moves[i][1];
                        }
                        redraw = true;
                    }
                    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        playgame = false;
                        doexit = true;
                        break;
                    }
                    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                        switch(ev.keyboard.keycode) {
                            case ALLEGRO_KEY_UP:
                                false_all_except(key, KEY_UP);
                                break;
                            case ALLEGRO_KEY_DOWN:
                                false_all_except(key, KEY_DOWN);
                                break;
                            case ALLEGRO_KEY_LEFT:
                                false_all_except(key, KEY_LEFT);
                                break;
                            case ALLEGRO_KEY_RIGHT:
                                false_all_except(key, KEY_RIGHT);
                                break;
                        }
                    }
                    else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                        if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                            playgame = false;
                            break;
                        }
                    }
                }
                no_coins = false;
                power_up = false;
                if((redraw && al_is_event_queue_empty(event_queue)) || ready) {
                    redraw = false;
                    al_clear_to_color(al_map_rgb(7, 13, 64));
                    al_draw_text(font1, al_map_rgb(255, 255, 255), 640/2, MAZE_START / 4, ALLEGRO_ALIGN_CENTRE, "PACMAN");
                    al_draw_text(font2, al_map_rgb(255, 255, 255), MAZE_START + (MAZE_SIZE_W + 4) * WALL_SIZE, MAZE_START + WALL_SIZE, ALLEGRO_ALIGN_CENTRE, "SCORE");
                    sprintf(str_score, "%d", score);
                    al_draw_text(font2, al_map_rgb(255, 255, 255), MAZE_START + (MAZE_SIZE_W + 4) * WALL_SIZE, MAZE_START + 3 * WALL_SIZE, ALLEGRO_ALIGN_CENTRE, str_score);
                    al_draw_text(font2, al_map_rgb(255, 255, 255), MAZE_START + (MAZE_SIZE_W + 4) * WALL_SIZE, MAZE_START + 6 * WALL_SIZE, ALLEGRO_ALIGN_CENTRE, "LIVES");
                    for (int i = 0; i < lives; i++) {
                        al_draw_bitmap(pacman_img, MAZE_START + (MAZE_SIZE_W + 2 + i * 1.5) * WALL_SIZE, MAZE_START + 9 * WALL_SIZE, 0);
                    }
                    no_coins = true;
                    for (int i = 0; i < MAZE_SIZE_H; i++) {
                        for (int j = 0; j < MAZE_SIZE_W; j++) {
                            current = MAP[i][j];
                            if (current == 1) {
                                al_draw_bitmap(wall, MAZE_START + j * WALL_SIZE, MAZE_START + i * WALL_SIZE, 0);
                            }
                            else if (current == 4) {
                                al_draw_bitmap(block, MAZE_START + j * WALL_SIZE, MAZE_START + i * WALL_SIZE, 0);
                            }
                            // moneta
                            else if (current == 5) {
                                no_coins = false;
                                al_draw_filled_circle(MAZE_START + j * WALL_SIZE + WALL_SIZE / 2, MAZE_START + i * WALL_SIZE + WALL_SIZE / 2, WALL_SIZE / 7, al_map_rgb(255, 204, 0));
                            }
                            // power_up
                            else if (current == 7) {
                                no_coins = false;
                                al_draw_filled_circle(MAZE_START + j * WALL_SIZE + WALL_SIZE / 2, MAZE_START + i * WALL_SIZE + WALL_SIZE / 2, WALL_SIZE / 3, al_map_rgb(205, 16, 104));
                                al_draw_circle(MAZE_START + j * WALL_SIZE + WALL_SIZE / 2, MAZE_START + i * WALL_SIZE + WALL_SIZE / 2, WALL_SIZE / 3, al_map_rgb(255, 133, 200), 1);
                            }
                        }
                    }

                    al_draw_rotated_bitmap(pacman_img, PACMAN_SIZE / 2, PACMAN_SIZE / 2, pacman.x + PACMAN_SIZE / 2, pacman.y + PACMAN_SIZE / 2, ALLEGRO_PI / 2 * turn, 0);
                    for (int i = 0; i < NUMBER_OF_GHOSTS; i++) {
                        // przestraszony duch
                        if (ghosts[i].mode == 2) {
                            al_draw_bitmap(ghost_img[4], ghosts[i].x, ghosts[i].y, 0);
                            power_up = true;
                        }
                        // normalny duch
                        else {
                            al_draw_bitmap(ghost_img[i], ghosts[i].x, ghosts[i].y, 0);
                        }
                    }
                    // informacja o pozostalym czasie power-upa
                    if (power_up) {
                        al_draw_text(font3, al_map_rgb(255, 255, 255), MAZE_START + (MAZE_SIZE_W + 4.7) * WALL_SIZE, MAZE_START + 10.5 * WALL_SIZE, ALLEGRO_ALIGN_CENTRE, "POWER_UP");
                        sprintf(str_ready, "%d", 7 - int(al_get_time() - power_up_time));
                        al_draw_text(font2, al_map_rgb(255, 255, 255), MAZE_START + (MAZE_SIZE_W + 4) * WALL_SIZE, MAZE_START + 12 * WALL_SIZE, ALLEGRO_ALIGN_CENTRE, str_ready);

                    }
                    // teleporty po Pacmanie i duszkach, zeby je zakrywaly
                    for (int i = 0; i < MAZE_SIZE_H; i++) {
                        if (MAP[i][0] == 8) {
                            al_draw_bitmap(teleport, MAZE_START - WALL_SIZE, MAZE_START + i * WALL_SIZE, 0);
                            al_draw_bitmap(teleport, MAZE_START + WALL_SIZE * MAZE_SIZE_W, MAZE_START + i * WALL_SIZE, 0);
                        }
                    }

                    if (ready) {
                        ready -= 1;
                        if (ready) {
                            sprintf(str_ready, "%d", ready);
                        }
                        else {
                            strcpy(str_ready, "GO!");
                        }
                        al_draw_text(font2, al_map_rgb(255, 255, 255), MAZE_START + (MAZE_SIZE_W + 4) * WALL_SIZE, MAZE_START / 4 + 10, ALLEGRO_ALIGN_CENTRE, str_ready);
                        al_flip_display();
                        al_rest(1);
                        if (!ready) {
                            al_flush_event_queue(event_queue);
                            beginning = al_get_time();
                        }
                    }
                    else {
                        al_flip_display();
                    }
                }
                // koniec gry
                if (!lives) {
                    playgame = false;
                }
                // plansza wygrana
                if (no_coins) {
                    playgame = true;
                    break;
                }
            }
            // koniec gry, pokazanie najlepszych wynikow
            if (!playgame) {
                high_scores(font1, font2, event_queue, score);
            }
        }
    }
    al_destroy_bitmap(wall);
    al_destroy_bitmap(pacman_img);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
