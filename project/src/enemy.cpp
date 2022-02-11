#include <cstddef>

#include "enemy.hpp"

Wolf::Wolf() : hp(6), damage(11) { }

bool Wolf::hit(int damage) {
    hp -= damage;
    return hp > 0;
}

int Wolf::get_hp() {
    return hp;
}

int Wolf::get_damage() {
    return damage;
}

Dog::Dog() : hp(3), damage(6) { }

bool Dog::hit(int damage) {
    hp -= damage;
    return hp > 0;
}

int Dog::get_hp() {
    return hp;
}

int Dog::get_damage() {
    return damage;
}

Rat::Rat() : hp(2), damage(4) { }

bool Rat::hit(int damage) {
    hp -= damage;
    return hp > 0;
}

int Rat::get_hp() {
    return hp;
}

int Rat::get_damage() {
    return damage;
}



