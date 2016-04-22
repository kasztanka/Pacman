// moves.h odpowiada za poruszanie sie duszkow, kazdy ma wlasny sposob gonienia Pacmana
// gdy go nie gonia, kreca sie wokol ustalonego naroznika albo uciekaja

int find_neighbours(int MAP[][MAZE_SIZE_W], int neighbours[][2], int y, int x, int old_move[]);
void target_red(sprite pacman, int target[], sprite ghost);
void target_pink(int MAP[][MAZE_SIZE_W], sprite pacman, int target[], sprite ghost, int turn);
void target_blue(int MAP[][MAZE_SIZE_W], sprite pacman, int target[], sprite ghosts[], int turn);
void target_orange(int MAP[][MAZE_SIZE_W], sprite pacman, int target[], sprite ghost, int position_y, int position_x);

void make_move(int MAP[][MAZE_SIZE_W], sprite ghosts[], sprite pacman, int ghost_number, int old_move[], int turn) {
    old_move[0] = old_move[0] * (-1);
    old_move[1] = old_move[1] * (-1);
    int position_x = int((ghosts[ghost_number].x - MAZE_START) / 20);
    int position_y = int((ghosts[ghost_number].y - MAZE_START) / 20);
    int neighbours[3][2];
    // funkcja sprawdza, ile mozliwosci wyboru drog ma duszek
    int len_neighbours = find_neighbours(MAP, neighbours, position_y, position_x, old_move);
    switch (len_neighbours) {
        // duszki musza sie cofnac bo nie ma gdzie pojsc
        case 0:
            break;
        // duszki sie nie cofaja (chyba ze na poczatku uciekania) wiec idzie w inny mozliwy kierunek
        case 1:
            for (int i = 0; i < 2; i++) {
                if (old_move[0] != neighbours[i][0] || old_move[1] != neighbours[i][1]) {
                    old_move[0] = neighbours[i][0];
                    old_move[1] = neighbours[i][1];
                    break;
                }
            }
            break;
        // duszek ma 3 lub 4 mozliwe drogi
        default:
            int target[2];
            int min_distance = 1000;
            int min_index = 0;
            int distance;
            int new_position_y, new_position_x;
            // jesli ucieka, pojdzie w przypadkowym kierunku
            if (ghosts[ghost_number].mode == 2 && !(ghosts[ghost_number].hit)) {
                min_index = rand() % len_neighbours;
            }
            else {
                // jesli zostal trafiony wraca do swojego punktu startowego
                if (ghosts[ghost_number].mode == 2) {
                    target[0] = int((ghosts[ghost_number].start_y - MAZE_START) / 20);
                    target[1] = int((ghosts[ghost_number].start_x - MAZE_START) / 20);
                }
                else {
                    // kazdy duch ma inny cel
                    switch (ghost_number) {
                        case 0:
                            target_red(pacman, target, ghosts[ghost_number]);
                            break;
                        case 1:
                            target_pink(MAP, pacman, target, ghosts[ghost_number], turn);
                            break;
                        case 2:
                            target_blue(MAP, pacman, target, ghosts, turn);
                            break;
                        case 3:
                            target_orange(MAP, pacman, target, ghosts[ghost_number], position_y, position_x);
                            break;
                    }
                }
                // szukanie najmniejszej odleglosci od duszka do celu w zaleznosci od wybranej drogi
                for (int i = 0; i < len_neighbours; i++) {
                    new_position_y = position_y + neighbours[i][0];
                    new_position_x = position_x + neighbours[i][1];
                    distance = pow((new_position_y - target[0]), 2) + pow((new_position_x - target[1]), 2);
                    if (distance < min_distance) {
                        min_distance = distance;
                        min_index = i;
                    }
                }
            }
            old_move[0] = neighbours[min_index][0];
            old_move[1] = neighbours[min_index][1];
    }
}

int find_neighbours(int MAP[][MAZE_SIZE_W], int neighbours[][2], int y, int x, int old_move[]) {
    int len_neighbours = 0;
    for (int i = -1; i <= 1; i ++) {
        for (int j = -1; j <= 1; j ++) {
            if ((!i || !j) && (i || j) && (j != old_move[0] || i != old_move[1])) {
                // duszek nie wychodzi poza labirynt
                if (1 <= y + j && MAZE_H - 1> y + j && 0 <= x + i && MAZE_W > x + i) {
                    if (MAP[y + j][x + i] != 1) {
                        neighbours[len_neighbours][0] = j;
                        neighbours[len_neighbours][1] = i;
                        len_neighbours += 1;
                    }
                }
                // duszek moze skorzystac z teleportu
                if (x + i <= -1 || x + i >= MAZE_W) {
                    neighbours[len_neighbours][0] = j;
                    neighbours[len_neighbours][1] = i;
                    len_neighbours += 1;
                }
            }
        }
    }
    return len_neighbours;
}

// cel czerwonego to pozycja Pacmana
void target_red(sprite pacman, int target[], sprite ghost) {
    if (ghost.mode) {
        target[0] = 1;
        target[1] = 18;
    }
    else {
        target[0] = int((pacman.y - MAZE_START) / 20);
        target[1] = int((pacman.x - MAZE_START) / 20);
    }
}

// cel rozowego to 4 pola przed pozycja Pacmana
void target_pink(int MAP[][MAZE_SIZE_W], sprite pacman, int target[], sprite ghost, int turn) {
    if (ghost.mode) {
        target[0] = 1;
        target[1] = 1;
    }
    else {
        target[0] = int((pacman.y - MAZE_START) / 20) + 4 * !(turn % 2) * pow((-1), (turn > 1));
        target[1] = int((pacman.x - MAZE_START) / 20) + 4 * (turn % 2) * pow((-1), (turn > 1));
    }
}

// cel niebieskiego zalezy od pozycji Pacmana i pozycji czerwonego duszka
// niebieski probuje osaczyc Pacmana
void target_blue(int MAP[][MAZE_SIZE_W], sprite pacman, int target[], sprite ghosts[], int turn) {
    if (ghosts[2].mode) {
        target[0] = 20;
        target[1] = 1;
    }
    else {
        int pacman_y_2 = int((pacman.y - MAZE_START) / 20) + 2 * !(turn % 2) * pow((-1), (turn > 1));
        int pacman_x_2 = int((pacman.x - MAZE_START) / 20) + 2 * (turn % 2) * pow((-1), (turn > 1));
        int red_y = int((ghosts[0].y - MAZE_START) / 20);
        int red_x = int((ghosts[0].x - MAZE_START) / 20);
        target[0] = pacman_y_2 + (pacman_y_2 - red_y);
        target[1] = pacman_x_2 + (pacman_x_2 - red_x);
    }
}

// cel pomaranczowego zmienia sie w zaleznosci od jego odleglosci od Pacmana
// jesli jest dalej to goni Pacmana jak czerwony duszek
// jesli blizej to wraca do swojego naroznika
void target_orange(int MAP[][MAZE_SIZE_W], sprite pacman, int target[], sprite ghost, int position_y, int position_x) {
    if (ghost.mode) {
    target[0] = 20;
    target[1] = 18;
    }
    else {
        int pacman_y = int((pacman.y - MAZE_START) / 20);
        int pacman_x = int((pacman.x - MAZE_START) / 20);
        float distance = sqrt( pow((position_y - pacman_y), 2) + pow((position_x - pacman_x), 2));
        if (distance >= 6) {
            target[0] = int(pacman_y);
            target[1] = int(pacman_x);
        }
        else {
            target[0] = 20;
            target[1] = 18;
        }

    }
}
