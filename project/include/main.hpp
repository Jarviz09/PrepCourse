#ifndef PROJECT_INCLUDE_MAIN_HPP_
#define PROJECT_INCLUDE_MAIN_HPP_

void get_step(Map& map, Player& player);
int next_step(Map& map, Player& player);
int kick_enemy();
int fight(Player& player, Enemy& monster);
void fighting(Player& player);

#endif  //  PROJECT_INCLUDE_MAIN_HPP_
