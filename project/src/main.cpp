#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>

#include "map.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "main.hpp"

void get_step(Map& map, Player& player) {
    int x = player.get_posX();
    int y = player.get_posY();

    if (map.get_elem(x, y) == '.') {
        std::cout << "Supported actions:\n";
        if (x > 0) {
            std::cout << " * move left" << std::endl;
        }

        if (x < map.get_x() - 1) {
            std::cout << " * move right" << std::endl;
        }

        if (y > 0) {
            std::cout << " * move down" << std::endl;
        }

        if (y < map.get_y() - 1) {
            std::cout << " * move up" << std::endl;
        }

        if (x == 0 && y == 0 && map.get_x() == 1 && map.get_y() == 1) {
            std::cout << "\n";
        }

        std::cout << x << " x " << y << ", hp: " << player.get_hp() << " > ";

        return;
    }

    switch (map.get_elem(x, y)) {
        case 'w':
            std::cout << "\nwolf found, 6 hp" << std::endl;
            break;
        case 'd':
            std::cout << "\ndog found, 3 hp" << std::endl;
            break;
        case 'r':
            std::cout << "\nrat found, 2 hp" << std::endl;
    }

    fighting(player);

    return;
}

int next_step(Map& map, Player& player) {
    std::string str;
    while (true) {
        if (!(std::getline(std::cin, str))) {
            return (-1);
        }

        if (str == (std::string) "move left") {
            if (player.get_posX() == 0) {
                continue;
            }

            player.set_posX(-1);
            return 0;
        }

        if (str == (std::string) "move right") {
            if (player.get_posX() == map.get_x() - 1) {
                continue;
            }

            player.set_posX(1);
            return 0;
        }

        if (str == (std::string) "move down") {
            if (player.get_posY() == 0) {
                continue;
            }

            player.set_posY(-1);
            return 0;
        }

        if (str == (std::string) "move up") {
            if (player.get_posY() == map.get_y() - 1) {
                continue;
            }
            player.set_posY(1);
            return 0;
        }
    }
}

int kick_enemy() {
    std::string str;
    while (true) {
        if (!(std::getline(std::cin, str))) {
            return (-1);
        }

        if (str == (std::string)"kick enemy") {
            return 0;
        }
    }
}

int fight(Player& player, Enemy& monster) {
    while (true) {
        if (kick_enemy() == (-1)) {
            return (-1);
        }

        monster.hit(player.get_damage());
        if (monster.get_hp() <= 0) {
            std::cout << "\nenemy killed\n";
            return 0;
        }

        player.hit(monster.get_damage());
        if (player.get_hp() <= 0) {
            std::cout << "\nplayer died\n";
            return 1;
        }

        std::cout << "\nenemy kicked. Enemy hp: " << monster.get_hp() << std::endl;
        fighting(player);
    }
}

void fighting(Player& player) {
    std::cout << "Supported actions:" << std::endl;
    std::cout << " * kick enemy" << std::endl;
    std::cout << player.get_posX() << " x " << player.get_posY() << ", hp: " << player.get_hp() << " > ";
}

int main(int argc, const char** argv) {
    if (argc < 3) {
        return -1;
    }

    std::ifstream map_stream(argv[2]);
    if (!map_stream) {
        return -1;
    }

    Map map(map_stream);
    Player player;
    while (std::cin) {
        get_step(map, player);
        int x = player.get_posX();
        int y = player.get_posY();
        if (map.get_elem(x, y) == '.') {
            if (next_step(map, player) == -1) {
                break;
            }
            if (map.get_elem(player.get_posX(), player.get_posY()) == '.') {
                std::cout << "\nmoved\n";
            }
            continue;
        }

        Enemy* monster;
        switch (map.get_elem(x, y)) {
            case 'w': {
                monster = new Wolf();
                break;
            }
            case 'd': {
                monster = new Dog();
                break;
            }
            case 'r': {
                monster = new Rat();
                break;
            }
        }

        if (fight(player, *monster) != 0) {
            delete monster;
            break;
        }
        map.set_elem(x, y, '.');
        delete monster;
    }

    return 0;
}

