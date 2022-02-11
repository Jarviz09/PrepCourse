#include <iostream>
#include <string>

#include "map.hpp"

Map::Map(std::istream &is) {
    is >> x;
    is >> y;
    map.resize(x);
    for (int i = 0; i < x; ++i) {
        map[i].resize(y);
        for (int j = 0; j < y; ++j) {
            map[i][j] = '.';
        }
    }
    int i = 0;
    while (is) {
        int X = -1;
        int Y = -1;
        i++;
        std::string Mob = "\0";
        is >> X;
        is >> Y;
        is >> Mob;

        if (X != -1 && Y != -1 && Mob[0]) {
            map[X][Y] = Mob[0];
        }
    }
}

int Map::get_x() {
    return x;
}

int Map::get_y() {
    return y;
}

char Map::get_elem(int x, int y) {
    return map[x][y];
}

void Map::set_elem(int x, int y, char c) {
    map[x][y] = c;
}



