// menu.h odpowiada za wyglad menu i pomocy oraz obsluge najlepszych wynikow - zmiane pliku z nimi

const int OPT_NUMBER = 4;
void high_scores(ALLEGRO_FONT *font1, ALLEGRO_FONT *font2, ALLEGRO_EVENT_QUEUE *event_queue, int new_score);
void help(ALLEGRO_FONT *font2, ALLEGRO_EVENT_QUEUE *event_queue);

int menu(ALLEGRO_FONT *font1, ALLEGRO_FONT *font2, ALLEGRO_EVENT_QUEUE *event_queue, bool *doexit, bool *playgame) {
    char options[OPT_NUMBER][30] = {"START", "HELP", "HIGH SCORES", "EXIT"};
    char connect[30];
    int active = 0;
    bool exit = false;
    bool redraw_menu = true;
    ALLEGRO_EVENT ev;
    while (!exit) {
        if (redraw_menu) {
            al_clear_to_color(al_map_rgb(50,10,70));
            al_draw_text(font1, al_map_rgb(255,255,255), 700/2, (480/4), ALLEGRO_ALIGN_CENTRE, "PACMAN");
            for (int i = 0; i < OPT_NUMBER; i++) {
                if (active == i) {
                    strcpy(connect, "> ");
                    strcat(connect, options[i]);
                    strcat(connect, " <");
                    al_draw_text(font2, al_map_rgb(255,182,227), 700/2, (480/4 * (2 + (float)(i / 2.0))), ALLEGRO_ALIGN_CENTRE, connect);
                }
                else {
                    al_draw_text(font2, al_map_rgb(255,255,255), 700/2, (480/4 * (2 + (float)(i / 2.0))), ALLEGRO_ALIGN_CENTRE, options[i]);
                }
            }
            al_flip_display();
            redraw_menu = false;
        }
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    if (active > 0) {
                        redraw_menu = true;
                        active -= 1;

                    }
                    break;

                case ALLEGRO_KEY_DOWN:
                    if (active < OPT_NUMBER - 1) {
                        redraw_menu = true;
                        active += 1;
                    }
                    break;

                case ALLEGRO_KEY_ENTER:
                    switch(active) {
                        case 0:
                            exit = true;
                            *playgame = true;
                            break;
                        case 1:
                            help(font2, event_queue);
                            redraw_menu = true;
                            break;
                        case 2:
                            high_scores(font1, font2, event_queue, 0);
                            redraw_menu = true;
                            break;
                        case 3:
                            exit = true;
                            *doexit = true;
                            break;
                    }
                    break;

                case ALLEGRO_KEY_ESCAPE:
                    exit = true;
                    *doexit = true;
                    break;
                }
        }
    }
    return 0;
}

void high_scores(ALLEGRO_FONT *font1, ALLEGRO_FONT *font2, ALLEGRO_EVENT_QUEUE *event_queue, int new_score) {
    FILE *file = fopen("high_scores.txt", "r");
    int length = 7;
    char users[length + 1][15];
    int scores[length + 1];
    int users_len = 0;
    int word;
    char c;
    for (int i = 0; i < length; i++) {
        if (fgetc(file) != EOF) {
            c = fgetc(file);
            word = 0;
            while (c != '\n' && c != EOF) {
                users[i][word] = c;
                word += 1;
                c = fgetc(file);
            }
            users[i][word] = 0x00;
            fscanf(file, "%d", &scores[i]);
            users_len += 1;
        }
    }
    fclose(file);
    // po skonczonej grze sprawdzanie czy gracz dostal sie do najlepszych wynikow
    // i ewentualna zmiana pliku z nimi
    if (new_score) {
        int index = users_len;
        int swaps;
        scores[index] = new_score;
        for (int i = index - 1; i >= 0; i--) {
            if (scores[i] < scores[index]) {
                strcpy(users[index], users[i]);
                swaps = scores[index];
                scores[index] = scores[i];
                scores[i] = swaps;
                index--;
            }
            else {
                break;
            }
        }
        if (users_len < length) {
            users_len++;
        }
        // jesli gracz dostal sie na liste, moze podac swoje imie/nick
        if (index < length) {
            char user[15];
            int now = 0;
            ALLEGRO_EVENT ev2;
            bool redraw_menu = true;
            bool  exit = false;
            while (!exit) {
                user[now] = 0x00;
                if (redraw_menu) {
                    al_clear_to_color(al_map_rgb(50, 10, 70));
                    al_draw_text(font2, al_map_rgb(255, 255, 255), 700/2, 120, ALLEGRO_ALIGN_CENTRE, "ENTER YOUR NAME:");
                    al_draw_text(font2, al_map_rgb(255, 255, 255), 700/2, 180, ALLEGRO_ALIGN_CENTRE, user);
                    al_flip_display();
                    redraw_menu = false;
                }
                al_wait_for_event(event_queue, &ev2);
                if(ev2.type == ALLEGRO_EVENT_KEY_CHAR) {
                    int new_char = ev2.keyboard.unichar;
                    redraw_menu = true;
                    if (new_char >= 32 && now < 15) {
                        user[now] = new_char;
                        now++;
                    }
                    if (new_char == 8 && now != 0) {
                        now--;
                    }
                }
                else if (ev2.type == ALLEGRO_EVENT_KEY_UP) {
                    switch(ev2.keyboard.keycode) {
                        case ALLEGRO_KEY_ENTER:
                            exit = true;
                            break;
                    }
                }
            }
            strcpy(users[index], user);
            FILE *file = fopen("high_scores.txt", "w");
            for (int i = 0; i < users_len; i++) {
                fprintf(file, "\n%s\n%d", users[i], scores[i]);
            }
            fclose(file);
        }
    }

    char str_score[40];
    char numbers[length];
    al_clear_to_color(al_map_rgb(50, 10, 70));
    al_draw_text(font1, al_map_rgb(255, 255, 255), 700/2, 40, ALLEGRO_ALIGN_CENTRE, "HIGH SCORES");
    for (int i = 0; i < users_len; i++) {
        sprintf(numbers, "%d. ", i + 1);
        strcpy(str_score, numbers);
        strcat(str_score, users[i]);
        al_draw_text(font2, al_map_rgb(255, 255, 255), 640/6, 80 + 50 * (i + 1), ALLEGRO_ALIGN_LEFT, str_score);
        sprintf(numbers, "%d", scores[i]);
        strcpy(str_score, numbers);
        al_draw_text(font2, al_map_rgb(255, 255, 255), 600, 80 + 50 * (i + 1), ALLEGRO_ALIGN_RIGHT, str_score);
    }
    al_draw_text(font2, al_map_rgb(255, 182, 227), 700/2, 520, ALLEGRO_ALIGN_CENTRE, "> RETURN <");
    al_flip_display();
    ALLEGRO_EVENT ev;
    while (true) {
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                return;
            }
        }
    }
}

void help(ALLEGRO_FONT *font2, ALLEGRO_EVENT_QUEUE *event_queue) {
    ALLEGRO_BITMAP *background = al_load_bitmap("help.png");
    al_draw_bitmap(background, 0, 0, 0);
    al_draw_text(font2, al_map_rgb(255, 182, 227), 700/2, 530, ALLEGRO_ALIGN_CENTRE, "> RETURN <");
    al_flip_display();
    ALLEGRO_EVENT ev;
    while (true) {
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                return;
            }
        }
    }
}



