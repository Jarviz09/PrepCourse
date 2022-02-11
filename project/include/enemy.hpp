#ifndef PROJECT_INCLUDE_ENEMY_HPP_
#define PROJECT_INCLUDE_ENEMY_HPP_

class Enemy {
 public:
    virtual
    bool hit(int damage) = 0;
    virtual
    int get_hp() = 0;
    virtual
    int get_damage() = 0;
    virtual ~Enemy() = default;
};

class Wolf : public Enemy {
 public:
    Wolf();
    bool hit(int damage) override;
    int get_hp();
    int get_damage();
 private:
    int hp;
    int damage;
};

class Dog : public Enemy {
 public:
    Dog();
    bool hit(int damage) override;
    int get_hp();
    int get_damage();
 private:
    int hp;
    int damage;
};

class Rat : public Enemy {
 public:
    Rat();
    bool hit(int damage) override;
    int get_hp();
    int get_damage();
 private:
    int hp;
    int damage;
};

#endif  //  PROJECT_INCLUDE_ENEMY_HPP_

