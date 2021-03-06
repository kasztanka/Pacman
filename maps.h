// maps.h odpowiada za przechowywanie mapek do gry i ich wybor

const int NUMBER_OF_MAPS = 4;
const int MAZE_H = 21;
const int MAZE_W = 19;
int maps[NUMBER_OF_MAPS][MAZE_H][MAZE_W] = {
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 1},
        {1, 7, 1, 1, 1, 5, 1, 1, 5, 1, 5, 1, 1, 5, 1, 1, 1, 7, 1},
        {1, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 1},
        {1, 1, 1, 5, 1, 1, 5, 1, 5, 1, 5, 1, 5, 1, 1, 5, 1, 1, 1},
        {1, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 5, 1},
        {1, 5, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 5, 1},
        {1, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 1},
        {1, 1, 5, 1, 5, 1, 5, 1, 4, 1, 1, 1, 5, 1, 5, 1, 5, 1, 1},
        {8, 5, 5, 5, 5, 1, 5, 1, 4, 4, 4, 1, 5, 1, 5, 5, 5, 5, 8},
        {1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1},
        {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
        {1, 5, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 5, 1},
        {1, 5, 1, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 1, 5, 1},
        {1, 5, 1, 5, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 5, 1, 5, 1},
        {1, 5, 5, 5, 5, 5, 5, 5, 5, 2, 5, 5, 5, 5, 5, 5, 5, 5, 1},
        {1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1},
        {1, 5, 5, 5, 5, 5, 1, 5, 5, 1, 5, 5, 1, 5, 5, 5, 5, 5, 1},
        {1, 7, 1, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 1, 7, 1},
        {1, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    },
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 1},
        {1, 7, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 7, 1},
        {1, 5, 5, 1, 5, 5, 5, 1, 5, 1, 5, 1, 5, 5, 5, 1, 5, 5, 1},
        {1, 1, 5, 1, 5, 1, 5, 1, 5, 5, 5, 1, 5, 1, 5, 1, 5, 1, 1},
        {0, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 0},
        {0, 1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1, 0},
        {0, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 0},
        {1, 1, 5, 1, 5, 1, 5, 1, 1, 1, 4, 1, 5, 1, 5, 1, 5, 1, 1},
        {8, 5, 5, 1, 5, 1, 5, 1, 4, 4, 4, 1, 5, 1, 5, 1, 5, 5, 8},
        {1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1},
        {8, 5, 5, 1, 5, 1, 5, 5, 5, 5, 5, 5, 5, 1, 5, 1, 5, 5, 8},
        {1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1},
        {0, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 0},
        {1, 1, 5, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 5, 1, 1},
        {1, 5, 5, 1, 5, 5, 5, 5, 5, 2, 5, 5, 5, 5, 5, 1, 5, 5, 1},
        {1, 5, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 5, 1},
        {1, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 5, 1},
        {1, 7, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 7, 1},
        {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    },
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
		{1, 7, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 7, 1},
		{1, 5, 5, 5, 5, 5, 5, 1, 5, 1, 5, 1, 5, 5, 5, 5, 5, 5, 1},
		{1, 1, 5, 1, 1, 1, 5, 1, 5, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1},
		{8, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 8},
		{1, 1, 5, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 5, 1, 1},
		{1, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 1},
		{1, 5, 1, 1, 1, 1, 5, 1, 4, 1, 1, 1, 5, 1, 1, 1, 1, 5, 1},
		{1, 5, 5, 5, 5, 5, 5, 1, 4, 4, 4, 1, 5, 5, 5, 5, 5, 5, 1},
		{1, 5, 1, 5, 1, 1, 5, 1, 1, 1, 1, 1, 5, 1, 1, 5, 1, 5, 1},
		{1, 5, 1, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 1, 5, 1},
		{1, 5, 1, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 1, 5, 1},
		{1, 5, 1, 5, 5, 5, 5, 1, 5, 1, 5, 1, 5, 5, 5, 5, 1, 5, 1},
		{1, 5, 1, 5, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 5, 1, 5, 1},
		{1, 5, 5, 5, 5, 5, 5, 5, 5, 2, 5, 5, 5, 5, 5, 5, 5, 5, 1},
		{1, 7, 1, 5, 1, 1, 5, 1, 1, 1, 1, 1, 5, 1, 1, 5, 1, 7, 1},
		{1, 5, 1, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 1, 5, 1},
		{1, 5, 1, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 1, 5, 1},
		{1, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    },
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
		{1, 5, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 5, 1},
		{1, 7, 1, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 1, 7, 1},
		{1, 5, 1, 5, 1, 1, 5, 1, 5, 1, 5, 1, 5, 1, 1, 5, 1, 5, 1},
		{8, 5, 5, 5, 5, 1, 5, 1, 5, 5, 5, 1, 5, 1, 5, 5, 5, 5, 8},
		{1, 5, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 5, 1},
		{1, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 1},
		{1, 1, 5, 1, 5, 1, 5, 1, 4, 1, 1, 1, 5, 1, 5, 1, 5, 1, 1},
		{0, 1, 5, 5, 5, 1, 5, 1, 4, 4, 4, 1, 5, 1, 5, 5, 5, 1, 0},
		{0, 1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1, 0},
		{0, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 0},
		{1, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 1},
		{8, 5, 5, 1, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 1, 5, 5, 8},
		{1, 1, 5, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 5, 1, 1},
		{1, 5, 5, 5, 5, 1, 5, 5, 5, 2, 5, 5, 5, 1, 5, 5, 5, 5, 1},
		{1, 7, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 7, 1},
		{1, 5, 1, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 1, 5, 1},
		{1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1},
		{1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    }/*,
    {
    },
    {
    },
    {
    },
    {
    },
    {
    }*/
};
void random_map(int map[][MAZE_W]) {
    // mapa jest wybierana losowo
    int chosen = rand() % NUMBER_OF_MAPS;
    for (int i = 0; i < MAZE_W; i++) {
        for (int j = 0; j < MAZE_H; j++) {
            map[j][i] = maps[chosen][j][i];
        }
    }
}
