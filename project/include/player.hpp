#ifndef PROJECT_INCLUDE_PLAYER_HPP_
#define PROJECT_INCLUDE_PLAYER_HPP_

class Player {
 public:
    Player();
    bool hit(int damage);
    int get_hp();
    int get_damage();
    int get_posX();
    int get_posY();
    void set_posY(int i);
    void set_posX(int i);
 private:
    int hp;
    int damage;
    int posX;
    int posY;
};
#endif  //  PROJECT_INCLUDE_PLAYER_HPP_
