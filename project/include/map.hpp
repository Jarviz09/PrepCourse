#ifndef PROJECT_INCLUDE_MAP_HPP_
#define PROJECT_INCLUDE_MAP_HPP_

#include <cstddef>
#include <vector>

class Map {
 public:
    explicit Map(std::istream &is);
    int get_x();
    int get_y();
    void set_elem(int x, int y, char c);
    char get_elem(int x, int y);
    Map(Map &) = default;
    ~Map() = default;
 private:
    std::vector<std::vector<char>> map;
    int x;
    int y;
};

#endif  //  PROJECT_INCLUDE_MAP_HPP_

