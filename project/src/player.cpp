#include <cstddef>

#include "player.hpp"

Player::Player() : hp(100), damage(1), posX(0), posY(0) { }

bool Player::hit(int damage) {
    hp -= damage;
    return hp > 0;
}

int Player::get_hp() {
    return hp;
}

int Player::get_damage() {
    return damage;
}

int Player::get_posX() {
    return posX;
}
int Player::get_posY() {
    return posY;
}

void Player::set_posX(int i) {
    posX += i;
}

void Player::set_posY(int i) {
    posY += i;
}


